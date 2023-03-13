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

#include "ast/expressions/expression.h"
#include "ast/expressions/binary.h"
#include "common/token.h"


BinaryExpression::BinaryExpression(
    Token& token,
    enum BinaryType type,
    std::unique_ptr<Expression>&& left,
    std::unique_ptr<Expression>&& right
) : Expression(ExpressionType::Binary),
    token(token),
    type(type),
    left(std::move(left)),
    right(std::move(right))
{}


/**
 * Returns the token associated with this binary expression.
 */
Token&
BinaryExpression::getToken()
{
    return token;
}


/**
 * Returns the type of binary operator held in this expression.
 */
enum BinaryType&
BinaryExpression::getBinaryType()
{
    return type;
}


/**
 * Returns the expression on the left side of the binary operator.
 */
std::unique_ptr<Expression>&
BinaryExpression::getLeft()
{
    return left;
}


/**
 * Returns the expression on the right side of the binary operator.
 */
std::unique_ptr<Expression>&
BinaryExpression::getRight()
{
    return right;
}


/**
 * Set the name of the function that corresponds to this binary.
 */
void
BinaryExpression::setFunctionName(std::string const& fun_name_)
{
    fun_name = fun_name_;
}


/**
 * Returns the name of the function that corresponds to this binary.
 */
std::string&
BinaryExpression::getFunctionName()
{
    return fun_name;
}
