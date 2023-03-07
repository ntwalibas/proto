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
#include <vector>

#include "ast/definitions/definition.h"
#include "ast/statements/block.h"
#include "symbols/scope.h"
#include "common/token.h"


BlockStatement::BlockStatement(
    Token& token
) : Statement(StatementType::Block),
    token(token),
    scope(nullptr)
{}


/**
 * Returns the token associated with this block.
 */
Token&
BlockStatement::getToken()
{
    return token;
}


/**
 * Add a definition to this block.
 */
void
BlockStatement::addDefinition(std::unique_ptr<Definition>&& definition)
{
    definitions.push_back(std::move(definition));
}


/**
 * Returns definitions bound to this block.
 */
std::vector<std::unique_ptr<Definition>>&
BlockStatement::getDefinitions()
{
    return definitions;
}


/**
 * Set the scope introduced by this block.
 */
void
BlockStatement::setScope(std::shared_ptr<Scope>& scope_)
{
    scope = scope_;
}

/**
 * Returns the scope associated with this block. 
 */
std::shared_ptr<Scope>&
BlockStatement::getScope()
{
    return scope;
}