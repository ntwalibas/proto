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

#ifndef PROTO_AST_TYPE_DECLARATION_H
#define PROTO_AST_TYPE_DECLARATION_H

#include <cstdbool>

#include "common/token.h"
#include "declaration.h"


enum class TypeCategory
{
    Simple,
    // Array
};


class TypeDeclaration : public Declaration
{
    public:
        virtual ~TypeDeclaration() {}

        /**
         * Returns the category of type declaration, simple or array.
         */
        enum TypeCategory& getTypeCategory();

        /**
         * Returns the type name.
         */
        virtual std::string getTypeName() = 0;

        /**
         * Returns true if this type declaration is const.
         */
        virtual bool isConst() const = 0;

    protected:
        TypeDeclaration(
            enum TypeCategory category
        ) : Declaration(DeclarationType::Type),
            category(category)
        {}

        enum TypeCategory category;
};


class SimpleTypeDeclaration : public TypeDeclaration
{
    public:
        SimpleTypeDeclaration(bool is_const, Token& token);
        SimpleTypeDeclaration(SimpleTypeDeclaration const& type_decl) noexcept = default;
        SimpleTypeDeclaration(SimpleTypeDeclaration&& type_decl) noexcept = default;
        SimpleTypeDeclaration& operator=(SimpleTypeDeclaration const& type_decl) noexcept = default;
        SimpleTypeDeclaration& operator=(SimpleTypeDeclaration&& type_decl) noexcept = default;
        ~SimpleTypeDeclaration() noexcept = default;

        /**
         * Returns the type name.
         */
        std::string getTypeName();

        /**
         * Returns true is this type declaration is const-qualified.
         */
        bool isConst() const;

        /**
         * Returns the token associated with this type declaration.
         */
        Token& getToken();

        /**
         * Compares the given type declaration is equal to the current type declaration.
         */
        bool operator==(SimpleTypeDeclaration& type_decl);
        bool operator!=(SimpleTypeDeclaration& type_decl);

    protected:
        bool    is_const;   /* Whether this type declaration is qualified as const. */
        Token   token;      /* The token with type information. */
};


/**
 * Compares two type declarations.
 */
bool
typeDeclarationEquals(
    std::unique_ptr<TypeDeclaration>& left,
    std::unique_ptr<TypeDeclaration>& right
);


/**
 * Returns a copy of the given type declaration.
 */
std::unique_ptr<TypeDeclaration>
copy(std::unique_ptr<TypeDeclaration>& source);

#endif
