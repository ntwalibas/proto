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

#ifndef PROTO_AST_ARRAY_EXPRESSION_H
#define PROTO_AST_ARRAY_EXPRESSION_H

#include <memory>
#include <vector>

#include "common/token.h"
#include "expression.h"


class ArrayExpression : public Expression
{
    public:
        ArrayExpression(Token& token);

        /**
         * Returns the token associated with this array expression.
         */
        Token& getToken();
        
        /**
         * Add an expression to this array's contents.
         */
        void addContent(std::unique_ptr<Expression>&& content);

        /**
         * Returns all the expressions held in this array expression.
         */
        std::vector<std::unique_ptr<Expression>>& getContents();

    protected:
        Token                                       token;      /* Token associated with this array. */
        std::vector<std::unique_ptr<Expression>>    contents;   /* The content of the array */
};

#endif
