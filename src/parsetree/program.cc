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

#include <cstddef>
#include <utility>
#include <memory>
#include <vector>

#include "parsetree/definitions/definition.h"
#include "symbols/scope.h"
#include "parsetree/program.h"


Program::Program() : scope(std::make_shared<Scope>(nullptr))
{}


/**
 * Adds a definition to this program.
 */
void
Program::addDefinition(std::unique_ptr<Definition>&& definition)
{
    definitions.push_back(std::move(definition));
}


/**
 * Returns all definitions in this program.
 */
std::vector<std::unique_ptr<Definition>>&
Program::getDefinitions()
{
    return definitions;
}


/**
 * Returns the program associated with this scope. 
 */
std::shared_ptr<Scope>&
Program::getScope()
{
    return scope;
}
