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

#ifndef PROTO_AST_BINARY_EXPRESSION_H
#define PROTO_AST_BINARY_EXPRESSION_H

#include <memory>

#include "common/token.h"
#include "expression.h"


enum class BinaryType
{
    
};


class BinaryExpression : public Expression
{
    public:
        BinaryExpression(
            Token& token,
            enum BinaryType type,
            std::unique_ptr<Expression>&& left,
            std::unique_ptr<Expression>&& right
        );

        /**
         * Returns the token associated with this binary expression.
         */
        Token& getToken();

        /**
         * Returns the type of binary operator held in this expression.
         */
        enum BinaryType& getBinaryType();

        /**
         * Returns the expression on the left side of the binary operator.
         */
        std::unique_ptr<Expression>& getLeft();

        /**
         * Returns the expression on the right side of the binary operator.
         */
        std::unique_ptr<Expression>& getRight();

    protected:
        Token                       token;  /* Token associated with this binary expression. */
        enum BinaryType             type;   /* Type of this binary expression. */
        std::unique_ptr<Expression> left;   /* Expression on the left side of the binary operator. */
        std::unique_ptr<Expression> right;  /* Expression on the right side of the binary operator. */
};

#endif
