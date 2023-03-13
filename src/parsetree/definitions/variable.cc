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

#include <utility>
#include <memory>

#include "parsetree/definitions/variable.h"
#include "parsetree/declarations/type.h"
#include "common/token.h"


VariableDefinition::VariableDefinition(
    Token& token,
    std::unique_ptr<TypeDeclaration>&& type_decl,
    std::unique_ptr<Expression>&& initializer
) : Definition(DefinitionType::Variable),
    token(token),
    type_decl(std::move(type_decl)),
    initializer(std::move(initializer))
{}


/**
 * Returns the token associated with this variable definition.
 */
Token&
VariableDefinition::getToken()
{
    return token;
}


/**
 * Returns the type of this variable definition.
 */
std::unique_ptr<TypeDeclaration>&
VariableDefinition::getTypeDeclaration()
{
    return type_decl;
}


/**
 * Returns the expression that initializes this variable definition.
 */
std::unique_ptr<Expression>&
VariableDefinition::getInitializer()
{
    return initializer;
}
