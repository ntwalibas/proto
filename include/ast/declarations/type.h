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
    Array
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


class ArrayTypeDeclaration : public TypeDeclaration
{
    public:
        ArrayTypeDeclaration(bool is_const, Token& token, long size, SimpleTypeDeclaration& simple_type);

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
         * Returns the size of this array type.
         */
        long getSize();

        /**
         * Returns the simple stored in the array.
         */
        SimpleTypeDeclaration& getSimpleType();

        /**
         * Compares the given type declaration is equal to the current type declaration.
         */
        bool operator==(ArrayTypeDeclaration& type_decl);
        bool operator!=(ArrayTypeDeclaration& type_decl);

    protected:
        bool                    is_const;       /* Whether this type declaration is qualified as const. */
        Token                   token;          /* The token with type information. */
        long                    size;           /* Size of the array. */
        SimpleTypeDeclaration   simple_type;    /* The simple type to store in the array. */
};


/**
 * Compares two type declarations.
 */
bool
typeDeclarationEquals(
    std::unique_ptr<TypeDeclaration>& left,
    std::unique_ptr<TypeDeclaration>& right
);

#endif
