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

#include <memory>

#include "interpreter/ast/definitions/function.h"
#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/definitions/function.h"
#include "interpreter/interpreter.h"
#include "cleaner/symbols/scope.h"


Interpreter::Interpreter(
    CleanScope* scope
) : scope(scope)
{}

/**
 * Interprets the program starting with the main function found in the global scope.
 */
int
Interpreter::interpret()
{
    std::unique_ptr<CleanFunctionDefinition>& main_fun =
        scope->getSymbol<CleanFunctionDefinition>("main()");
    
    std::vector<std::unique_ptr<CleanExpression>> args{};
    
    std::unique_ptr<CleanExpression> ret_expr =
        FunctionDefinitionInterpreter().interpret(main_fun.get(), args);

    if (ret_expr && ret_expr->type == CleanExpressionType::SignedInt) {
        CleanSignedIntExpression* int_expr =
            static_cast<CleanSignedIntExpression*>(ret_expr.get());
        
        // The value will be of type int64_t
        // But since we are interpreting through C++, we convert to int
        return (int) int_expr->value;
    }

    // If we got a nullptr from main, we have an error
    return -1;
}
