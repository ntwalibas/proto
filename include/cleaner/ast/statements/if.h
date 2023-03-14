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

#ifndef PROTO_AST_CLEAN_IF_STATEMENT_H
#define PROTO_AST_CLEAN_IF_STATEMENT_H

#include <utility>
#include <cstddef>
#include <memory>
#include <vector>

#include "cleaner/ast/expressions/expression.h"
#include "statement.h"
#include "block.h"


struct CleanElifBranch
{
    CleanElifBranch(
        std::unique_ptr<CleanExpression>&& condition,
        std::unique_ptr<CleanBlockStatement>&& body
    ) : condition(std::move(condition)),
        body(std::move(body))
    {}

    std::unique_ptr<CleanExpression> condition;
    std::unique_ptr<CleanBlockStatement> body;
};

struct CleanElseBranch
{
    CleanElseBranch(
        std::unique_ptr<CleanBlockStatement> body
    ) : body(std::move(body))
    {}

    std::unique_ptr<CleanBlockStatement> body;
};


struct CleanIfStatement : public CleanStatement
{
    CleanIfStatement(
        std::unique_ptr<CleanExpression>&& condition,
        std::unique_ptr<CleanBlockStatement>&& body
    ) : CleanStatement(CleanStatementType::If),
        condition(std::move(condition)),
        body(std::move(body))
    {}

    std::unique_ptr<CleanExpression> condition;
    std::unique_ptr<CleanBlockStatement> body;
    std::vector<
        std::unique_ptr<CleanElifBranch>> elif_branches;
    std::unique_ptr<CleanElseBranch> else_branch;
};

#endif
