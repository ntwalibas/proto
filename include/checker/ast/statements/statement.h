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

#include "ast/statements/statement.h"
#include "ast/statements/block.h"
#include "symbols/scope.h"


class StatementChecker
{
    public:
        StatementChecker(
            Statement* stmt,
            std::shared_ptr<Scope> const& scope
        );
        
        /**
         * Checks that any given statement obeys the language semantics
         */
        void check();

        // Block
        void checkBlock();

        // If
        void checkIf();

        // For
        void checkFor();

        // While
        void checkWhile();

        // Break
        void checkBreak();

        // Contiue
        void checkContinue();

        // Return
        void checkReturn();

        // Expression
        void checkExpression();

    private:
        bool inside_loop;
        Statement* stmt;
        std::shared_ptr<Scope> const& scope;
};

#endif
