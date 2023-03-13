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

#include "parsetree/expressions/expression.h"
#include "parsetree/expressions/group.h"
#include "common/token.h"


GroupExpression::GroupExpression(
    Token& token,
    std::unique_ptr<Expression>&& expression
) : Expression(ExpressionType::Group),
    token(token),
    expression(std::move(expression))
{}


/**
 * Returns the token associated with this group expression.
 */
Token&
GroupExpression::getToken()
{
    return token;
}


/**
 * Returns the expression held inside in this group.
 */
std::unique_ptr<Expression>&
GroupExpression::getExpression()
{
    return expression;
}
