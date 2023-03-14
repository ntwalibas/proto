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

#ifndef PROTO_EXPRESSION_CLEANER_H
#define PROTO_EXPRESSION_CLEANER_H

#include <memory>

#include "cleaner/ast/expressions/expression.h"
#include "parsetree/expressions/expression.h"
#include "parsetree/expressions/assignment.h"
#include "parsetree/expressions/ternaryif.h"
#include "parsetree/expressions/variable.h"
#include "cleaner/symbols/scope.forward.h"
#include "parsetree/expressions/literal.h"
#include "parsetree/expressions/binary.h"
#include "parsetree/expressions/unary.h"
#include "parsetree/expressions/group.h"
#include "parsetree/expressions/call.h"
#include "parsetree/expressions/cast.h"


class ExpressionCleaner
{
    public:
        ExpressionCleaner(
            std::shared_ptr<CleanScope> const& scope
        );
        
        /**
         * Returns a pruned expression node for the AST given the parse tree equivalent.
         */
        std::unique_ptr<CleanExpression> clean(Expression* expr);

        // Literals
        std::unique_ptr<CleanExpression> cleanLiteral(LiteralExpression* lit_expr);

        // Cast
        std::unique_ptr<CleanExpression> cleanCast(CastExpression* cast_expr);

        // Variable
        std::unique_ptr<CleanExpression> cleanVariable(VariableExpression* var_expr);

        // Group
        std::unique_ptr<CleanExpression> cleanGroup(GroupExpression* group_expr);

        // Call
        std::unique_ptr<CleanExpression> cleanCall(CallExpression* call_expr);

        // Unary
        std::unique_ptr<CleanExpression> cleanUnary(UnaryExpression* un_expr);

        // Binary
        std::unique_ptr<CleanExpression> cleanBinary(BinaryExpression* bin_expr);

        // Ternary
        std::unique_ptr<CleanExpression> cleanTernaryIf(TernaryIfExpression* ternif_expr);

        // Assignment
        std::unique_ptr<CleanExpression> cleanAssignment(AssignmentExpression* assign_expr);

    private:
        std::shared_ptr<CleanScope> const& scope;
};

#endif
