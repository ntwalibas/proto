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
#include <string>

#include "parser/parser.h"
#include "utils/parser.h"
#include "ast/program.h"
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
        std::cout << "Usage: nova program" << std::endl;
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

    /* 2. Read the source */
    std::string source = readFile(source_path);

    /* 3. Lex the source */
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    // printTokens(lexer);

    /* 4. Parse tokens */
    Parser parser(lexer);
    try {
        Program program = parser.parse();
        if (parser.errors.size() > 0) {
            for (auto& e : parser.errors) {
                printError(
                    e.getToken(),
                    e.getPrimaryMessage(),
                    e.getSecondaryMessage(),
                    e.getToken().source_path
                );
            }
        }
        else {
            std::cout << "The program has " << program.getDefinitions().size() << " definitions." << std::endl;
        }
    } catch (ParserError& e) {
        printError(
            e.getToken(),
            e.getPrimaryMessage(),
            e.getSecondaryMessage(),
            e.getToken().source_path
        );

        return 1;
    }
    

    return 0;
}
