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

#ifndef PROTO_VARIABLE_DEFINITION_INTERPRETER_H
#define PROTO_VARIABLE_DEFINITION_INTERPRETER_H

#include <memory>

#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/definitions/variable.h"
#include "cleaner/symbols/scope.h"


class VariableDefinitionInterpreter
{
    public:
        /**
         * Interprets the given variable definition.
         */
        std::unique_ptr<CleanExpression> interpret(
            CleanVariableDefinition* var_def, CleanScope* scope);
};

#endif
