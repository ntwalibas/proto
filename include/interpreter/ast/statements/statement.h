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

#ifndef PROTO_STATEMENT_INTERPRETER_H
#define PROTO_STATEMENT_INTERPRETER_H

#include <cstdbool>
#include <memory>

#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/statements/statement.h"
#include "cleaner/ast/statements/continue.h"
#include "cleaner/ast/statements/return.h"
#include "cleaner/ast/statements/while.h"
#include "cleaner/ast/statements/break.h"
#include "cleaner/ast/statements/block.h"
#include "cleaner/ast/statements/for.h"
#include "cleaner/ast/statements/if.h"
#include "cleaner/symbols/scope.h"


class StatementInterpreter
{
    public:
        StatementInterpreter();

        /**
         * Interprets the given statement.
         */
        std::unique_ptr<CleanExpression> interpret(
            CleanStatement* stmt, CleanScope* scope);

        // Block
        std::unique_ptr<CleanExpression> interpretBlock(
            CleanBlockStatement* block_stmt);

        // If
        std::unique_ptr<CleanExpression> interpretIf(
            CleanIfStatement* if_stmt, CleanScope* scope);

        // For
        std::unique_ptr<CleanExpression> interpretFor(
            CleanForStatement* for_stmt, CleanScope* scope);

        // While
        std::unique_ptr<CleanExpression> interpretWhile(
            CleanWhileStatement* while_stmt, CleanScope* scope);

        // Break
        std::unique_ptr<CleanExpression> interpretBreak(
            CleanBreakStatement* br_stmt);

        // Continue
        std::unique_ptr<CleanExpression> interpretContinue(
            CleanContinueStatement* cont_stmt);

        // Return
        std::unique_ptr<CleanExpression> interpretReturn(
            CleanReturnStatement* ret_stmt, CleanScope* scope);

        // Expressions
        std::unique_ptr<CleanExpression> interpretExpression(
            CleanExpression* expr_stmt, CleanScope* scope);
    
    private:
        bool returned;
        bool broke;
        bool continued;
};

#endif
