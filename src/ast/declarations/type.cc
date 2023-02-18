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
 * Returns the simple stored in the array.
 */
SimpleTypeDeclaration&
ArrayTypeDeclaration::getSimpleType()
{
    return simple_type;
}
