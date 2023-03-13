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

#ifndef PROTO_AST_FUNCTION_DEFINITION_H
#define PROTO_AST_FUNCTION_DEFINITION_H

#include <memory>
#include <vector>
#include <string>

#include "parsetree/declarations/variable.h"
#include "parsetree/declarations/type.h"
#include "parsetree/statements/block.h"
#include "common/token.h"
#include "definition.h"


class FunctionDefinition : public Definition
{
    public:
        FunctionDefinition(
            Token& token
        );

        /**
         * Returns the token associated with this function definition.
         */
        Token& getToken();

        /**
         * Add a parameter to this function.
         */
        void addParameter(std::unique_ptr<VariableDeclaration>&& parameter);

        /**
         * Returns parameters bound to this function.
         */
        std::vector<
            std::unique_ptr<VariableDeclaration>>& getParameters();

        /**
         * Set the return type of this function definition.
         */
        void setReturnType(std::unique_ptr<TypeDeclaration>&& ret_type);

        /**
         * Returns this function return type.
         */
        std::unique_ptr<TypeDeclaration>& getReturnType();

        /**
         * Sets the body of the function.
         */
        void setBody(std::unique_ptr<BlockStatement>&& fun_body);

        /**
         * Returns the body of the function.
         */
        std::unique_ptr<BlockStatement>& getBody();

        /**
         * Returns the mangled name of this function.
         */
        std::string getMangledName();

    protected:
        Token                                       token;          /* Token associated with this function. */
        std::vector<
            std::unique_ptr<VariableDeclaration>>   parameters;     /* Parameters accepted by this function. */
        std::unique_ptr<TypeDeclaration>            return_type;    /* Type of the expression returned by this function*/
        std::unique_ptr<BlockStatement>             body;           /* Body of the function. */
};

#endif
