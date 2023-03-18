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

#include "interpreter/ast/expressions/expression.h"
#include "interpreter/ast/definitions/function.h"
#include "interpreter/ast/definitions/variable.h"
#include "cleaner/ast/expressions/expression.h"
#include "cleaner/symbols/scope.h"


ExpressionInterpreter::ExpressionInterpreter(
    CleanScope* scope
) : scope(scope)
{}

/**
 * Interprets the given expression.
 */
std::unique_ptr<CleanExpression>
ExpressionInterpreter::interpret(CleanExpression* expr)
{
    switch (expr->type) {
        case CleanExpressionType::Boolean: {
            return interpretBool(
                static_cast<CleanBoolExpression*>(expr)
            );
        }

        case CleanExpressionType::SignedInt: {
            return interpretSignedInt(
                static_cast<CleanSignedIntExpression*>(expr)
            );
        }

        case CleanExpressionType::UnsignedInt: {
            return interpretUnsignedInt(
                static_cast<CleanUnsignedIntExpression*>(expr)
            );
        }

        case CleanExpressionType::Float: {
            return interpretFloat(
                static_cast<CleanFloatExpression*>(expr)
            );
        }

        case CleanExpressionType::String: {
            return interpretString(
                static_cast<CleanStringExpression*>(expr)
            );
        }

        case CleanExpressionType::Group: {
            return interpretGroup(
                static_cast<CleanGroupExpression*>(expr)
            );
        }

        case CleanExpressionType::Variable: {
            return interpretVariable(
                static_cast<CleanVariableExpression*>(expr)
            );
        }

        case CleanExpressionType::Call: {
            return interpretCall(
                static_cast<CleanCallExpression*>(expr)
            );
        }

        case CleanExpressionType::TernaryIf: {
            return interpretTernaryIf(
                static_cast<CleanTernaryIfExpression*>(expr)
            );
        }

        case CleanExpressionType::Assignment: {
            return interpretAssignment(
                static_cast<CleanAssignmentExpression*>(expr)
            );
        }

        case CleanExpressionType::Intrinsic: {
            return interpretIntrinsic(
                static_cast<CleanIntrinsicExpression*>(expr),
                scope
            );
        }

        default:
            throw std::runtime_error(
                "Expression iterpretation failed: unknow expression type."
            );
    }

    throw std::runtime_error(
        "Expression iterpretation failed: expression switch failed."
    );
}

// Bool
std::unique_ptr<CleanBoolExpression>
ExpressionInterpreter::interpretBool(
    CleanBoolExpression* bool_expr
)
{
    return std::make_unique<CleanBoolExpression>(bool_expr);
}

// Signed int
std::unique_ptr<CleanSignedIntExpression>
ExpressionInterpreter::interpretSignedInt(
    CleanSignedIntExpression* int_expr
)
{
    return std::make_unique<CleanSignedIntExpression>(int_expr);
}

// Unsigned int
std::unique_ptr<CleanUnsignedIntExpression>
ExpressionInterpreter::interpretUnsignedInt(
    CleanUnsignedIntExpression* uint_expr
)
{
    return std::make_unique<CleanUnsignedIntExpression>(uint_expr);
}

// Float
std::unique_ptr<CleanFloatExpression>
ExpressionInterpreter::interpretFloat(
    CleanFloatExpression* float_expr
)
{
    return std::make_unique<CleanFloatExpression>(float_expr);
}

// String
std::unique_ptr<CleanStringExpression>
ExpressionInterpreter::interpretString(
    CleanStringExpression* string_expr
)
{
    return std::make_unique<CleanStringExpression>(string_expr);
}

// Variable
std::unique_ptr<CleanExpression>
ExpressionInterpreter::interpretVariable(
    CleanVariableExpression* var_expr
)
{
    std::unique_ptr<CleanVariableDefinition>& var_def =
        scope->getSymbol<CleanVariableDefinition>(var_expr->var_name, true);
    
    return VariableDefinitionInterpreter().interpret(
        var_def.get(),
        scope
    );
}

// Group
std::unique_ptr<CleanExpression>
ExpressionInterpreter::interpretGroup(
    CleanGroupExpression* gr_expr
)
{
    return interpret(gr_expr->expression.get());
}

// Call
std::unique_ptr<CleanExpression>
ExpressionInterpreter::interpretCall(
    CleanCallExpression* call_expr
)
{
    // Construct new arguments from the call expression
    // These should essentially amount to literals only
    std::vector<std::unique_ptr<CleanExpression>> arguments;
    for (auto& argument: call_expr->arguments)
        arguments.push_back(interpret(argument.get()));

    // Find the function in the scope and interpret it based on new arguments
    std::unique_ptr<CleanFunctionDefinition>& fun_def =
        scope->getSymbol<CleanFunctionDefinition>(call_expr->fun_name, true);
    
    return FunctionDefinitionInterpreter().interpret(
        fun_def.get(),
        arguments
    );
}

// Ternary if
std::unique_ptr<CleanExpression>
ExpressionInterpreter::interpretTernaryIf(
    CleanTernaryIfExpression* ternif_expr
)
{
    std::unique_ptr<CleanExpression> eval_condition =
        interpret(ternif_expr->condition.get());
    CleanBoolExpression* bool_condition =
        static_cast<CleanBoolExpression*>(eval_condition.get());
    if (bool_condition->value == true)
        return interpret(ternif_expr->then_branch.get());
    else
        return interpret(ternif_expr->else_branch.get());
}

// Assignment
std::unique_ptr<CleanExpression>
ExpressionInterpreter::interpretAssignment(
    CleanAssignmentExpression* assign_expr
)
{
    CleanVariableExpression* lvalue_expr =
        static_cast<CleanVariableExpression*>(assign_expr->lvalue.get());

    // Pull the variable definition to update
    std::unique_ptr<CleanVariableDefinition>& var_def =
        scope->getSymbol<CleanVariableDefinition>(lvalue_expr->var_name, true);

    // If the rvalue is the same variable on the lvalue,
    // we just return the content of the corresponding variable definition
    bool interpret_rvalue = true;
    if (assign_expr->rvalue->type == CleanExpressionType::Variable) {
        CleanVariableExpression* rvalue_expr =
            static_cast<CleanVariableExpression*>(assign_expr->rvalue.get());
        if (lvalue_expr->var_name == rvalue_expr->var_name)
            interpret_rvalue = false;
    }

    // Update the variable definition initializer
    if (interpret_rvalue)
        var_def->initializer = interpret(assign_expr->rvalue.get());
    
    return VariableDefinitionInterpreter().interpret(
        var_def.get(),
        scope
    );
}

// Intrinsic
std::unique_ptr<CleanExpression>
ExpressionInterpreter::interpretIntrinsic(
    CleanIntrinsicExpression* intr_expr,
    CleanScope* scope
)
{
    return intr_expr->callable(scope);
}
