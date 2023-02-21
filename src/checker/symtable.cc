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
#include "checker/symtable.h"


/**
 * Add a definition to the table.
 * Throws std::invalid_argument if insertion happened.
 */
void
DefinitionsSymtable::addDefinition(
    std::string const& def_name,
    std::unique_ptr<Definition>& definition
)
{
    const auto [it, success] = symbols.insert({
        def_name,
        DefinitionSymbol(definition, false)
    });

    if (! success)
        throw std::invalid_argument(
            "Definition with name [" + def_name + "] already exists in the symtable."
        );
}

/**
 * Returns a definition, given its name.
 * Throws std::out_of_range if no such definition could be found.
 */
std::unique_ptr<Definition>&
DefinitionsSymtable::getDefinition(std::string const& def_name)
{
    auto def_sym = symbols.at(def_name);
    // Attempting to obtain a definition is equivalent to using it
    def_sym.used = true;
    return def_sym.definition;
}

/**
 * Returns true if the given definition exists in this table.
 */
bool
DefinitionsSymtable::hasDefinition(std::string const& def_name)
{
    try {
        auto& def_sym = symbols.at(def_name);
        // Checking if a definition exists is equivalent to using it
        def_sym.used = true;
        return true;
    } catch (std::out_of_range const& e) {
        return false;
    }
}

/**
 * Returns all the symbols present in this table.
 */
std::map<std::string, struct DefinitionSymbol>&
DefinitionsSymtable::getDefinitions()
{
    return symbols;
}

/**
 * Deletes all the symbols in this table.
 */
void
DefinitionsSymtable::clear() noexcept
{
    symbols.clear();
}


// Definition symbol
DefinitionSymbol::DefinitionSymbol(
    std::unique_ptr<Definition>& definition,
    bool used
) : definition(definition),
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

const std::array<std::string, 10>
BuiltinTypesSymtable::builtin_types = {
    "void",
    "bool",
    "uint32",
    "uint64",
    "int32",
    "int64",
    "float32",
    "float64",
    "char",
    "string"
};
