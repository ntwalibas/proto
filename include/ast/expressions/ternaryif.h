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

#ifndef PROTO_AST_TERNARY_IF_EXPRESSION_H
#define PROTO_AST_TERNARY_IF_EXPRESSION_H

#include <memory>

#include "common/token.h"
#include "expression.h"


class TernaryIfExpression : public Expression
{
    public:
        TernaryIfExpression(
            Token& token,
            std::unique_ptr<Expression>&& condition,
            std::unique_ptr<Expression>&& lvalue,
            std::unique_ptr<Expression>&& rvalue
        );

        /**
         * Returns the token associated with this ternary expression.
         */
        Token& getToken();

        /**
         * Returns the condition upon which branches are chosen.
         */
        std::unique_ptr<Expression>& getCondition();

        /**
         * Returns the expression on the left side of the colon in ternary operator.
         */
        std::unique_ptr<Expression>& getLvalue();

        /**
         * Returns the expression on the right side of the colon in ternary operator.
         */
        std::unique_ptr<Expression>& getRvalue();

    protected:
        Token                       token;      /* Token associated with this ternary expression. */
        std::unique_ptr<Expression> condition;  /* COndition upon which branches are chosen. */
        std::unique_ptr<Expression> lvalue;     /* Expression on the left side of the colon in ternary operator. */
        std::unique_ptr<Expression> rvalue;     /* Expression on the right side of the colon in ternary operator. */
};

#endif
