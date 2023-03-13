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
#include "parsetree/expressions/ternaryif.h"
#include "common/token.h"


TernaryIfExpression::TernaryIfExpression(
    Token& token,
    std::unique_ptr<Expression>&& condition,
    std::unique_ptr<Expression>&& lvalue,
    std::unique_ptr<Expression>&& rvalue
) : Expression(ExpressionType::TernaryIf),
    token(token),
    condition(std::move(condition)),
    lvalue(std::move(lvalue)),
    rvalue(std::move(rvalue))
{}


/**
 * Returns the token associated with this ternary expression.
 */
Token&
TernaryIfExpression::getToken()
{
    return token;
}

/**
 * Returns the condition upon which branches are chosen.
 */
std::unique_ptr<Expression>&
TernaryIfExpression::getCondition()
{
    return condition;
}


/**
 * Returns the expression on the left side of the colon in ternary operator.
 */
std::unique_ptr<Expression>&
TernaryIfExpression::getLvalue()
{
    return lvalue;
}


/**
 * Returns the expression on the right side of the colon in ternary operator.
 */
std::unique_ptr<Expression>&
TernaryIfExpression::getRvalue()
{
    return rvalue;
}
