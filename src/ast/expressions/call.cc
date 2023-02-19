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
#include "ast/expressions/call.h"
#include "common/token.h"


CallExpression::CallExpression(
    Token& token
) : Expression(ExpressionType::Call),
    token(token)
{}


/**
 * Returns the token associated with this function call expression.
 */
Token&
CallExpression::getToken()
{
    return token;
}

/**
 * Add an argument to this function call.
 */
void
CallExpression::addArgument(std::unique_ptr<Expression>&& argument)
{
    arguments.push_back(std::move(argument));
}

/**
 * Returns the arguments passed to the function in this call.
 */
std::vector<std::unique_ptr<Expression>>&
CallExpression::getArguments()
{
    return arguments;
}