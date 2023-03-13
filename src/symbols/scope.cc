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

#include <stdexcept>
#include <cstdbool>
#include <memory>
#include <string>

#include "parsetree/definitions/definition.h"
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
Scope::addVariableDeclaration(
    std::string const& decl_name,
    std::unique_ptr<VariableDeclaration>& declaration
)
{
    symtable.addVariableDeclaration(decl_name, declaration);
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
        std::unique_ptr<Definition>& def =
            symtable.getDefinition(def_name);
        def->isUsed(true);
        return def;
    } catch (std::out_of_range const& e) {
        if (deep && parent)
            return parent->getDefinition(def_name, deep);

        throw;
    }
}

std::unique_ptr<VariableDeclaration>&
Scope::getVariableDeclaration(std::string const& decl_name, bool deep)
{
    try {
        return symtable.getVariableDeclaration(decl_name);
    } catch (std::out_of_range const& e) {
        if (deep && parent)
            return parent->getVariableDeclaration(decl_name, deep);

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
    if (!res && deep && parent)
        return parent->hasDefinition(def_name, deep);
    else
        return res;
}

bool
Scope::hasVariableDeclaration(std::string const& decl_name, bool deep)
{
    bool res = symtable.hasVariableDeclaration(decl_name);
    if (!res && parent)
        return parent->hasVariableDeclaration(decl_name, deep);
    else
        return res;
}


/**
 * Returns true if this scope has a parent.
 */
bool
Scope::hasParent()
{
    return parent != nullptr;
}
