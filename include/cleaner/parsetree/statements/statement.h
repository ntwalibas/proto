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

#ifndef PROTO_STATEMENT_CLEANER_H
#define PROTO_STATEMENT_CLEANER_H

#include <memory>

#include "cleaner/ast/statements/statement.h"
#include "cleaner/ast/statements/continue.h"
#include "parsetree/statements/statement.h"
#include "parsetree/statements/statement.h"
#include "cleaner/symbols/scope.forward.h"
#include "parsetree/statements/continue.h"
#include "cleaner/ast/statements/return.h"
#include "cleaner/ast/statements/while.h"
#include "cleaner/ast/statements/break.h"
#include "cleaner/ast/statements/block.h"
#include "parsetree/statements/return.h"
#include "parsetree/statements/while.h"
#include "parsetree/statements/block.h"
#include "parsetree/statements/break.h"
#include "cleaner/ast/statements/for.h"
#include "cleaner/ast/statements/if.h"
#include "parsetree/statements/for.h"
#include "parsetree/statements/if.h"


class StatementCleaner
{
    public:
        StatementCleaner();
        
        /**
         * Returns the statement AST node corresponding to the parse tree node.
         */
        std::unique_ptr<CleanStatement> clean(
            Statement* stmt,
            std::shared_ptr<CleanScope> const& scope
        );

        // Block
        std::unique_ptr<CleanBlockStatement> cleanBlock(
            BlockStatement* block_stmt,
            std::shared_ptr<CleanScope> const& scope
        );

        // If
        std::unique_ptr<CleanIfStatement> cleanIf(
            IfStatement* if_stmt,
            std::shared_ptr<CleanScope> const& scope
        );

        // For
        std::unique_ptr<CleanForStatement> cleanFor(
            ForStatement* for_stmt,
            std::shared_ptr<CleanScope> const& scope
        );

        // While
        std::unique_ptr<CleanWhileStatement> cleanWhile(
            WhileStatement* while_stmt,
            std::shared_ptr<CleanScope> const& scope
        );

        // Break
        std::unique_ptr<CleanBreakStatement> cleanBreak();

        // Contiue
        std::unique_ptr<CleanStatement> cleanContinue();

        // Return
        std::unique_ptr<CleanReturnStatement> cleanReturn(
            ReturnStatement* return_stmt,
            std::shared_ptr<CleanScope> const& scope
        );

        // Expression
        std::unique_ptr<CleanExpression> cleanExpression(
            Expression* expression_stmt,
            std::shared_ptr<CleanScope> const& scope
        );
};

#endif
