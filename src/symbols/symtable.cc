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

#include <algorithm>
#include <exception>
#include <cstdbool>
#include <memory>
#include <string>
#include <array>
#include <map>

#include "ast/definitions/definition.h"
#include "symbols/symtable.h"


/**
 * Add a symbol to the table.
 */
bool
Symtable::addDefinition(
    std::string const& def_name,
    std::unique_ptr<Definition>& definition
)
{
    const auto [it, success] = definitions.insert({
        def_name,
        DefinitionSymbol(definition, false)
    });

    return success;
}

bool
Symtable::addVariableDeclaration(
    std::string const& decl_name,
    std::unique_ptr<VariableDeclaration>& declaration
)
{
    const auto [it, success] = declarations.insert({
        decl_name,
        VariableDeclarationSymbol(declaration, false)
    });

    return success;
}

/**
 * Returns a symbol, given its name.
 * Throws std::out_of_range if no such symbol could be found.
 */
std::unique_ptr<Definition>&
Symtable::getDefinition(std::string const& def_name)
{
    auto def_sym = definitions.at(def_name);
    // Attempting to obtain a definition is equivalent to using it
    def_sym.used = true;
    return def_sym.definition;
}

std::unique_ptr<VariableDeclaration>&
Symtable::getVariableDeclaration(std::string const& decl_name)
{
    auto decl_sym = declarations.at(decl_name);
    // Attempting to obtain a declaration is equivalent to using it
    decl_sym.used = true;
    return decl_sym.declaration;
}

/**
 * Returns true if the given symbol exists in this table.
 */
bool
Symtable::hasDefinition(std::string const& def_name)
{
    try {
        auto& def_sym = definitions.at(def_name);
        // Checking if a definition exists is equivalent to using it
        def_sym.used = true;
        return true;
    } catch (std::out_of_range const& e) {
        return false;
    }
}

bool
Symtable::hasVariableDeclaration(std::string const& decl_name)
{
    try {
        auto& decl_sym = declarations.at(decl_name);
        // Checking if a declaration exists is equivalent to using it
        decl_sym.used = true;
        return true;
    } catch (std::out_of_range const& e) {
        return false;
    }
}

/**
 * Returns all the symbols present in this table.
 */
std::map<std::string, struct DefinitionSymbol>&
Symtable::getDefinitions()
{
    return definitions;
}

std::map<std::string, struct VariableDeclarationSymbol>&
Symtable::getVariableDeclarations()
{
    return declarations;
}

/**
 * Deletes all the symbols in this table.
 */
void
Symtable::clear() noexcept
{
    definitions.clear();
    declarations.clear();
}

/**
 * Deletes all the definitons in this table.
 */
void
Symtable::clearDefinitions() noexcept
{
    definitions.clear();
}

/**
 * Deletes all the declarations in this table.
 */
void
Symtable::clearVariableDeclarations() noexcept
{
    declarations.clear();
}


// Definition symbol
DefinitionSymbol::DefinitionSymbol(
    std::unique_ptr<Definition>& definition,
    bool used
) : definition(definition),
    used(used)
{}

// VariableDeclaration symbol
VariableDeclarationSymbol::VariableDeclarationSymbol(
    std::unique_ptr<VariableDeclaration>& declaration,
    bool used
) : declaration(declaration),
    used(used)
{}


// Types symtable
bool
BuiltinTypesSymtable::isBuiltinType(std::string const& type)
{
    return std::find(
        BuiltinTypesSymtable::builtin_types.begin(),
        BuiltinTypesSymtable::builtin_types.end(),
        type
    ) != BuiltinTypesSymtable::builtin_types.end();
}

const std::array<std::string, 6>
BuiltinTypesSymtable::builtin_types{
    "void",
    "bool",
    "int",
    "uint",
    "float",
    "string"
};
