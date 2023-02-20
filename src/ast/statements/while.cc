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

#include "ast/expressions/expression.h"
#include "ast/statements/while.h"
#include "ast/statements/block.h"
#include "common/token.h"


WhileStatement::WhileStatement(
    Token& token,
    std::unique_ptr<Expression>&& condition,
    std::unique_ptr<BlockStatement>&& body
) : Statement(StatementType::While),
    token(token),
    condition(std::move(condition)),
    body(std::move(body))
{}


/**
 * Returns the token associated with the while statement.
 */
Token&
WhileStatement::getToken()
{
    return token;
}


/**
 * Returns the condition upon which the loop executes.
 */
std::unique_ptr<Expression>&
WhileStatement::getCondition()
{
    return condition;
}


/**
 * Return the loop's body.
 */
std::unique_ptr<BlockStatement>&
WhileStatement::getBody()
{
    return body;
}
