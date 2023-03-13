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

#include "parsetree/expressions/expression.h"
#include "parsetree/statements/block.h"
#include "parsetree/statements/if.h"
#include "common/token.h"


// If conditional
IfStatement::IfStatement(
    Token& token,
    std::unique_ptr<Expression>&& condition,
    std::unique_ptr<BlockStatement>&& body
) : Statement(StatementType::If),
    token(token),
    condition(std::move(condition)),
    body(std::move(body)),
    else_branch(nullptr)
{}

/**
 * Returns the token associated with the if statement.
 */
Token&
IfStatement::getToken()
{
    return token;
}

/**
 * Returns the condition upon which the conditional executes.
 */
std::unique_ptr<Expression>&
IfStatement::getCondition()
{
    return condition;
}

/**
 * Returns the conditional's body.
 */
std::unique_ptr<BlockStatement>&
IfStatement::getBody()
{
    return body;
}

/**
 * Add elif branch.
 */
void 
IfStatement::addElifBranch(std::unique_ptr<ElifBranch>&& elif_branch)
{
    elif_branches.push_back(std::move(elif_branch));
}

/**
 * Returns all the elif branches
 */
std::vector<std::unique_ptr<ElifBranch>>&
IfStatement::getElifBranches()
{
    return elif_branches;
}

/**
 * Set the else branch.
 */
void
IfStatement::setElseBranch(std::unique_ptr<ElseBranch>&& els_branch)
{
    else_branch = std::move(els_branch);
}

/**
 * Returns the else branch.
 */
std::unique_ptr<ElseBranch>&
IfStatement::getElseBranch()
{
    return else_branch;
}


// Elif branch
ElifBranch::ElifBranch(
    Token& token,
    std::unique_ptr<Expression>&& condition,
    std::unique_ptr<BlockStatement>&& body
) : token(token),
    condition(std::move(condition)),
    body(std::move(body))
{}

/**
 * Returns the token associated with the elif branch.
 */
Token&
ElifBranch::getToken()
{
    return token;
}

/**
 * Returns the condition upon which the branch executes.
 */
std::unique_ptr<Expression>&
ElifBranch::getCondition()
{
    return condition;
}

/**
 * Return the branch's body.
 */
std::unique_ptr<BlockStatement>&
ElifBranch::getBody()
{
    return body;
}


// Else branch
ElseBranch::ElseBranch(
    Token& token,
    std::unique_ptr<BlockStatement>&& body
) : token(token),
    body(std::move(body))
{}

/**
 * Returns the token associated with the else branch.
 */
Token&
ElseBranch::getToken()
{
    return token;
}

/**
 * Return the branch's body.
 */
std::unique_ptr<BlockStatement>&
ElseBranch::getBody()
{
    return body;
}
