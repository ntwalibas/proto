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

#ifndef PROTO_FUNCTION_DEFINITION_INTERPRETER_H
#define PROTO_FUNCTION_DEFINITION_INTERPRETER_H

#include "cleaner/ast/definitions/function.h"
#include "cleaner/symbols/scope.h"


class FunctionDefinitionInterpreter
{
    public:
        FunctionDefinitionInterpreter();

        /**
         * Interprets the given function definition.
         */
        void interpret(CleanFunctionDefinition* fun_def, CleanScope* scope);
};

#endif
