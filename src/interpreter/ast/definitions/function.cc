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
#include <utility>
#include <memory>
#include <vector>
#include <string>
#include <map>

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
    // If the scope already contains variables,
    // we probably have a recursive call, we save them on stack frame
    std::map<std::string,std::unique_ptr<CleanVariableDefinition>>& var_defs =
        fun_def->scope->getSymbols<CleanVariableDefinition>();
    for (auto& [name, var_def]: var_defs) {
        // new_frame.push_back(std::move(var_def));
        fun_def->stack_frame.push(std::move(var_def));
    }

    // Create temporary variables in the function's scope
    // and bind passed arguments to them
    for (
        std::vector<std::unique_ptr<CleanExpression>>::size_type i = 0;
        i < arguments.size();
        ++i
    ) {
        fun_def->scope->addSymbol<CleanVariableDefinition>(
            fun_def->parameters[i]->name,
            std::make_unique<CleanVariableDefinition>(
                fun_def->parameters[i]->name,
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
    
    // We restore the scope after this function's body interpretation
    if (fun_def->stack_frame.size() > 0) {
        for (int i = 0; i < fun_def->parameters.size(); ++i) {
            std::unique_ptr<CleanVariableDefinition>& var_def =
                fun_def->stack_frame.top();
            fun_def->scope->addSymbol<CleanVariableDefinition>(
                var_def->name,
                std::move(var_def)
            );
            fun_def->stack_frame.pop();
        }
    }
    // If the stack frame is empty,
    // there was no recursion (or we are at the end of recursion), we empty the scope
    else {
        fun_def->scope->clearSymbols<CleanVariableDefinition>();
    }
    
    return ret_expr;
}
