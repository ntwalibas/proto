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

#include "ast/definitions/definition.h"
#include "ast/expressions/expression.h"
#include "ast/definitions/variable.h"
#include "ast/expressions/variable.h"
#include "ast/expressions/literal.h"
#include "ast/expressions/array.h"
#include "ast/expressions/group.h"
#include "ast/declarations/type.h"
#include "inference/inference.h"
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

        case ExpressionType::Array:
            return inferArrayType();

        case ExpressionType::Variable:
            return inferVariableType();
        
        case ExpressionType::Group:
            return inferGroupType();

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

    std::unique_ptr<TypeDeclaration>& first_element_type_decl =
            Inference(contents[0], scope).infer();
    
    // Pre-emptively set the type of the array
    expr->setTypeDeclaration(createArrayTypeDeclaration(
        true,
        contents.size(),
        first_element_type_decl->getTypeName()
    ));

    for (auto it = contents.begin(); it != contents.end(); ++it) {
        if ((*it)->getType() == ExpressionType::Array)
            throw std::domain_error("Arrays within arrays are not currently supported");

        std::unique_ptr<TypeDeclaration>& it_type_decl =
            Inference(*it, scope).infer();
        
        if (! typeDeclarationEquals(first_element_type_decl, it_type_decl))
            throw std::invalid_argument("Array elements do not have the same types.");
    }

    return expr->getTypeDeclaration();
}


// Variables
std::unique_ptr<TypeDeclaration>&
Inference::inferVariableType()
{
    VariableExpression* var_expr = static_cast<VariableExpression*>(expr.get());
    
    std::unique_ptr<Definition>& def = scope->getDefinition(var_expr->getToken().getLexeme());

    if (def->getType() != DefinitionType::Variable)
        throw std::invalid_argument("There is no variable with the given name");

    VariableDefinition* var_def = static_cast<VariableDefinition*>(def.get());
    expr->setTypeDeclaration(
        copy(var_def->getTypeDeclaration())
    );
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
