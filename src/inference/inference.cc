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

#include "ast/expressions/expression.h"
#include "ast/expressions/literal.h"
#include "ast/expressions/array.h"
#include "ast/declarations/type.h"
#include "inference/inference.h"
#include "utils/inference.h"

Inference::Inference(
    std::unique_ptr<Expression>& expr
) : expr(expr)
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

        case ExpressionType::Array:
            return inferArrayType();

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
                "uint64"
            ));
            return lit_expr->getTypeDeclaration();

        case LiteralType::Float:
            lit_expr->setTypeDeclaration(createSimpleTypeDeclaration(
                true,
                "float64"
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


// Arrays
std::unique_ptr<TypeDeclaration>&
Inference::inferArrayType()
{
    ArrayExpression* arr_expr = static_cast<ArrayExpression*>(expr.get());

    std::vector<std::unique_ptr<Expression>>& contents =
        arr_expr->getContents();
    
    if (contents.size() == 0)
        throw std::length_error("An array expression cannot be empty.");
    
    auto curr_it = contents.begin();
    auto next_it = std::next(curr_it);
    for (; next_it != contents.end(); ++curr_it, next_it = std::next(curr_it)) {
        std::unique_ptr<TypeDeclaration>& curr_type_decl =
            Inference(*curr_it).infer();

        std::unique_ptr<TypeDeclaration>& next_type_decl =
            Inference(*next_it).infer();
        
        if (! typeDeclarationEquals(curr_type_decl, next_type_decl))
            throw std::invalid_argument("Array elements do not have the same types.");
        
        // Pre-emptively set the type of the array
        if (curr_it == contents.begin()) {
            if (curr_type_decl->getTypeCategory() == TypeCategory::Array)
                throw std::domain_error("Arrays within arrays are not currently supported");

            expr->setTypeDeclaration(createArrayTypeDeclaration(
                true,
                contents.size(),
                curr_type_decl->getTypeName()
            ));
        }
    }

    return expr->getTypeDeclaration();
}
