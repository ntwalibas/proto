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

#ifndef PROTO_AST_STATEMENT_H
#define PROTO_AST_STATEMENT_H

#include "ast/definitions/definition.h"
#include "common/token.h"


enum class StatementType {
    Block,
    If,
    For,
    While,
    Break,
    Continue,
    Return,
    Expression
};


class Statement : public Definition
{
    public:
        Statement(
            enum StatementType type
        ) : Definition(DefinitionType::Statement),
            type(type)
        {}
        virtual ~Statement() {};

        /**
         * Returns the type of this definition.
         */
        enum StatementType getType() const
        {
            return type;
        }

        /**
         * Returns the token associated with this statement.
         */
        virtual Token& getToken() = 0;

    protected:
        enum StatementType type;     /* The type of definition of the derived class. */
};

#endif
