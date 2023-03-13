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

#ifndef PROTO_STATEMENT_CHECKER_H
#define PROTO_STATEMENT_CHECKER_H

#include <memory>

#include "checker/parsetree/expressions/expression.h"
#include "parsetree/statements/statement.h"
#include "parsetree/statements/statement.h"
#include "parsetree/statements/continue.h"
#include "parsetree/statements/return.h"
#include "parsetree/declarations/type.h"
#include "parsetree/statements/while.h"
#include "parsetree/statements/block.h"
#include "parsetree/statements/break.h"
#include "parsetree/statements/for.h"
#include "parsetree/statements/if.h"
#include "symbols/scope.h"


class StatementChecker
{
    public:
        StatementChecker(
            std::unique_ptr<TypeDeclaration>& ret_type_decl
        );
        
        /**
         * Checks that any given statement obeys the language semantics
         */
        void check(
            Statement* stmt,
            std::shared_ptr<Scope> const& scope
        );

        // Block
        void checkBlock(
            BlockStatement* block_stmt,
            std::shared_ptr<Scope> const& scope
        );

        // If
        void checkIf(
            IfStatement* if_stmt,
            std::shared_ptr<Scope> const& scope
        );

        // For
        void checkFor(
            ForStatement* for_stmt,
            std::shared_ptr<Scope> const& scope
        );

        // While
        void checkWhile(
            WhileStatement* while_stmt,
            std::shared_ptr<Scope> const& scope
        );

        // Break
        void checkBreak(
            BreakStatement* break_stmt,
            std::shared_ptr<Scope> const& scope
        );

        // Contiue
        void checkContinue(
            ContinueStatement* continue_stmt,
            std::shared_ptr<Scope> const& scope
        );

        // Return
        void checkReturn(
            ReturnStatement* return_stmt,
            std::shared_ptr<Scope> const& scope
        );

        // Expression
        void checkExpression(
            Expression* expression_stmt,
            std::shared_ptr<Scope> const& scope
        );

    private:
        bool                                inside_loop;    /* Flag that indicates if we are inside a loop. */
        std::unique_ptr<TypeDeclaration>&   ret_type_decl;  /* Function return type for checking return statements. */
};

#endif
