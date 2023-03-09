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

#ifndef PROTO_FUNCTION_DEFINITION_CHECKER_H
#define PROTO_FUNCTION_DEFINITION_CHECKER_H

#include <memory>

#include "ast/definitions/definition.h"
#include "ast/definitions/function.h"
#include "symbols/scope.h"


class FunctionDefinitionChecker
{
    public:
        FunctionDefinitionChecker(
            FunctionDefinition* function_def,
            std::shared_ptr<Scope> const& scope
        );

        /**
         * Checks that:
         * 1. All parameters are valid variable declarations, and the return type is correct.
         * 2. The function body has valid statements.
         */
        void check(std::unique_ptr<Definition>& definition);

    private:
        FunctionDefinition* function_def;
        std::shared_ptr<Scope> const& scope;

        // Check that all parameters are valid variable declarations.
        // Check that the return type is valid.
        void checkHeader(std::shared_ptr<Scope>& fun_scope);

        // Check that the body contains valid statements.
        void checkBody(std::shared_ptr<Scope>& fun_scope);
};

#endif
