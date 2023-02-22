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

#ifndef PROTO_VARIABLE_DEFINITION_CHECKER_H
#define PROTO_VARIABLE_DEFINITION_CHECKER_H

#include <memory>

#include "ast/definitions/variable.h"
#include "ast/declarations/type.h"


class VariableDefinitionChecker
{
    public:
        VariableDefinitionChecker(std::unique_ptr<VariableDefinition>& variable_def);

        /**
         * Checking a variable definition (like all definitions) is a two-step process:
         *
         * 1. Check the header: that is the type of the variable.
         *    If this fails, the second step will not be invoked as it depends on
         *    this first step to succeed.
         *
         * 2. Check the body: this checks the initializer expression
              of the variable, and compares its inferred type with the one
              given from the first step.
         */
        void check();

    private:
        std::unique_ptr<VariableDefinition>& variable_def;

        // Check the type of the variable definition
        void checkHeader();

        // Check the initializer expression of the variable definition
        std::unique_ptr<TypeDeclaration> checkBody();
};

#endif
