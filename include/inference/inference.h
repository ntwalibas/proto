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

#ifndef PROTO_AST_INFERENCE_H
#define PROTO_AST_INFERENCE_H

#include <memory>

#include "ast/expressions/expression.h"
#include "ast/declarations/type.h"
#include "symbols/scope.h"


class Inference
{
    public:
        Inference(
            std::unique_ptr<Expression>& expr,
            std::shared_ptr<Scope> const& scope
        );

        /**
         * Infer the type (declaration) of an arbitrary expression.
         */
        std::unique_ptr<TypeDeclaration>& infer();

        // Literals
        std::unique_ptr<TypeDeclaration>& inferLiteralType();

        // Casts
        std::unique_ptr<TypeDeclaration>& inferCastType();

        // Variables
        std::unique_ptr<TypeDeclaration>& inferVariableType();

        // Groups
        std::unique_ptr<TypeDeclaration>& inferGroupType();

        // Function calls
        std::unique_ptr<TypeDeclaration>& inferCallType();

        // Unary operators
        std::unique_ptr<TypeDeclaration>& inferUnaryType();

        // Binary operators
        std::unique_ptr<TypeDeclaration>& inferBinaryType();

        // Ternary if
        std::unique_ptr<TypeDeclaration>& inferTernaryIfType();

        // Assignment operators
        std::unique_ptr<TypeDeclaration>& inferAssignmentType();
    
    private:
        std::unique_ptr<Expression>&    expr;   /* Expression which type to infer. */
        std::shared_ptr<Scope>          scope;  /* Scope within which this expression occurs. */
};

#endif
