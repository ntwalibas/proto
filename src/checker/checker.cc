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

#include "checker/checker_error.h"
#include "checker/ast/program.h"
#include "checker/checker.h"
#include "common/token.h"


Checker::Checker(
    Program& program
) : program(program)
{}


/**
 * Checks that the program confirms to the language semantics..
 * Returns the AST annotated with further information to aid later stages.
 */
void
Checker::check()
{
    ProgramChecker prog_checker(program);

    try {
        prog_checker.check();
        if (prog_checker.errors.size() > 0) {
            errors.insert(
                errors.end(),
                prog_checker.errors.begin(),
                prog_checker.errors.end()
            );
        }
    } catch (CheckerError& e) {
        throw;
    }
}
