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

#ifndef PROTO_FUNCTION_DEFINITION_CLEANER_H
#define PROTO_FUNCTION_DEFINITION_CLEANER_H

#include <memory>

#include "cleaner/ast/definitions/function.h"
#include "parsetree/definitions/definition.h"
#include "parsetree/definitions/function.h"
#include "cleaner/symbols/scope.forward.h"


class FunctionDefinitionCleaner
{
    public:
        FunctionDefinitionCleaner(
            FunctionDefinition* function_def,
            std::shared_ptr<CleanScope> const& scope
        );

        /**
         * Produces a function AST for its parsetree
         */
        void clean();

    private:
        FunctionDefinition* function_def;
        std::shared_ptr<CleanScope> const& scope;

        // Clean the function's parameters and return type
        void cleanHeader(std::unique_ptr<CleanFunctionDefinition>& clean_fun);

        // Clean the function body
        void cleanBody(std::unique_ptr<CleanFunctionDefinition>& clean_fun);
};

#endif
