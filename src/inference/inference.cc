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
#include <iterator>
#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "ast/declarations/declaration.h"
#include "ast/definitions/definition.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/assignment.h"
#include "ast/declarations/variable.h"
#include "inference/inference_error.h"
#include "ast/definitions/variable.h"
#include "ast/definitions/function.h"
#include "ast/expressions/variable.h"
#include "ast/expressions/literal.h"
#include "ast/expressions/binary.h"
#include "ast/expressions/unary.h"
#include "ast/expressions/group.h"
#include "ast/declarations/type.h"
#include "ast/expressions/call.h"
#include "inference/inference.h"
#include "symbols/symtable.h"
#include "utils/inference.h"
#include "symbols/scope.h"


Inference::Inference(
    std::unique_ptr<Expression>& expr,
    std::shared_ptr<Scope> const& scope
) : expr(expr),
    scope(scope)
{}


/**
 * Infer the type (declaration) of this literal expression
 * and set it on the expression.
 */
std::unique_ptr<TypeDeclaration>&
Inference::infer()
{
    switch (expr->getType()) {
        case ExpressionType::Literal:
            return inferLiteralType();

        case ExpressionType::Variable:
            return inferVariableType();
        
        case ExpressionType::Group:
            return inferGroupType();
        
        case ExpressionType::Unary:
            return inferUnaryType();
        
        case ExpressionType::Binary:
            return inferBinaryType();

        default:
            throw std::invalid_argument("Given expression has type for which inference has not been implemented yet.");
    }
}


// Literals
std::unique_ptr<TypeDeclaration>&
Inference::inferLiteralType()
{
    LiteralExpression* lit_expr = static_cast<LiteralExpression*>(expr.get());

    switch (lit_expr->getLiteralType()) {
        case LiteralType::Boolean:
            lit_expr->setTypeDeclaration(createSimpleTypeDeclaration(
                true,
                "bool"
            ));
            return lit_expr->getTypeDeclaration();

        case LiteralType::Integer:
            lit_expr->setTypeDeclaration(createSimpleTypeDeclaration(
                true,
                "uint"
            ));
            return lit_expr->getTypeDeclaration();

        case LiteralType::Float:
            lit_expr->setTypeDeclaration(createSimpleTypeDeclaration(
                true,
                "float"
            ));
            return lit_expr->getTypeDeclaration();

        case LiteralType::String:
            lit_expr->setTypeDeclaration(createSimpleTypeDeclaration(
                true,
                "string"
            ));
            return lit_expr->getTypeDeclaration();
    }
}


// Variables
std::unique_ptr<TypeDeclaration>&
Inference::inferVariableType()
{
    VariableExpression* var_expr = static_cast<VariableExpression*>(expr.get());

    bool decl_found = scope->hasVariableDeclaration(var_expr->getToken().getLexeme(), true);
    bool def_found = scope->hasDefinition(var_expr->getToken().getLexeme(), true);
    if (! decl_found && ! def_found) {
        throw InferenceError(
            var_expr->getToken(),
            "variable used before definition or declaration",
            "expected a variable definition or a function parameter named [" +
            var_expr->getToken().getLexeme() + "]",
            false
        );
    }

    // Function parameters take precedence because they overshadow global variables;
    // But also because we forbid parameters from being redefined inside a function.
    if (decl_found) {
        std::unique_ptr<VariableDeclaration>& decl = scope->getVariableDeclaration(
            var_expr->getToken().getLexeme(),
            true
        );

        expr->setTypeDeclaration(
            copy(decl->getTypeDeclaration())
        );
    }
    else {
        std::unique_ptr<Definition>& def = scope->getDefinition(
            var_expr->getToken().getLexeme(),
            true
        );

        if (def->getType() != DefinitionType::Variable) {
            throw InferenceError(
                var_expr->getToken(),
                "variable used before definition",
                "no variable defined named [" + var_expr->getToken().getLexeme() +
                "] was defined or declared",
                false
            );
        }

        VariableDefinition* var_def = static_cast<VariableDefinition*>(def.get());
        expr->setTypeDeclaration(
            copy(var_def->getTypeDeclaration())
        );
    }
    
    return expr->getTypeDeclaration();
}


// Groups
std::unique_ptr<TypeDeclaration>&
Inference::inferGroupType()
{
    GroupExpression* gr_expr = static_cast<GroupExpression*>(expr.get());
    std::unique_ptr<Expression>& grouped_expr = gr_expr->getExpression();

    std::unique_ptr<TypeDeclaration>& grouped_type_decl =
        Inference(grouped_expr, scope).infer();

    expr->setTypeDeclaration(
        copy(grouped_type_decl)
    );
    return expr->getTypeDeclaration();
}


// Function calls
std::unique_ptr<TypeDeclaration>&
Inference::inferCallType()
{
    CallExpression* call_expr = static_cast<CallExpression*>(expr.get());
    std::vector<std::unique_ptr<Expression>>& args = call_expr->getArguments();

    // Build the call corresponding function's mangled name
    std::string fun_name = call_expr->getToken().getLexeme() + "(";
    for (auto it = args.begin(); it != args.end(); ++it) {
        std::unique_ptr<TypeDeclaration>& type_decl =
            Inference(* it, scope).infer();
        fun_name += type_decl->getTypeName();

        if (next(it) != args.end())
            fun_name += ",";
    }
    fun_name += ")";

    if (! scope->hasDefinition(fun_name, true)) {
        throw InferenceError(
            call_expr->getToken(),
            "no such function",
            "no function with signature [" + fun_name + "] was defined",
            false
        );
    }

    std::unique_ptr<Definition>& def = scope->getDefinition(
        fun_name,
        true
    );

    if (def->getType() != DefinitionType::Function) {
        throw InferenceError(
            call_expr->getToken(),
            "function called but not defined",
            "no function with name [" + call_expr->getToken().getLexeme() + "] was defined",
            false
        );
    }

    FunctionDefinition* fun_def = static_cast<FunctionDefinition*>(def.get());
    expr->setTypeDeclaration(
        copy(fun_def->getReturnType())
    );

    return expr->getTypeDeclaration();
}


// Unary operators
std::unique_ptr<TypeDeclaration>&
Inference::inferUnaryType()
{
    UnaryExpression* un_expr = static_cast<UnaryExpression*>(expr.get());
    std::unique_ptr<TypeDeclaration>& expr_type_decl =
        Inference(un_expr->getExpression(), scope).infer();
    
    std::string expr_type_name =  expr_type_decl->getTypeName();

    std::string op_name;
    switch (un_expr->getUnaryType()) {
        case UnaryType::Plus:
            op_name = "__pos__(" + expr_type_name + ")";
            break;
        
        case UnaryType::Minus:
            op_name = "__neg__(" + expr_type_name + ")";
            break;
        
        case UnaryType::BitwiseNot:
            op_name = "__bnot__(" + expr_type_name + ")";
            break;
        
        case UnaryType::LogicalNot:
            op_name = "__not__(" + expr_type_name + ")";
            break;
    }

    try {
        expr->setTypeDeclaration(
            copy(BuiltinFunctionsSymtable().getReturnType(op_name))
        );
    } catch (std::out_of_range const& e) {
        throw InferenceError(
            un_expr->getToken(),
            "invalid argument to [" + un_expr->getToken().getLexeme() + "] unary operator",
            "the [" + un_expr->getToken().getLexeme() + "] operator does " +
            "not accept an operand of type [" + expr_type_name + "].",
            false
        );
    }

    return expr->getTypeDeclaration();
}


// Binary operators
std::unique_ptr<TypeDeclaration>&
Inference::inferBinaryType()
{
    BinaryExpression* bin_expr = static_cast<BinaryExpression*>(expr.get());
    std::unique_ptr<TypeDeclaration>& left_expr_type =
        Inference(bin_expr->getLeft(), scope).infer();
    std::unique_ptr<TypeDeclaration>& right_expr_type =
        Inference(bin_expr->getRight(), scope).infer();
    
    std::string left_type_name = left_expr_type->getTypeName();
    std::string right_type_name = right_expr_type->getTypeName();

    std::string op_name;
    switch (bin_expr->getBinaryType()) {
        // Terms
        case BinaryType::Plus:
            op_name = "__add__(" + left_type_name + "," + right_type_name + ")";
            break;

        case BinaryType::Minus:
            op_name = "__sub__(" + left_type_name + "," + right_type_name + ")";
            break;

        // Factors
        case BinaryType::Mul:
            op_name = "__mul__(" + left_type_name + "," + right_type_name + ")";
            break;

        case BinaryType::Div:
            op_name = "__div__(" + left_type_name + "," + right_type_name + ")";
            break;

        case BinaryType::Rem:
            op_name = "__rem__(" + left_type_name + "," + right_type_name + ")";
            break;

        case BinaryType::Pow:
            op_name = "__pow__(" + left_type_name + "," + right_type_name + ")";
            break;

        // Bit
        case BinaryType::BitwiseAnd:
            op_name = "__band__(" + left_type_name + "," + right_type_name + ")";
            break;

        case BinaryType::BitwiseOr:
            op_name = "__bor__(" + left_type_name + "," + right_type_name + ")";
            break;

        case BinaryType::BitwiseXor:
            op_name = "__xor__(" + left_type_name + "," + right_type_name + ")";
            break;

        case BinaryType::LeftShift:
            op_name = "__lshift__(" + left_type_name + "," + right_type_name + ")";
            break;

        case BinaryType::RightShift:
            op_name = "__rshift__(" + left_type_name + "," + right_type_name + ")";
            break;

        // Logical
        case BinaryType::LogicalAnd:
            op_name = "__and__(" + left_type_name + "," + right_type_name + ")";
            break;

        case BinaryType::LogicalOr:
            op_name = "__or__(" + left_type_name + "," + right_type_name + ")";
            break;
        
        // Comparison
        case BinaryType::Equal:
            op_name = "__eq__(" + left_type_name + "," + right_type_name + ")";
            break;
        case BinaryType::NotEqual:
            op_name = "__ne__(" + left_type_name + "," + right_type_name + ")";
            break;
        case BinaryType::Greater:
            op_name = "__gt__(" + left_type_name + "," + right_type_name + ")";
            break;
        case BinaryType::GreaterOrEqual:
            op_name = "__ge__(" + left_type_name + "," + right_type_name + ")";
            break;
        case BinaryType::Less:
            op_name = "__lt__(" + left_type_name + "," + right_type_name + ")";
            break;
        case BinaryType::LessOrEqual:
            op_name = "__le__(" + left_type_name + "," + right_type_name + ")";
            break;

        default:
            throw std::invalid_argument("Unexpected binary operator, canno infer.");
    }

    try {
        expr->setTypeDeclaration(
            copy(BuiltinFunctionsSymtable().getReturnType(op_name))
        );
    } catch (std::out_of_range const& e) {
        throw InferenceError(
            bin_expr->getToken(),
            "invalid argument to [" + bin_expr->getToken().getLexeme() + "] binary operator",
            "the [" + bin_expr->getToken().getLexeme() + "] operator " +
            "does not accept operands of types [" +
            left_type_name + "] and [" +
            right_type_name + "].",
            false
        );
    }

    return expr->getTypeDeclaration();
}


// Assignment operator
std::unique_ptr<TypeDeclaration>&
Inference::inferAssignmentType()
{
    AssignmentExpression* assign_expr =
        static_cast<AssignmentExpression*>(expr.get());
    std::unique_ptr<Expression>& lval = assign_expr->getLvalue();

    expr->setTypeDeclaration(copy(Inference(lval, scope).infer()));
    return expr->getTypeDeclaration();
}