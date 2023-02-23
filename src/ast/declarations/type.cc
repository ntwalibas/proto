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

#include <cstdbool>
#include <string>

#include "ast/declarations/type.h"
#include "common/token.h"


// TypeDeclaration
enum TypeCategory&
TypeDeclaration::getTypeCategory()
{
    return category;
}


// Simple type declaration
SimpleTypeDeclaration::SimpleTypeDeclaration(
    bool is_const,
    Token& token
) : TypeDeclaration(TypeCategory::Simple),
    is_const(is_const),
    token(token)
{}

/**
 * Returns the type name.
 */
std::string
SimpleTypeDeclaration::getTypeName()
{
    return token.getLexeme();
}

/**
 * Returns true is this type declaration is const-qualified.
 */
bool
SimpleTypeDeclaration::isConst() const
{
    return is_const;
}

/**
 * Returns the token associated with this type declaration.
 */
Token&
SimpleTypeDeclaration::getToken()
{
    return token;
}

/**
 * Compares the given type declaration is equal to the current type declaration.
 */
bool
SimpleTypeDeclaration::operator==(SimpleTypeDeclaration& type_decl)
{
    return token.getLexeme() == type_decl.getToken().getLexeme();
}

bool
SimpleTypeDeclaration::operator!=(SimpleTypeDeclaration& type_decl)
{
    return !(*this == type_decl);
}


// Array type declaration
ArrayTypeDeclaration::ArrayTypeDeclaration(
    bool is_const,
    Token& token,
    long size,
    SimpleTypeDeclaration& simple_type
) : TypeDeclaration(TypeCategory::Array),
    is_const(is_const),
    token(token),
    size(size),
    simple_type(simple_type)
{}

/**
 * Returns the type name.
 */
std::string
ArrayTypeDeclaration::getTypeName()
{
    std::string name = "[";
    name += std::to_string(size);
    name += "]";
    name += simple_type.getTypeName();
    return name;
}

/**
 * Returns true is this type declaration is const-qualified.
 */
bool
ArrayTypeDeclaration::isConst() const
{
    return is_const;
}

/**
 * Returns the token associated with this type declaration.
 */
Token&
ArrayTypeDeclaration::getToken()
{
    return token;
}

 /**
 * Returns the size of this array type.
 */
long
ArrayTypeDeclaration::getSize()
{
    return size;
}

/**
 * Returns the simple stored in the array.
 */
SimpleTypeDeclaration&
ArrayTypeDeclaration::getSimpleType()
{
    return simple_type;
}

/**
 * Compares the given type declaration is equal to the current type declaration.
 */
bool
ArrayTypeDeclaration::operator==(ArrayTypeDeclaration& type_decl)
{
    return (size == type_decl.getSize()) && (simple_type == type_decl.getSimpleType());
}

bool
ArrayTypeDeclaration::operator!=(ArrayTypeDeclaration& type_decl)
{
    return !(*this == type_decl);
}

/**
 * Compares two type declarations.
 */
bool
typeDeclarationEquals(
    std::unique_ptr<TypeDeclaration>& left,
    std::unique_ptr<TypeDeclaration>& right
)
{
    enum TypeCategory category = left->getTypeCategory();
    if (category != right->getTypeCategory())
        return false;

    if (category == TypeCategory::Simple) {
        SimpleTypeDeclaration* left_decl =
            static_cast<SimpleTypeDeclaration*>(left.get());
        SimpleTypeDeclaration* right_decl =
            static_cast<SimpleTypeDeclaration*>(right.get());
        return (* left_decl == * right_decl);
    }
    else {
        ArrayTypeDeclaration* left_decl =
            static_cast<ArrayTypeDeclaration*>(left.get());
        ArrayTypeDeclaration* right_decl =
            static_cast<ArrayTypeDeclaration*>(right.get());
        return (* left_decl == * right_decl);
    }
}


/**
 * Returns a copy of the given type declaration.
 */
std::unique_ptr<TypeDeclaration>
copy(std::unique_ptr<TypeDeclaration>& type_decl)
{
    if (type_decl->getTypeCategory() == TypeCategory::Simple) {
        SimpleTypeDeclaration* sim_type_del =
            static_cast<SimpleTypeDeclaration*>(type_decl.get());
        return std::make_unique<SimpleTypeDeclaration>(*sim_type_del);
    }
    else {
        ArrayTypeDeclaration* arr_type_del =
            static_cast<ArrayTypeDeclaration*>(type_decl.get());
        return std::make_unique<ArrayTypeDeclaration>(*arr_type_del);
    }
}
