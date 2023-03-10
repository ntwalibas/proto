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
#include "parsetree/statements/return.h"
#include "common/token.h"


ReturnStatement::ReturnStatement(
    Token& token,
    std::unique_ptr<Expression>&& expression
) : Statement(StatementType::Return),
    token(token),
    expression(std::move(expression))
{}


/**
 * Returns the token associated with the return statement.
 */
Token&
ReturnStatement::getToken()
{
    return token;
}


/**
 * Returns the expression to be returned.
 */
std::unique_ptr<Expression>& 
ReturnStatement::getExpression()
{
    return expression;
}
