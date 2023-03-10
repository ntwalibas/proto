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

#ifndef PROTO_AST_BLOCK_STATEMENT_H
#define PROTO_AST_BLOCK_STATEMENT_H

#include <memory>
#include <vector>

#include "parsetree/definitions/definition.h"
#include "symbols/scope.h"
#include "common/token.h"
#include "statement.h"


class BlockStatement : public Statement
{
    public:
        BlockStatement(Token& token);

        /**
         * Returns the token associated with this block.
         */
        Token& getToken();

        /**
         * Add a definition to this block.
         */
        void addDefinition(std::unique_ptr<Definition>&& definition);

        /**
         * Returns definitions bound to this block.
         */
        std::vector<
            std::unique_ptr<Definition>>& getDefinitions();

        /**
         * Set the scope introduced by this block.
         */
        void setScope(std::shared_ptr<Scope>& scope_);

        /**
         * Returns the scope associated with this block. 
         */
        std::shared_ptr<Scope>& getScope();

    protected:
        Token                               token;          /* Token that indicates the start of this block. */
        std::vector<
            std::unique_ptr<Definition>>    definitions;    /* Definitions that appear in this block. */
        std::shared_ptr<Scope>              scope;          /* Scope containing all definitions in this block. */
};

#endif
