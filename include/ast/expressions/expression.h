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

#ifndef PROTO_AST_EXPRESSION_H
#define PROTO_AST_EXPRESSION_H

#include "ast/statements/statement.h"


enum class ExpressionType {
    // Primary expressions
    Literal,
    Array,
    Variable,
    // FunctionCall,
    // Group
};


class Expression : public Statement
{
    public:
        Expression(
            enum ExpressionType type
        ) : Statement(StatementType::Expression),
            type(type)
        {}
        virtual ~Expression() {}

        /**
         * Returns the type of this expression.
         */
        enum ExpressionType getType() const
        {
            return type;
        }

    protected:
        enum ExpressionType type;   /* The type of expression of the derived class. */
};

#endif
