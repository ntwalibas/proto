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

#include "interpreter/ast/expressions/expression.h"
#include "interpreter/ast/definitions/variable.h"
#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/definitions/variable.h"
#include "cleaner/symbols/scope.h"


/**
 * Interprets the given variable definition.
 */
std::unique_ptr<CleanExpression>
VariableDefinitionInterpreter::interpret(
    CleanVariableDefinition* var_def,
    CleanScope* scope
)
{
    return ExpressionInterpreter(scope).interpret(var_def->initializer.get());
}
