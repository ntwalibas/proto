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

#ifndef PROTO_AST_VARIABLE_DECLARATION_H
#define PROTO_AST_VARIABLE_DECLARATION_H

#include <memory>

#include "common/token.h"
#include "declaration.h"
#include "type.h"


class VariableDeclaration : public Declaration
{
    public:
        VariableDeclaration(
            Token& token,
            std::unique_ptr<TypeDeclaration>&& type_decl
        );

        /**
         * Returns the token associated with this variable definition.
         */
        Token& getToken();

        /**
         * Returns the type of this variable definition.
         */
        std::unique_ptr<TypeDeclaration>& getTypeDeclaration();

    protected:
        Token                               token;          /* Token associated with this variable. */
        std::unique_ptr<TypeDeclaration>    type_decl;      /* Variable type. */
};

#endif
