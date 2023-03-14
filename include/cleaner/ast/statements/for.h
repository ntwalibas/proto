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

#ifndef PROTO_AST_CLEAN_FOR_STATEMENT_H
#define PROTO_AST_CLEAN_FOR_STATEMENT_H

#include <utility>
#include <memory>

#include "cleaner/ast/expressions/expression.h"
#include "cleaner/symbols/scope.forward.h"
#include "statement.h"
#include "block.h"


struct CleanForStatement : public CleanStatement
{
    CleanForStatement(
        std::unique_ptr<CleanExpression>&& init_clause,
        std::unique_ptr<CleanExpression>&& term_clause,
        std::unique_ptr<CleanExpression>&& incr_clause,
        std::unique_ptr<CleanBlockStatement>&& body,
        std::shared_ptr<CleanScope>& scope
    ) : CleanStatement(CleanStatementType::For),
        init_clause(std::move(init_clause)),
        term_clause(std::move(term_clause)),
        incr_clause(std::move(incr_clause)),
        body(std::move(body)),
        scope(scope)
    {}

    std::unique_ptr<CleanExpression> init_clause;
    std::unique_ptr<CleanExpression> term_clause;
    std::unique_ptr<CleanExpression> incr_clause;
    std::unique_ptr<CleanBlockStatement> body;
    std::shared_ptr<CleanScope> scope;
};

#endif
