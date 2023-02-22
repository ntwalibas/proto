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
#include <utility>
#include <memory>

#include <iostream>

#include "ast/expressions/expression.h"
#include "ast/expressions/literal.h"
#include "ast/declarations/type.h"
#include "inference/inference.h"
#include "inference/literal.h"


/**
 * Infer the type (declaration) of this literal expression
 * and set it on the expression.
 */
std::unique_ptr<TypeDeclaration>&
Inference::infer(std::unique_ptr<Expression>& expr)
{
    switch (expr->getType()) {
        case ExpressionType::Literal: {
            LiteralExpression* lit_expr = static_cast<LiteralExpression*>(expr.get());
            std::unique_ptr<TypeDeclaration>& type_decl = LiteralInference::infer(lit_expr);
            return type_decl;
        }

        default:
            throw std::invalid_argument("Given expression has type for which inference has not been implemented yet.");
    }
}
