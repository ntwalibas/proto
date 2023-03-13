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
#include <cstddef>

#include "ast/definitions/definition.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/assignment.h"
#include "common/token.h"


AssignmentExpression::AssignmentExpression(
    Token& token,
    enum AssignmentType type,
    std::unique_ptr<Expression>&& lvalue,
    std::unique_ptr<Expression>&& rvalue
) : Expression(ExpressionType::Assignment),
    token(token),
    type(type),
    lvalue(std::move(lvalue)),
    rvalue(std::move(rvalue)),
    var_def(nullptr)
{}


/**
 * Returns the token associated with this assignment expression.
 */
Token&
AssignmentExpression::getToken()
{
    return token;
}

/**
 * Returns the type of assignment.
 */
enum AssignmentType
AssignmentExpression::getAssignmentType()
{
    return type;
}


/**
 * Returns the expression on the left side of the assignment operator.
 */
std::unique_ptr<Expression>&
AssignmentExpression::getLvalue()
{
    return lvalue;
}


/**
 * Sets the expression on the right side of the assignment expression.
 */
void
AssignmentExpression::setRvalue(std::unique_ptr<Expression>&& rvalue_)
{
    rvalue = std::move(rvalue_);
}


/**
 * Returns the expression on the right side of the assignment operator.
 */
std::unique_ptr<Expression>&
AssignmentExpression::getRvalue()
{
    return rvalue;
}


/**
 * Sets the variable definition introduced by this assignment, if any.
 */
void
AssignmentExpression::setVariableDefinition(std::unique_ptr<Definition>&& var_def_)
{
    var_def = std::move(var_def_);
}

/**
 * Returns the variable definition introduced by this assignment, if any.
 */
std::unique_ptr<Definition>&
AssignmentExpression::getVariableDefinition()
{
    return var_def;
}

/**
 * Set the name of the function that corresponds to this call.
 */
void
AssignmentExpression::setFunctionName(std::string const& fun_name_)
{
    fun_name = fun_name_;
}

/**
 * Returns the name of the function that corresponds to this call.
 */
std::string&
AssignmentExpression::getFunctionName()
{
    return fun_name;
}
