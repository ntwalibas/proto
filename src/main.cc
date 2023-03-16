/*  This file is part of the Proto programming language
 * 
 *  Copyright (c) 2023- Ntwali Bashige Toussaint
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include <iostream>
#include <cstddef>
#include <memory>
#include <string>

#include "interpreter/interpreter.h"
#include "cleaner/symbols/scope.h"
#include "parsetree/program.h"
#include "cleaner/cleaner.h"
#include "checker/checker.h"
#include "utils/messages.h"
#include "parser/parser.h"
#include "ansi_colors.h"
#include "lexer/lexer.h"
#include "utils/lexer.h"
#include "utils/file.h"


int
compile(std::string const& source_path);


int
main(int argc, char const * argv[])
{
    if (argc != 2) {
        std::cout << "Usage: proto program" << std::endl;
    }
    else {
        return compile(std::string(argv[1]));
    }

    return 0;
}

int
compile(std::string const& source_path)
{
    /* We begin by making sure the given source path exists */
    if (fileExists(source_path) == false) {
        std::cerr << ANSI_BRIGHT_BOLD_RED "error" ANSI_COLOR_RESET
                ": file [" ANSI_RED << source_path << ANSI_COLOR_RESET
                "] was not found." << std::endl;
        return 1;
    }

    /* 
     * Frontend processing
     *
     * The outcome of this will be a symbol table that contains the AST.
     *
     * We create a lexical scope for the frontend so its associated data
     * is delete except the final processed AST.
     */
    std::shared_ptr<CleanScope> scope = nullptr;
    {
        std::string source = readFile(source_path);
        Lexer lexer(std::make_shared<std::string>(source), source_path);

        Program program;
        Parser parser(lexer);
        try {
            program = parser.parse();
            if (parser.errors.size() > 0) {
                for (auto& e : parser.errors) {
                    printMessage(
                        "error",
                        e.getToken(), e.getPrimaryMessage(),
                        e.getSecondaryMessage(), e.getToken().source_path
                    );
                }

                return 1;
            }
        } catch (ParserError& e) {
            for (auto& e : parser.errors) {
                printMessage(
                    "error",
                    e.getToken(), e.getPrimaryMessage(),
                    e.getSecondaryMessage(), e.getToken().source_path
                );
            }

            // To avoid displaying spirious fatal errors,
            // we only show them if there are no non-fatal errors
            if (! parser.errors.size())
                printMessage(
                    "error",
                    e.getToken(), e.getPrimaryMessage(),
                    e.getSecondaryMessage(), e.getToken().source_path
                );

            return 1;
        }

        Checker checker(program);
        try {
            checker.check();
            if (checker.errors.size() > 0) {
                for (auto& e : checker.errors) {
                    printMessage(
                        "error",
                        e.getToken(), e.getPrimaryMessage(),
                        e.getSecondaryMessage(), e.getToken().source_path
                    );
                }

                return 1;
            }
        } catch (CheckerError& e) {
            for (auto& e : checker.errors) {
                printMessage(
                    "error",
                    e.getToken(), e.getPrimaryMessage(),
                    e.getSecondaryMessage(), e.getToken().source_path
                );
            }
            
            if (! checker.errors.size())
                printMessage(
                    "error",
                    e.getToken(), e.getPrimaryMessage(),
                    e.getSecondaryMessage(), e.getToken().source_path
                );

            return 1;
        }

        Cleaner cleaner(program);
        scope = cleaner.clean();
        for (auto& w : cleaner.warnings) {
            printMessage(
                "warning",
                w.getToken(), w.getPrimaryMessage(),
                w.getSecondaryMessage(), w.getToken().source_path
            );
        }
    }

    /*
     * Backend processing
     *
     * We process the AST found in the scope.
     *
     * In this PoC, we just invoke the interpreter.
     */
    {
        Interpreter interpreter(scope.get());
        return interpreter.interpret();
    }

    return 0;
}
