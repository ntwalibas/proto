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
#include "ast/declarations/type.h"
#include "ast/expressions/cast.h"
#include "common/token.h"


CastExpression::CastExpression(
    Token& token,
    std::unique_ptr<Expression>&& expression,
    std::unique_ptr<TypeDeclaration>&& dest_type
) : Expression(ExpressionType::Cast),
    token(token),
    expression(std::move(expression)),
    dest_type(std::move(dest_type))
{}


/**
 * Returns the token associated with this cast expression.
 */
Token&
CastExpression::getToken()
{
    return token;
}


/**
 * Returns the expression to be cast to the given type.
 */
std::unique_ptr<Expression>&
CastExpression::getExpression()
{
    return expression;
}


/**
 * Returns the type the expression is to be cast to.
 */
std::unique_ptr<TypeDeclaration>&
CastExpression::getTypeDeclaration()
{
    return dest_type;
}
