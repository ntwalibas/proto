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

#include "parsetree/definitions/definition.h"
#include "parsetree/expressions/expression.h"
#include "parsetree/statements/block.h"
#include "parsetree/statements/for.h"
#include "common/token.h"


ForStatement::ForStatement(
    Token& token,
    std::unique_ptr<Definition>&& init_clause,
    std::unique_ptr<Expression>&& term_clause,
    std::unique_ptr<Expression>&& incr_clause,
    std::unique_ptr<BlockStatement>&& body
) : Statement(StatementType::For),
    token(token),
    init_clause(std::move(init_clause)),
    term_clause(std::move(term_clause)),
    incr_clause(std::move(incr_clause)),
    body(std::move(body))
{}


/**
 * Returns the token associated with the for statement.
 */
Token&
ForStatement::getToken()
{
    return token;
}


/**
 * Returns the loop initialization clause.
 */
std::unique_ptr<Definition>&
ForStatement::getInitClause()
{
    return init_clause;
}


/**
 * Returns the loop termination clause.
 */
std::unique_ptr<Expression>&
ForStatement::getTermClause()
{
    return term_clause;
}


/**
 * Returns the loop increment clause.
 */
std::unique_ptr<Expression>&
ForStatement::getIncrClause()
{
    return incr_clause;
}


/**
 * Return the loop's body.
 */
std::unique_ptr<BlockStatement>&
ForStatement::getBody()
{
    return body;
}
