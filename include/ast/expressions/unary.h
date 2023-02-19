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

#ifndef PROTO_AST_UNARY_EXPRESSION_H
#define PROTO_AST_UNARY_EXPRESSION_H

#include <memory>

#include "common/token.h"
#include "expression.h"


enum class UnaryType
{
    Plus,
    Minus,
    BitwiseNot,
    LogicalNot
};


class UnaryExpression : public Expression
{
    public:
        UnaryExpression(
            Token& token,
            enum UnaryType type,
            std::unique_ptr<Expression>&& expression
        );

        /**
         * Returns the token associated with this unary expression.
         */
        Token& getToken();

        /**
         * Returns the type of unary held in this expression.
         */
        enum UnaryType& getUnaryType();

        /**
         * Returns the expression the unary operator is applied to.
         */
        std::unique_ptr<Expression>& getExpression();

    protected:
        Token                       token;      /* Token associated with this unary expression. */
        enum UnaryType              type;       /* Type of this unary expression. */
        std::unique_ptr<Expression> expression; /* Expression the unary operator is applied to. */
};

#endif
