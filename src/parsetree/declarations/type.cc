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

#include "parsetree/declarations/type.h"
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

/**
 * Compares two type declarations.
 */
bool
typeDeclarationEquals(
    std::unique_ptr<TypeDeclaration>& left,
    std::unique_ptr<TypeDeclaration>& right
)
{
    SimpleTypeDeclaration* left_decl =
        static_cast<SimpleTypeDeclaration*>(left.get());
    SimpleTypeDeclaration* right_decl =
        static_cast<SimpleTypeDeclaration*>(right.get());
    return (* left_decl == * right_decl);
}


/**
 * Returns a copy of the given type declaration.
 */
std::unique_ptr<TypeDeclaration>
copy(std::unique_ptr<TypeDeclaration>& type_decl)
{
    SimpleTypeDeclaration* sim_type_del =
        static_cast<SimpleTypeDeclaration*>(type_decl.get());
    return std::make_unique<SimpleTypeDeclaration>(*sim_type_del);
}
