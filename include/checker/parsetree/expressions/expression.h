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
// #include "parsetree/expressions/assignment.h"
// #include "parsetree/expressions/ternaryif.h"
// #include "parsetree/expressions/variable.h"
// #include "parsetree/expressions/literal.h"
// #include "parsetree/expressions/binary.h"
// #include "parsetree/expressions/unary.h"
// #include "parsetree/expressions/group.h"
// #include "parsetree/expressions/call.h"
// #include "parsetree/expressions/cast.h"
#include "parsetree/declarations/type.h"
#include "symbols/scope.h"


class ExpressionChecker
{
    public:
        ExpressionChecker(
            std::shared_ptr<Scope> const& scope
        );
        
        /**
         * Checks that any given expression obeys the language semantics
         */
        std::unique_ptr<TypeDeclaration>& check(Expression* expr);

        // Literals
        std::unique_ptr<TypeDeclaration>& checkLiteral(Expression* expr);

        // Cast
        std::unique_ptr<TypeDeclaration>& checkCast(Expression* expr);

        // Variable
        std::unique_ptr<TypeDeclaration>& checkVariable(Expression* expr);

        // Group
        std::unique_ptr<TypeDeclaration>& checkGroup(Expression* expr);

        // Call
        std::unique_ptr<TypeDeclaration>& checkCall(Expression* expr);

        // Unary
        std::unique_ptr<TypeDeclaration>& checkUnary(Expression* expr);

        // Binary
        std::unique_ptr<TypeDeclaration>& checkBinary(Expression* expr);

        // Ternary
        std::unique_ptr<TypeDeclaration>& checkTernaryIf(Expression* expr);

        // Assignment
        std::unique_ptr<TypeDeclaration>& checkAssignment(Expression* expr);

    private:
        std::shared_ptr<Scope> scope;
};

#endif
