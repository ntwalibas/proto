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

#include "checker/ast/definitions/variable.h"
#include "ast/statements/statement.h"
#include "checker/checker_error.h"
#include "checker/ast/program.h"
#include "ast/program.h"


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
            case DefinitionType::Variable: {
                VariableDefinition* var_def =
                    static_cast<VariableDefinition*>(def.get());
                VariableDefinitionChecker checker(var_def);
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

                    throw;
                }

                break;
            }

            case DefinitionType::Function: {
                throw std::invalid_argument("Function definitions are not currently checked.");
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
}
