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

#ifndef PROTO_AST_IF_STATEMENT_H
#define PROTO_AST_IF_STATEMENT_H

#include <memory>
#include <vector>

#include "ast/expressions/expression.h"
#include "ast/statements/statement.h"
#include "common/token.h"
#include "statement.h"
#include "block.h"


class IfStatement : public Statement
{
    public:
        IfStatement(
            Token& token,
            std::unique_ptr<Expression>&& condition,
            std::unique_ptr<BlockStatement>&& body
        );

        /**
         * Returns the token associated with the if statement.
         */
        Token& getToken();

        /**
         * Returns the condition upon which the conditional executes.
         */
        std::unique_ptr<Expression>& getCondition();

        /**
         * Returns the conditional's body.
         */
        std::unique_ptr<BlockStatement>& getBody();

        /**
         * Add elif branch.
         */
        void addElifBranch(std::unique_ptr<class ElifBranch>&& elif_branch);

        /**
         * Returns all the elif branches
         */
        std::vector<std::unique_ptr<class ElifBranch>>& getElifBranches();

        /**
         * Set the else branch.
         */
        void setElseBranch(std::unique_ptr<class ElseBranch>&& els_branch);

        /**
         * Returns the else branch.
         */
        std::unique_ptr<class ElseBranch>& getElseBranch();

    protected:
        Token                           token;          /* Token associated to this statement. */
        std::unique_ptr<Expression>     condition;      /* Condition upon which the conditional executes. */
        std::unique_ptr<BlockStatement> body;           /* Body of the conditional. */
        std::vector<std::unique_ptr<
            class ElifBranch>>          elif_branches;  /* Elif branches. **/
        std::unique_ptr<
            class ElseBranch>           else_branch;    /* Else branch. */
};


class ElifBranch
{
    public:
        ElifBranch(
            Token& token,
            std::unique_ptr<Expression>&& condition,
            std::unique_ptr<BlockStatement>&& body
        );

        /**
         * Returns the token associated with the elif branch.
         */
        Token& getToken();

        /**
         * Returns the condition upon which the branch executes.
         */
        std::unique_ptr<Expression>& getCondition();

        /**
         * Return the branch's body.
         */
        std::unique_ptr<BlockStatement>& getBody();

    protected:
        Token                           token;      /* Token associated to this statement. */
        std::unique_ptr<Expression>     condition;  /* Condition upon which the branch executes. */
        std::unique_ptr<BlockStatement> body;       /* Body of the branch. */
};


class ElseBranch
{
    public:
        ElseBranch(
            Token& token,
            std::unique_ptr<BlockStatement>&& body
        );

        /**
         * Returns the token associated with the else branch.
         */
        Token& getToken();

        /**
         * Return the branch's body.
         */
        std::unique_ptr<BlockStatement>& getBody();

    protected:
        Token                           token;      /* Token associated to this statement. */
        std::unique_ptr<BlockStatement> body;       /* Body of the branch. */
};

#endif
