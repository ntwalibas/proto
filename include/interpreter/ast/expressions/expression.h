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

#ifndef PROTO_EXPRESSION_INTERPRETER_H
#define PROTO_EXPRESSION_INTERPRETER_H

#include "cleaner/ast/expressions/expression.h"
#include "cleaner/symbols/scope.h"


class ExpressionInterpreter
{
    public:
        ExpressionInterpreter(CleanScope* scope);

        /**
         * Interprets the given expression.
         */
        std::unique_ptr<CleanExpression> interpret(
            CleanExpression* expr);

        // Bool
        std::unique_ptr<CleanBoolExpression> interpretBool(
            CleanBoolExpression* bool_expr);

        // Signed int
        std::unique_ptr<CleanSignedIntExpression> interpretSignedInt(
            CleanSignedIntExpression* int_expr);

        // Unsigned int
        std::unique_ptr<CleanUnsignedIntExpression> interpretUnsignedInt(
            CleanUnsignedIntExpression* uint_expr);

        // Float
        std::unique_ptr<CleanFloatExpression> interpretFloat(
            CleanFloatExpression* float_expr);

        // String
        std::unique_ptr<CleanStringExpression> interpretString(
            CleanStringExpression* string_expr);

        // Call
        std::unique_ptr<CleanExpression> interpretCall(
            CleanCallExpression* call_expr,
            CleanScope* scope);
        
        // Intrinsic
        std::unique_ptr<CleanExpression> interpretIntrinsic(
            CleanIntrinsicExpression* intr_expr,
            CleanScope* scope);

    private:
        CleanScope* scope;
};

#endif
