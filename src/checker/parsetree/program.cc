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

#include <cstdbool>
#include <memory>
#include <string>

#include "checker/parsetree/definitions/function.h"
#include "checker/parsetree/definitions/variable.h"
#include "parsetree/definitions/function.h"
#include "parsetree/definitions/variable.h"
#include "parsetree/statements/statement.h"
#include "checker/parsetree/program.h"
#include "checker/checker_error.h"
#include "parsetree/program.h"
#include "symbols/scope.h"


ProgramChecker::ProgramChecker(
    Program& program
) : program(program)
{}


/**
 * Check if all the definitions in the program are semantically valid.
 */
void
ProgramChecker::check()
{
    for (auto& def: program.getDefinitions()) {
        switch(def->getType()) {
            case DefinitionType::Function: {
                FunctionDefinition* fun_def =
                    static_cast<FunctionDefinition*>(def.get());
                FunctionDefinitionChecker checker(fun_def, program.getScope());
                try {
                    checker.check(def);
                } catch (CheckerError const& e) {
                    if (! e.isFatal())
                        errors.push_back(e);
                    else
                        throw;
                }

                break;
            }

            case DefinitionType::Variable: {
                VariableDefinition* var_def =
                    static_cast<VariableDefinition*>(def.get());
                VariableDefinitionChecker checker(var_def, program.getScope());
                try {
                    checker.check();

                    // Add the variable definition to the symbol table
                    program.getScope()->addDefinition(
                        def->getToken().getLexeme(),
                        def
                    );
                } catch (CheckerError const& e) {
                    if (! e.isFatal())
                        errors.push_back(e);
                    else
                        throw;
                }

                break;
            }

            case DefinitionType::Statement: {
                std::unique_ptr<Statement> stmt(
                    static_cast<Statement*>(def.release())
                );

                throw CheckerError(
                    stmt->getToken(),
                    "unexpected statement",
                    "a statement cannot occur at file scope, it must be inside a function",
                    true
                );
            }
        }
    }

    // Make sure we have a main function and it is valid
    if (program.getScope()->hasDefinition("main()")) {
        std::unique_ptr<Definition>& def =
            program.getScope()->getDefinition("main()");
        FunctionDefinition* main_fun =
            static_cast<FunctionDefinition*>(def.get());
        
        if (main_fun->getReturnType()->getTypeName() != "int") {
            throw CheckerError(
                program.getDefinitions()[0]->getToken(),
                "invalid main function",
                "the main function must have return type `int`",
                true
            );
        }
    }
    else {
        throw CheckerError(
            program.getDefinitions()[0]->getToken(),
            "missing entry point",
            std::string("the program is missing a main function with signature ") +
            "`main()->int` to serve as entry point",
            true
        );
    }
}
