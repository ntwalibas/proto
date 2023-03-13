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
#include <cstddef>
#include <memory>
#include <string>

#include "parsetree/declarations/variable.h"
#include "parsetree/definitions/function.h"
#include "parsetree/declarations/type.h"
#include "parsetree/statements/block.h"
#include "common/token.h"


FunctionDefinition::FunctionDefinition(
    Token& token
) : Definition(DefinitionType::Function),
    token(token),
    return_type(nullptr),
    body(nullptr)
{}


/**
 * Returns the token associated with this variable definition.
 */
Token&
FunctionDefinition::getToken()
{
    return token;
}


/**
 * Add a parameter to this function.
 */
void
FunctionDefinition::addParameter(std::unique_ptr<VariableDeclaration>&& parameter)
{
    parameters.push_back(std::move(parameter));
}


/**
 * Returns parameters bound to this function.
 */
std::vector<std::unique_ptr<VariableDeclaration>>&
FunctionDefinition::getParameters()
{
    return parameters;
}

/**
 * Set the return type of this function definition.
 */
void
FunctionDefinition::setReturnType(std::unique_ptr<TypeDeclaration>&& ret_type)
{
    return_type = std::move(ret_type);
}


/**
 * Returns this function return type.
 */
std::unique_ptr<TypeDeclaration>&
FunctionDefinition::getReturnType()
{
    return return_type;
}

/**
 * Sets the body of the function.
 */
void
FunctionDefinition::setBody(std::unique_ptr<BlockStatement>&& fun_body)
{
    body = std::move(fun_body);
}

/**
 * Returns the body of the function.
 */
std::unique_ptr<BlockStatement>&
FunctionDefinition::getBody()
{
    return body;
}

/**
 * Returns the mangled name of this function.
 */
std::string
FunctionDefinition::getMangledName()
{
    std::string name = token.getLexeme() + "(";

    for (auto it = parameters.begin(); it != parameters.end(); ++it) {
        name += (*it)->getTypeDeclaration()->getTypeName();
        if (next(it) != parameters.end())
            name += ",";
    }

    name += ")";

    return name;
}
