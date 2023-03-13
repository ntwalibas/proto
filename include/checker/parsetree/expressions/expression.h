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

#ifndef PROTO_EXPRESSION_CHECKER_H
#define PROTO_EXPRESSION_CHECKER_H

#include <memory>

#include "parsetree/expressions/expression.h"
#include "parsetree/declarations/type.h"
#include "symbols/scope.h"


class ExpressionChecker
{
    public:
        ExpressionChecker(
            Expression* expr,
            std::shared_ptr<Scope> const& scope
        );
        
        /**
         * Checks that any given expression obeys the language semantics
         */
        std::unique_ptr<TypeDeclaration>& check();

        // Literals
        std::unique_ptr<TypeDeclaration>& checkLiteral();

        // Cast
        std::unique_ptr<TypeDeclaration>& checkCast();

        // Variable
        std::unique_ptr<TypeDeclaration>& checkVariable();

        // Group
        std::unique_ptr<TypeDeclaration>& checkGroup();

        // Call
        std::unique_ptr<TypeDeclaration>& checkCall();

        // Unary
        std::unique_ptr<TypeDeclaration>& checkUnary();

        // Binary
        std::unique_ptr<TypeDeclaration>& checkBinary();

        // Ternary
        std::unique_ptr<TypeDeclaration>& checkTernaryIf();

        // Assignment
        std::unique_ptr<TypeDeclaration>& checkAssignment();

    private:
        Expression* expr;
        std::shared_ptr<Scope> const& scope;
};

#endif
