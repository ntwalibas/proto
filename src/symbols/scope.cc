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

#include <exception>
#include <cstdbool>
#include <memory>
#include <string>

#include "ast/definitions/definition.h"
#include "symbols/symtable.h"
#include "symbols/scope.h"


Scope::Scope(
    std::shared_ptr<Scope> const& parent
) : parent(const_cast<std::shared_ptr<Scope>&>(parent))
{}


/**
 * Add a symbol to this scope symtable.
 */
void
Scope::addDefinition(
    std::string const& def_name,
    std::unique_ptr<Definition>& definition
)
{
    symtable.addDefinition(def_name, definition);
}

void
Scope::addDeclaration(
    std::string const& decl_name,
    std::unique_ptr<Declaration>& declaration
)
{
    symtable.addDeclaration(decl_name, declaration);
}


/**
 * Returns a symbol in this scope's symtable, given its name.
 * Throws std::out_of_range if no such symbol could be found.
 *
 * If `deep` is true, in case the symbol can't be found in the current scope,
 * the symbol will be searched for in the parent scope.
 */
std::unique_ptr<Definition>&
Scope::getDefinition(std::string const& def_name, bool deep)
{
    try {
        return symtable.getDefinition(def_name);
    } catch (std::out_of_range const& e) {
        if (deep && parent)
            return parent->getDefinition(def_name, deep);

        throw;
    }
}

std::unique_ptr<Declaration>&
Scope::getDeclaration(std::string const& decl_name, bool deep)
{
    try {
        return symtable.getDeclaration(decl_name);
    } catch (std::out_of_range const& e) {
        if (deep && parent)
            return parent->getDeclaration(decl_name, deep);

        throw;
    }
}


/**
 * Returns true if the given symbol exists in this scope's symtable.
 *
 * If `deep` is true, in case the symbol can't be found in the current scope,
 * the symbol will be searched for in the parent scope.
 */
bool
Scope::hasDefinition(std::string const& def_name, bool deep)
{
    bool res = symtable.hasDefinition(def_name);
    if (!res && parent)
        return parent->hasDefinition(def_name, deep);
    else
        return res;
}

bool
Scope::hasDeclaration(std::string const& decl_name, bool deep)
{
    bool res = symtable.hasDeclaration(decl_name);
    if (!res && parent)
        return parent->hasDeclaration(decl_name, deep);
    else
        return res;
}
