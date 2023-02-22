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

#include <memory>

#include "ast/expressions/expression.h"
#include "ast/declarations/type.h"
#include "inference/literal.h"
#include "utils/inference.h"


/**
 * Infer the type (declaration) of this literal expression
 * and set it on the expression.
 */
std::unique_ptr<TypeDeclaration>&
LiteralInference::infer(std::unique_ptr<LiteralExpression>& expr)
{
    switch (expr->getLiteralType()) {
        case LiteralType::Boolean:
            expr->setTypeDeclaration(createSimpleTypeDeclaration(
                true,
                "bool"
            ));
            return expr->getTypeDeclaration();

        case LiteralType::Integer:
            expr->setTypeDeclaration(createSimpleTypeDeclaration(
                true,
                "uint64"
            ));
            return expr->getTypeDeclaration();

        case LiteralType::Float:
            expr->setTypeDeclaration(createSimpleTypeDeclaration(
                true,
                "float64"
            ));
            return expr->getTypeDeclaration();

        case LiteralType::String:
            expr->setTypeDeclaration(createSimpleTypeDeclaration(
                true,
                "string"
            ));
            return expr->getTypeDeclaration();
    }
}
