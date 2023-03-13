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

#include "parsetree/expressions/expression.h"
#include "parsetree/expressions/literal.h"
#include "common/token.h"


LiteralExpression::LiteralExpression(
    Token& token,
    enum LiteralType type
) : Expression(ExpressionType::Literal),
    token(token),
    type(type)
{}


/**
 * Returns the token associated with this literal expression.
 */
Token&
LiteralExpression::getToken()
{
    return token;
}


/**
 * Returns the type of literal help in this expression.
 */
enum LiteralType&
LiteralExpression::getLiteralType()
{
    return type;
}
