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
#include <utility>
#include <memory>
#include <string>

#include "checker/parsetree/expressions/expression.h"
#include "checker/parsetree/declarations/type.h"
#include "parsetree/expressions/expression.h"
#include "parsetree/expressions/assignment.h"
#include "parsetree/declarations/variable.h"
#include "parsetree/expressions/ternaryif.h"
#include "parsetree/definitions/variable.h"
#include "parsetree/expressions/variable.h"
#include "parsetree/expressions/binary.h"
#include "parsetree/expressions/unary.h"
#include "parsetree/expressions/group.h"
#include "parsetree/declarations/type.h"
#include "parsetree/expressions/cast.h"
#include "parsetree/expressions/call.h"
#include "inference/inference_error.h"
#include "checker/checker_error.h"
#include "inference/inference.h"
#include "intrinsics/reslib.h"
#include "symbols/scope.h"


ExpressionChecker::ExpressionChecker(
    std::shared_ptr<Scope> const& scope
) : scope(scope)
{}


/**
 * Checks that any given expression obeys the language semantics
 */
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::check(Expression* expr)
{
    switch (expr->getType()) {
        case ExpressionType::Literal:
            return checkLiteral(expr);

        case ExpressionType::Cast:
            return checkCast(expr);

        case ExpressionType::Variable:
            return checkVariable(expr);

        case ExpressionType::Group:
            return checkGroup(expr);

        case ExpressionType::Call:
            return checkCall(expr);

        case ExpressionType::Unary:
            return checkUnary(expr);

        case ExpressionType::Binary:
            return checkBinary(expr);

        case ExpressionType::TernaryIf:
            return checkTernaryIf(expr);

        case ExpressionType::Assignment:
            return checkAssignment(expr);

        default:
            throw std::invalid_argument("Given expression cannot currently be checked.");
    }
}

// Literals
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkLiteral(Expression* expr)
{
    try {
        return Inference(expr, scope).infer();
    } catch(InferenceError& e) {
        throw CheckerError(
            e.getToken(),
            e.getPrimaryMessage(),
            e.getSecondaryMessage(),
            e.isFatal()
        );
    }
}

// Cast
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkCast(Expression* expr)
{
    CastExpression* cast_expr = static_cast<CastExpression*>(expr);

    // Make sure that the type destination type is valid
    TypeDeclarationChecker(cast_expr->getTypeDeclaration()).check();

    // Make sure the expression to cast is valid
    std::unique_ptr<TypeDeclaration>& expr_type_decl =
        check(cast_expr->getExpression().get());
    
    // Make sure there is a function that can perform the cast
    std::string dest_type_name = cast_expr->getTypeDeclaration()->getTypeName();
    std::string source_type_name = expr_type_decl->getTypeName();
    std::string op_name = "__cast@" + dest_type_name + "__(" + source_type_name + ")";
    if (! ReslibFunctionsSymtable().hasFunctionDefinition(op_name)) {
        throw CheckerError(
            cast_expr->getToken(),
            "invalid cast",
            "expression of type `" + source_type_name + "` "
            "cannot be cast to the type `" + dest_type_name +"`",
            false
        );
    }
    
    try {
        return Inference(expr, scope).infer();
    } catch(InferenceError& e) {
        throw CheckerError(
            e.getToken(),
            e.getPrimaryMessage(),
            e.getSecondaryMessage(),
            e.isFatal()
        );
    }
}

// Variable
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkVariable(Expression* expr)
{
    try {
        return Inference(expr, scope).infer();
    } catch(InferenceError& e) {
        throw CheckerError(
            e.getToken(),
            e.getPrimaryMessage(),
            e.getSecondaryMessage(),
            e.isFatal()
        );
    }
}

// Group
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkGroup(Expression* expr)
{
    GroupExpression* gr_expr =
        static_cast<GroupExpression*>(expr);

    check(gr_expr->getExpression().get());
    try {
        return Inference(expr, scope).infer();
    } catch(InferenceError& e) {
        throw CheckerError(
            e.getToken(),
            e.getPrimaryMessage(),
            e.getSecondaryMessage(),
            e.isFatal()
        );
    }
}

// Call
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkCall(Expression* expr)
{
    CallExpression* call_expr =
        static_cast<CallExpression*>(expr);
    for (auto& argument: call_expr->getArguments())
        check(argument.get());

    try {
        return Inference(expr, scope).infer();
    } catch(InferenceError& e) {
        throw CheckerError(
            e.getToken(),
            e.getPrimaryMessage(),
            e.getSecondaryMessage(),
            e.isFatal()
        );
    }
}

// Unary
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkUnary(Expression* expr)
{
    UnaryExpression* un_expr =
        static_cast<UnaryExpression*>(expr);
    check(un_expr->getExpression().get());

    try {
        return Inference(expr, scope).infer();
    } catch(InferenceError& e) {
        throw CheckerError(
            e.getToken(),
            e.getPrimaryMessage(),
            e.getSecondaryMessage(),
            e.isFatal()
        );
    }
}

// Binary
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkBinary(Expression* expr)
{
    BinaryExpression* bin_expr =
        static_cast<BinaryExpression*>(expr);
    check(bin_expr->getLeft().get());
    check(bin_expr->getRight().get());

    try {
        return Inference(expr, scope).infer();
    } catch(InferenceError& e) {
        throw CheckerError(
            e.getToken(),
            e.getPrimaryMessage(),
            e.getSecondaryMessage(),
            e.isFatal()
        );
    }
}

// TernaryIf
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkTernaryIf(Expression* expr)
{
    TernaryIfExpression* ternif_expr =
        static_cast<TernaryIfExpression*>(expr);

    // Make sure the condition is of type bool
    std::unique_ptr<Expression>& ternif_cond = ternif_expr->getCondition();
    std::unique_ptr<TypeDeclaration>& cond_type_decl =
        check(ternif_cond.get());
    if (cond_type_decl->getTypeName() != "bool") {
        throw CheckerError(
            ternif_cond->getToken(),
            "invalid condition to ternary if",
            "the condition expression to ternary if must be of type bool",
            false
        );
    }

    // Make sure the alternative branches have the same type
    std::unique_ptr<Expression>& lval = ternif_expr->getLvalue();
    std::unique_ptr<Expression>& rval = ternif_expr->getRvalue();
    std::unique_ptr<TypeDeclaration>& lval_type_decl =
        check(lval.get());
    std::unique_ptr<TypeDeclaration>& rval_type_decl =
        check(rval.get());
    if (! typeDeclarationEquals(lval_type_decl, rval_type_decl)) {
        throw CheckerError(
            ternif_expr->getToken(),
            "ternary if branches type mismatch",
            "the lvalue to ternary if has type `" + lval_type_decl->getTypeName() +
            "` while the rvalue has type `" + rval_type_decl->getTypeName() + "`",
            false
        );
    }

    try {
        return Inference(expr, scope).infer();
    } catch(InferenceError& e) {
        throw CheckerError(
            e.getToken(),
            e.getPrimaryMessage(),
            e.getSecondaryMessage(),
            e.isFatal()
        );
    }
}

// Assignment
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkAssignment(Expression* expr)
{
    AssignmentExpression* assign_expr =
        static_cast<AssignmentExpression*>(expr);
    std::unique_ptr<Expression>& lval = assign_expr->getLvalue();
    std::unique_ptr<Expression>& rval = assign_expr->getRvalue();

    // Make sure the LHS is a variable expression
    if (lval->getType() != ExpressionType::Variable) {
        throw CheckerError(
            lval->getToken(),
            "invalid lvalue to assignment expression",
            "the lvalue of an assignment expression must be a variable",
            false
        );
    }

    // Check the lval and rval
    std::unique_ptr<TypeDeclaration>& rval_type_decl =
        check(rval.get());

    // Make sure that if the LHS exists, it is not const
    VariableExpression* var_expr = static_cast<VariableExpression*>(lval.get());
    bool decl_found = scope->hasVariableDeclaration(var_expr->getToken().getLexeme(), true);
    bool def_found = scope->hasDefinition(var_expr->getToken().getLexeme(), true);

    if (decl_found || def_found) {
        if (decl_found) {
            std::unique_ptr<VariableDeclaration>& decl = scope->getVariableDeclaration(
                var_expr->getToken().getLexeme(),
                true
            );

            if (decl->getTypeDeclaration()->isConst()) {
                throw CheckerError(
                    var_expr->getToken(),
                    "assignment to const variable",
                    std::string("the variable declaration bound to this expression is ") +
                    "declared const and connot be changed",
                    false
                );
            }
        }
        else {
            std::unique_ptr<Definition>& def = scope->getDefinition(
                var_expr->getToken().getLexeme(),
                true
            );
            
            VariableDefinition* var_def = static_cast<VariableDefinition*>(def.get());
            if (var_def->getTypeDeclaration()->isConst()) {
                throw CheckerError(
                    var_expr->getToken(),
                    "assignment to const variable",
                    std::string("the variable definition bound to this expression is ") +
                    "declared const and connot be changed",
                    false
                );
            }
        }
    }

    // In-place assignment: they decay to function calls
    if (assign_expr->getAssignmentType() != AssignmentType::Simple) {
        // Require that the LHS exists in case of in-place assignment
        if (!decl_found && !def_found) {
            throw CheckerError(
                var_expr->getToken(),
                "variable used before definition or declaration",
                "expected a variable definition or a function parameter named `" +
                var_expr->getToken().getLexeme() + "`",
                false
            );
        }
    }
    // Simple assignment: if LHS doesn't exist, create it
    else {
        try {
            // If the LHS exists, make sure it has the same type as the RHS
            if (decl_found || def_found) {
                std::unique_ptr<TypeDeclaration>& lval_type_decl =
                    check(lval.get());
                if (! typeDeclarationEquals(rval_type_decl, lval_type_decl)) {
                    throw CheckerError(
                        assign_expr->getToken(),
                        "assignment expressions type mismatch",
                        "the lvalue to the assignment has type `" + lval_type_decl->getTypeName() +
                        "` while the rvalue has type `" + rval_type_decl->getTypeName() + "`",
                        false
                    );
                }
            }
            // In case the LHS doesn't exist, we introduce a new definition in its name
            else {
                Token var_token = lval->getToken();
                assign_expr->setVariableDefinition(
                    std::make_unique<VariableDefinition>(
                        var_token,
                        copy(rval_type_decl),
                        std::move(rval)
                    )
                ); 
                scope->addDefinition(
                    var_token.getLexeme(),
                    assign_expr->getVariableDefinition()
                );

                // Reset the assignment rvalue to point to itself
                assign_expr->setRvalue(
                    std::make_unique<VariableExpression>(var_token)
                );
            }
        } catch(InferenceError& e) {
            throw CheckerError(
                e.getToken(),
                e.getPrimaryMessage(),
                e.getSecondaryMessage(),
                e.isFatal()
            );
        }
    }

    try {
        return Inference(expr, scope).infer();
    } catch(InferenceError& e) {
        throw CheckerError(
            e.getToken(),
            e.getPrimaryMessage(),
            e.getSecondaryMessage(),
            e.isFatal()
        );
    }
}
