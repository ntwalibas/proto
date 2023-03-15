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

#include <stdexcept>
#include <cstdbool>
#include <cstdint>
#include <utility>
#include <memory>
#include <string>

#include "cleaner/parsetree/expressions/expression.h"
#include "cleaner/parsetree/definitions/variable.h"
#include "cleaner/ast/expressions/expression.h"
#include "parsetree/expressions/expression.h"
#include "parsetree/expressions/assignment.h"
#include "parsetree/expressions/ternaryif.h"
#include "parsetree/expressions/variable.h"
#include "parsetree/definitions/variable.h"
#include "parsetree/expressions/literal.h"
#include "parsetree/expressions/binary.h"
#include "parsetree/expressions/unary.h"
#include "parsetree/expressions/group.h"
#include "parsetree/expressions/call.h"
#include "parsetree/expressions/cast.h"
#include "cleaner/symbols/scope.h"


ExpressionCleaner::ExpressionCleaner(
    std::shared_ptr<CleanScope> const& scope
) : scope(scope)
{}

/**
 * Returns a pruned expression node for the AST given the parse tree equivalent.
 */
std::unique_ptr<CleanExpression>
ExpressionCleaner::clean(
    Expression* expr
)
{
    switch (expr->getType()) {
        case ExpressionType::Literal: {
            LiteralExpression * lit_expr =
                static_cast<LiteralExpression*>(expr);
            return cleanLiteral(lit_expr);
        }

        case ExpressionType::Cast: {
            CastExpression * cast_expr =
                static_cast<CastExpression*>(expr);
            return cleanCast(cast_expr);
        }

        case ExpressionType::Variable: {
            VariableExpression * var_expr =
                static_cast<VariableExpression*>(expr);
            return cleanVariable(var_expr);
        }

        case ExpressionType::Group:{
            GroupExpression * group_expr =
                static_cast<GroupExpression*>(expr);
            return cleanGroup(group_expr);
        }

        case ExpressionType::Call: {
            CallExpression * call_expr =
                static_cast<CallExpression*>(expr);
            return cleanCall(call_expr);
        }

        case ExpressionType::Unary: {
            UnaryExpression * un_expr =
                static_cast<UnaryExpression*>(expr);
            return cleanUnary(un_expr);
        }

        case ExpressionType::Binary: {
            BinaryExpression * bin_expr =
                static_cast<BinaryExpression*>(expr);
            return cleanBinary(bin_expr);
        }

        case ExpressionType::TernaryIf: {
            TernaryIfExpression * ternif_expr =
                static_cast<TernaryIfExpression*>(expr);
            return cleanTernaryIf(ternif_expr);
        }

        case ExpressionType::Assignment: {
            AssignmentExpression * assign_expr =
                static_cast<AssignmentExpression*>(expr);
            return cleanAssignment(assign_expr);
        }

        default:
            std::invalid_argument("Unexpected expression while cleaning.");
    }

    std::invalid_argument("Should never reach here.");
}

// Literals
std::unique_ptr<CleanExpression>
ExpressionCleaner::cleanLiteral(
    LiteralExpression* lit_expr
)
{
    switch (lit_expr->getLiteralType()) {
        case LiteralType::Boolean: {
            return std::make_unique<CleanBoolExpression>(
                lit_expr->getToken().getLexeme() == "true" ? true : false
            );
        }

        case LiteralType::Integer: {
            return std::make_unique<CleanSignedIntExpression>(
                (int64_t) std::stoll(lit_expr->getToken().getLexeme().c_str())
            );
        }

        case LiteralType::Float: {
            return std::make_unique<CleanFloatExpression>(
                (double) std::stod(lit_expr->getToken().getLexeme().c_str())
            );
        }

        case LiteralType::String: {
            return std::make_unique<CleanStringExpression>(
                lit_expr->getToken().getLexeme()
            );
        }
    }
}

// Cast
std::unique_ptr<CleanExpression>
ExpressionCleaner::cleanCast(
    CastExpression* cast_expr
)
{
    std::unique_ptr<CleanCallExpression> clean_call =
        std::make_unique<CleanCallExpression>(cast_expr->getFunctionName());
    
    Expression* expr =
        static_cast<Expression*>(cast_expr->getExpression().get());
    clean_call->arguments.push_back(clean(expr));
    
    return clean_call;
}

// Variable
std::unique_ptr<CleanExpression>
ExpressionCleaner::cleanVariable(
    VariableExpression* var_expr
)
{
    return std::make_unique<CleanVariableExpression>(
        var_expr->getToken().getLexeme()
    );
}

// Group
std::unique_ptr<CleanExpression>
ExpressionCleaner::cleanGroup(
    GroupExpression* group_expr
)
{
    Expression* expr =
        static_cast<Expression*>(group_expr->getExpression().get());
    return std::make_unique<CleanGroupExpression>(
        clean(expr)
    );
}

std::unique_ptr<CleanExpression>
ExpressionCleaner::cleanCall(
    CallExpression* call_expr
)
{
    std::unique_ptr<CleanCallExpression> clean_call =
        std::make_unique<CleanCallExpression>(call_expr->getFunctionName());
    
    for (auto const& argument: call_expr->getArguments()) {
        Expression* arg_expr = static_cast<Expression*>(argument.get());
        clean_call->arguments.push_back(clean(arg_expr));
    }
    
    return clean_call;
}

// Unary
std::unique_ptr<CleanExpression>
ExpressionCleaner::cleanUnary(
    UnaryExpression* un_expr
)
{
    Expression* expr =
        static_cast<Expression*>(un_expr->getExpression().get());
    
    // If we have negation on an integer
    // we reduce it to a signed integer representation
    if (
        un_expr->getUnaryType() == UnaryType::Minus &&
        expr->getType() == ExpressionType::Literal
    ) {
        LiteralExpression* lit_expr =
            static_cast<LiteralExpression*>(expr);
        if (lit_expr->getLiteralType() == LiteralType::Integer)
            return std::make_unique<CleanSignedIntExpression>(
                // TODO: review this to be sure we are conformant
                -((int64_t) std::stoll(lit_expr->getToken().getLexeme().c_str()))
            );
    }

    // In any other case, we perform a function call
    std::unique_ptr<CleanCallExpression> clean_call =
        std::make_unique<CleanCallExpression>(un_expr->getFunctionName());
    clean_call->arguments.push_back(clean(expr));

    return clean_call;
}

std::unique_ptr<CleanExpression>
ExpressionCleaner::cleanBinary(
    BinaryExpression* bin_expr
)
{
    Expression* left_expr =
        static_cast<Expression*>(bin_expr->getLeft().get());
    Expression* right_expr =
        static_cast<Expression*>(bin_expr->getRight().get());

    std::unique_ptr<CleanCallExpression> clean_call =
        std::make_unique<CleanCallExpression>(bin_expr->getFunctionName());

    clean_call->arguments.push_back(clean(left_expr));
    clean_call->arguments.push_back(clean(right_expr));

    return clean_call;
}

// Ternary if
std::unique_ptr<CleanExpression>
ExpressionCleaner::cleanTernaryIf(
    TernaryIfExpression* ternif_expr
)
{
    Expression* cond_expr =
        static_cast<Expression*>(ternif_expr->getCondition().get());
    Expression* lval_expr =
        static_cast<Expression*>(ternif_expr->getLvalue().get());
    Expression* rval_expr =
        static_cast<Expression*>(ternif_expr->getRvalue().get());

    return std::make_unique<CleanTernaryIfExpression>(
        clean(cond_expr),
        clean(lval_expr),
        clean(rval_expr)
    );
}

// Assignment
std::unique_ptr<CleanExpression>
ExpressionCleaner::cleanAssignment(
    AssignmentExpression* assign_expr
)
{
    Expression* lval_expr =
        static_cast<Expression*>(assign_expr->getLvalue().get());
    Expression* rval_expr =
        static_cast<Expression*>(assign_expr->getRvalue().get());
    
    // If we have an in-place assignment, we call the corresponding function
    if (assign_expr->getAssignmentType() != AssignmentType::Simple) {
        std::unique_ptr<CleanCallExpression> clean_call =
            std::make_unique<CleanCallExpression>(assign_expr->getFunctionName());
        clean_call->arguments.push_back(clean(lval_expr));
        clean_call->arguments.push_back(clean(rval_expr));
        return clean_call;
    }

    // If we have a simple assignment,
    // we make sure to take care of a possibly introduced variable definition
    auto& def = assign_expr->getVariableDefinition();
    if (def != nullptr) {
        VariableDefinition* var_def =
            static_cast<VariableDefinition*>(def.get());
        VariableDefinitionCleaner(var_def, scope).clean();
    }

    return std::make_unique<CleanAssignmentExpression>(
        clean(lval_expr),
        clean(rval_expr)
    );
}
