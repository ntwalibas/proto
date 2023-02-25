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

#ifndef PROTO_SYMBOLS_SYMTABLE_H
#define PROTO_SYMBOLS_SYMTABLE_H

#include <cstdbool>
#include <memory>
#include <string>
#include <array>
#include <map>

#include "ast/definitions/definition.h"
#include "ast/declarations/variable.h"


class Symtable
{
    public:
        /**
         * Add a symbol to the table.
         */
        bool addDefinition(std::string const& def_name, std::unique_ptr<Definition>& definition);
        bool addVariableDeclaration(std::string const& decl_name, std::unique_ptr<VariableDeclaration>& declaration);

        /**
         * Returns a symbol, given its name.
         * Throws std::out_of_range if no such symbol could be found.
         */
        std::unique_ptr<Definition>& getDefinition(std::string const& def_name);
        std::unique_ptr<VariableDeclaration>& getVariableDeclaration(std::string const& decl_name);

        /**
         * Returns true if the given symbol exists in this table.
         */
        bool hasDefinition(std::string const& def_name);\
        bool hasVariableDeclaration(std::string const& decl_name);

        /**
         * Returns all the symbols present in this table.
         */
        std::map<std::string, struct DefinitionSymbol>& getDefinitions();
        std::map<std::string, struct VariableDeclarationSymbol>& getVariableDeclarations();

        /**
         * Deletes all the symbols in this table.
         */
        void clear() noexcept;

        /**
         * Deletes all the definitons in this table.
         */
        void clearDefinitions() noexcept;

        /**
         * Deletes all the declarations in this table.
         */
        void clearVariableDeclarations() noexcept;

    private:
        std::map<std::string, struct DefinitionSymbol>  definitions;    /* Map between symbol name and symbol information. */
        std::map<std::string, struct VariableDeclarationSymbol> declarations;   /* Map between symbol name and symbol information. */
};

struct DefinitionSymbol
{
    DefinitionSymbol(std::unique_ptr<Definition>& definition, bool used);

    std::unique_ptr<Definition>&    definition;     /* Definition bound to this symbol. */
    bool                            used;           /* Has this symbol been used. */
};

struct VariableDeclarationSymbol
{
    VariableDeclarationSymbol(std::unique_ptr<VariableDeclaration>& declaration, bool used);

    std::unique_ptr<VariableDeclaration>&   declaration;    /* VariableDeclaration bound to this symbol. */
    bool                                    used;           /* Has this symbol been used. */
};


// Types symtable
// Since we don't have user-defined types at the moment,
// this symtable only stores the names of built-in types
class BuiltinTypesSymtable
{
    public:
        /**
         * Returns true if the given type is a builtin type.
         */
        static bool isBuiltinType(std::string const& type);

    private:
        const static std::array<std::string, 6> builtin_types;
};

#endif
