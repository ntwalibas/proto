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

#include <cstddef>
#include <memory>
#include <vector>

#include "interpreter/ast/definitions/function.h"
#include "interpreter/ast/statements/statement.h"
#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/definitions/function.h"
#include "cleaner/symbols/scope.h"


/**
 * Interprets the given function definition.
 */
std::unique_ptr<CleanExpression>
FunctionDefinitionInterpreter::interpret(
    CleanFunctionDefinition* fun_def,
    std::vector<std::unique_ptr<CleanExpression>>& arguments
)
{
    // Create temporary variables in the function's scope
    //  and bind passed arguments to them
    for (
        std::vector<std::unique_ptr<CleanExpression>>::size_type i = 0;
        i < arguments.size();
        ++i
    ) {
        fun_def->scope->addSymbol<CleanVariableDefinition>(
            fun_def->parameters[i]->name,
            std::make_unique<CleanVariableDefinition>(
                "",
                nullptr,
                std::move(arguments[i])
            )
        );
    }

    std::unique_ptr<CleanExpression> ret_expr =
        StatementInterpreter().interpret(
            fun_def->body.get(),
            fun_def->scope.get()
        );

    // Delete the temporary variable definitions created above
    // Technically this is unnecessary (and may impact performance)
    // but we want to start the next interpretation with a clean slate
    fun_def->scope->clearSymbols<CleanVariableDefinition>();
    
    return ret_expr;
}
