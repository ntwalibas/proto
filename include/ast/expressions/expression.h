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

#include <utility>
#include <cstddef>
#include <memory>

#include "ast/statements/statement.h"
#include "ast/declarations/type.h"


enum class ExpressionType {
    // Primary
    Literal,
    // Array,
    Variable,
    Group,
    Call,

    // Unary
    Unary,

    // Binary
    Binary,

    // Assignment
    Assignment
};


class Expression : public Statement
{
    public:
        Expression(
            enum ExpressionType type
        ) : Statement(StatementType::Expression),
            type(type),
            type_decl(nullptr)
        {}
        virtual ~Expression() {}

        /**
         * Returns the type of this expression.
         */
        enum ExpressionType getType() const
        {
            return type;
        }

        /**
         * Returns the token associated with this expression.
         */
        virtual Token& getToken() = 0;

        /**
         * Set the type declaration of this expression.
         */
        void setTypeDeclaration(std::unique_ptr<TypeDeclaration>&& type_decl_)
        {
            type_decl = std::move(type_decl_);
        }

        /**
         * Returns the type declaration of this expression.
         */
        std::unique_ptr<TypeDeclaration>& getTypeDeclaration()
        {
            return type_decl;
        }

    protected:
        enum ExpressionType     type;       /* The type of expression of the derived class. */
        std::unique_ptr<
            TypeDeclaration>    type_decl;  /* Type (declaration) of this expression. */
};

#endif
