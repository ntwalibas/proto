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
#include <vector>

#include "ast/expressions/expression.h"
#include "ast/expressions/array.h"
#include "common/token.h"


ArrayExpression::ArrayExpression(
    Token& token
) : Expression(ExpressionType::Array),
    token(token)
{}


/**
 * Returns the token associated with this variable definition.
 */
Token&
ArrayExpression::getToken()
{
    return token;
}

/**
 * Add an expression to this array's contents.
 */
void
ArrayExpression::addContent(std::unique_ptr<Expression>&& content)
{
    contents.push_back(std::move(content));
}

/**
 * Returns all the expressions held in this array expression.
 */
std::vector<std::unique_ptr<Expression>>&
ArrayExpression::getContents()
{
    return contents;
}