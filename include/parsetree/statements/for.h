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

#ifndef PROTO_AST_FOR_STATEMENT_H
#define PROTO_AST_FOR_STATEMENT_H

#include <memory>

#include "parsetree/definitions/definition.h"
#include "parsetree/expressions/expression.h"
#include "parsetree/statements/statement.h"
#include "common/token.h"
#include "statement.h"
#include "block.h"


class ForStatement : public Statement
{
    public:
        ForStatement(
            Token& token,
            std::unique_ptr<Definition>&& init_clause,
            std::unique_ptr<Expression>&& term_clause,
            std::unique_ptr<Expression>&& incr_clause,
            std::unique_ptr<BlockStatement>&& body
        );

        /**
         * Returns the token associated with the for statement.
         */
        Token& getToken();

        /**
         * Returns the loop initialization clause.
         */
        std::unique_ptr<Definition>& getInitClause();

        /**
         * Returns the loop termination clause.
         */
        std::unique_ptr<Expression>& getTermClause();

        /**
         * Returns the loop increment clause.
         */
        std::unique_ptr<Expression>& getIncrClause();

        /**
         * Return the loop's body.
         */
        std::unique_ptr<BlockStatement>& getBody();

    protected:
        Token                           token;          /* Token associated to this statement. */
        std::unique_ptr<Definition>     init_clause;    /* Loop initialization clause. */
        std::unique_ptr<Expression>     term_clause;    /* Loop termination clause. */
        std::unique_ptr<Expression>     incr_clause;    /* Loop increment clause. */
        std::unique_ptr<BlockStatement> body;           /* Body of the loop. */
};

#endif
