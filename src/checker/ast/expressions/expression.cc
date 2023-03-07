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

#include <exception>
#include <memory>

#include "checker/ast/expressions/expression.h"
#include "checker/ast/declarations/type.h"
#include "ast/expressions/expression.h"
#include "checker/checker_error.h"
#include "ast/declarations/type.h"
#include "ast/expressions/cast.h"
#include "inference/inference.h"
#include "symbols/symtable.h"
#include "symbols/scope.h"


ExpressionChecker::ExpressionChecker(
    std::unique_ptr<Expression>& expr,
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

        // case ExpressionType::Variable:
        //     return checkVariable();

        // case ExpressionType::Group:
        //     return checkGroup();

        // case ExpressionType::Call:
        //     return checkCall();

        // case ExpressionType::Unary:
        //     return checkUnary();

        // case ExpressionType::Binary:
        //     return checkBinary();

        // case ExpressionType::TernaryIf:
        //     return checkTernaryIf();

        // case ExpressionType::Assignment:
        //     return checkAssignment();

        default:
            throw std::invalid_argument("Given expression cannot currently be checked.");
    }
}

// Literals
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkLiteral()
{
    return Inference(expr, scope).infer();
}

// Cast
std::unique_ptr<TypeDeclaration>&
ExpressionChecker::checkCast()
{
    CastExpression* cast_expr = static_cast<CastExpression*>(expr.get());

    // Make sure that the type destination type is valid
    TypeDeclarationChecker(cast_expr->getTypeDeclaration()).check();

    // Make sure the expression to cast is valid
    std::unique_ptr<TypeDeclaration>& expr_type_decl =
        ExpressionChecker(cast_expr->getExpression(), scope).check();
    
    // Make sure there is a function that can perform the cast
    std::string dest_type_name = cast_expr->getTypeDeclaration()->getTypeName();
    std::string source_type_name = expr_type_decl->getTypeName();
    std::string op_name = "__cast@" + dest_type_name + "__(" + source_type_name + ")";
    if (! BuiltinFunctionsSymtable().hasFunctionDefinition(op_name)) {
        throw CheckerError(
            cast_expr->getToken(),
            "invalid cast",
            "expression of type [" + source_type_name + "] "
            "cannot be cast to the type [" + dest_type_name +"]",
            false
        );
    }
    
    return Inference(expr, scope).infer();
}
