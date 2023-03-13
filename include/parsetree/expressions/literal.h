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

#ifndef PROTO_AST_LITERAL_EXPRESSION_H
#define PROTO_AST_LITERAL_EXPRESSION_H

#include "common/token.h"
#include "expression.h"


enum class LiteralType
{
    Boolean,
    Integer,
    Float,
    String
};


class LiteralExpression : public Expression
{
    public:
        LiteralExpression(Token& token, enum LiteralType type);

        /**
         * Returns the token associated with this literal expression.
         */
        Token& getToken();

        /**
         * Returns the type of literal help in this expression.
         */
        enum LiteralType& getLiteralType();

    protected:
        Token               token;      /* Token associated with this literal. */
        enum LiteralType    type;       /* Type of this literal */
};

#endif
