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

#include "checker/ast/expressions/expression.h"
#include "checker/ast/declarations/type.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/assignment.h"
#include "ast/declarations/variable.h"
#include "ast/expressions/ternaryif.h"
#include "inference/inference_error.h"
#include "ast/definitions/variable.h"
#include "ast/expressions/variable.h"
#include "checker/checker_error.h"
#include "ast/declarations/type.h"
#include "ast/expressions/cast.h"
#include "inference/inference.h"
#include "symbols/symtable.h"
#include "symbols/scope.h"


ExpressionChecker::ExpressionChecker(
    Expression* expr,
    std::shared_ptr<Scope> const& scope
) : expr(expr),
    scope(scope)
{}


/**
 * Checks that any given expression obeys the language semantics
 */
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::check()
{
    switch (expr->getType()) {
        case ExpressionType::Literal:
            return checkLiteral();

        case ExpressionType::Cast:
            return checkCast();

        case ExpressionType::Variable:
            return checkVariable();

        case ExpressionType::Group:
            return checkGroup();

        case ExpressionType::Call:
            return checkCall();

        case ExpressionType::Unary:
            return checkUnary();

        case ExpressionType::Binary:
            return checkBinary();

        case ExpressionType::TernaryIf:
            return checkTernaryIf();

        case ExpressionType::Assignment:
            return checkAssignment();

        default:
            throw std::invalid_argument("Given expression cannot currently be checked.");
    }
}

// Literals
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkLiteral()
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
ExpressionChecker::checkCast()
{
    CastExpression* cast_expr = static_cast<CastExpression*>(expr);

    // Make sure that the type destination type is valid
    TypeDeclarationChecker(cast_expr->getTypeDeclaration()).check();

    // Make sure the expression to cast is valid
    std::unique_ptr<TypeDeclaration>& expr_type_decl =
        ExpressionChecker(cast_expr->getExpression().get(), scope).check();
    
    // Make sure there is a function that can perform the cast
    std::string dest_type_name = cast_expr->getTypeDeclaration()->getTypeName();
    std::string source_type_name = expr_type_decl->getTypeName();
    std::string op_name = "__cast@" + dest_type_name + "__(" + source_type_name + ")";
    if (! BuiltinFunctionsSymtable().hasFunctionDefinition(op_name)) {
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
ExpressionChecker::checkVariable()
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
ExpressionChecker::checkGroup()
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

// Call
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkCall()
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

// Unary
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkUnary()
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

// Binary
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkBinary()
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

// TernaryIf
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkTernaryIf()
{
    TernaryIfExpression* ternif_expr = static_cast<TernaryIfExpression*>(expr);

    // Make sure the condition is of type bool
    std::unique_ptr<Expression>& ternif_cond = ternif_expr->getCondition();
    std::unique_ptr<TypeDeclaration>& cond_type_decl =
        ExpressionChecker(ternif_cond.get(), scope).check();
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
        ExpressionChecker(lval.get(), scope).check();
    std::unique_ptr<TypeDeclaration>& rval_type_decl =
        ExpressionChecker(rval.get(), scope).check();
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
ExpressionChecker::checkAssignment()
{
    AssignmentExpression* assign_expr =
        static_cast<AssignmentExpression*>(expr);
    
    // Make sure the LHS is a variable expression
    std::unique_ptr<Expression>& lval = assign_expr->getLvalue();
    if (lval->getType() != ExpressionType::Variable) {
        throw CheckerError(
            lval->getToken(),
            "invalid lvalue to assignment expression",
            "the lvalue of an assignment expression must be a variable",
            false
        );
    }

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
            std::unique_ptr<TypeDeclaration>& rval_type_decl =
                Inference(assign_expr->getRvalue().get(), scope).infer();
            
            // If the LHS exists, make sure it has the same type as the RHS
            if (decl_found || def_found) {
                std::unique_ptr<TypeDeclaration>& lval_type_decl =
                    Inference(lval.get(), scope).infer();
                
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
                std::unique_ptr<Expression>& rval = assign_expr->getRvalue();

                assign_expr->setVariableDefinition(
                    std::make_unique<VariableDefinition>(
                        var_token,
                        copy(rval_type_decl),
                        std::move(rval)
                    )
                ); 
                scope->addDefinition(var_token.getLexeme(), assign_expr->getVariableDefinition());

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
