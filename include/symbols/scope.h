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

#ifndef PROTO_SYMBOLS_SCOPE_H
#define PROTO_SYMBOLS_SCOPE_H

#include <cstdbool>
#include <memory>
#include <string>

#include "ast/definitions/definition.h"
#include "symtable.h"


class Scope
{
    public:
        Scope(std::shared_ptr<Scope> const& parent);

        /**
         * Add a symbol to this scope symtable.
         */
        void addDefinition(std::string const& def_name, std::unique_ptr<Definition>& definition);
        void addVariableDeclaration(std::string const& decl_name, std::unique_ptr<VariableDeclaration>& declaration);

        /**
         * Returns a symbol in this scope's symtable, given its name.
         * Throws std::out_of_range if no such symbol could be found.
         *
         * If `deep` is true, in case the symbol can't be found in the current scope,
         * the symbol will be searched for in the parent scope.
         */
        std::unique_ptr<Definition>& getDefinition(std::string const& def_name, bool deep = false);
        std::unique_ptr<VariableDeclaration>& getVariableDeclaration(std::string const& decl_name, bool deep = false);

        /**
         * Returns true if the given symbol exists in this scope's symtable.
         *
         * If `deep` is true, in case the symbol can't be found in the current scope,
         * the symbol will be searched for in the parent scope.
         */
        bool hasDefinition(std::string const& def_name, bool deep = false);
        bool hasVariableDeclaration(std::string const& decl_name, bool deep = false);

        /**
         * Returns true if this scope has a parent.
         */
        bool hasParent();

    private:
        std::shared_ptr<Scope>  parent;     /* The scope parent to this one. */
        Symtable                symtable;   /* Symtable for definitions found in this scope. */
};

#endif
