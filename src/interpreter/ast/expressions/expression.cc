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
            return interpretBool(static_cast<CleanBoolExpression*>(expr));
        }

        case CleanExpressionType::SignedInt: {
            return interpretSignedInt(static_cast<CleanSignedIntExpression*>(expr));
        }

        case CleanExpressionType::UnsignedInt: {
            return interpretUnsignedInt(static_cast<CleanUnsignedIntExpression*>(expr));
        }

        case CleanExpressionType::Float: {
            return interpretFloat(static_cast<CleanFloatExpression*>(expr));
        }

        case CleanExpressionType::String: {
            return interpretString(static_cast<CleanStringExpression*>(expr));
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
