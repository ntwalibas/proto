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
#include <string>

#include "parsetree/expressions/expression.h"
#include "parsetree/expressions/unary.h"
#include "common/token.h"


UnaryExpression::UnaryExpression(
    Token& token,
    enum UnaryType type,
    std::unique_ptr<Expression>&& expression
) : Expression(ExpressionType::Unary),
    token(token),
    type(type),
    expression(std::move(expression))
{}


/**
 * Returns the token associated with this unary expression.
 */
Token&
UnaryExpression::getToken()
{
    return token;
}


/**
 * Returns the type of unary operator held in this expression.
 */
enum UnaryType&
UnaryExpression::getUnaryType()
{
    return type;
}


/**
 * Returns the expression the unary operator is applied to.
 */
std::unique_ptr<Expression>&
UnaryExpression::getExpression()
{
    return expression;
}


/**
 * Set the name of the function that corresponds to this unary.
 */
void
UnaryExpression::setFunctionName(std::string const& fun_name_)
{
    fun_name = fun_name_;
}


/**
 * Returns the name of the function that corresponds to this unary.
 */
std::string&
UnaryExpression::getFunctionName()
{
    return fun_name;
}
