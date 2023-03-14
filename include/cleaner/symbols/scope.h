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

#ifndef PROTO_CLEANER_SCOPE_H
#define PROTO_CLEANER_SCOPE_H

#include <type_traits>
#include <stdexcept>
#include <cstdbool>
#include <cstddef>
#include <utility>
#include <memory>
#include <string>

#include "cleaner/ast/definitions/function.h"
#include "cleaner/ast/definitions/variable.h"
#include "symtable.h"


struct CleanScope
{
    CleanScope(
        std::shared_ptr<CleanScope> const& parent
    ) : parent(const_cast<std::shared_ptr<CleanScope>&>(parent))
    {}

    /**
     * Add a function or variable definition to this scope.
     */
    template<typename T>
    void addSymbol(std::string const& key, std::unique_ptr<T>&& symbol)
    {
        if constexpr (std::is_same_v<T, CleanFunctionDefinition>) {
            fun_defs.addSymbol(key, std::move(symbol));
        } else if constexpr (std::is_same_v<T, CleanVariableDefinition>) {
            var_defs.addSymbol(key, std::move(symbol));
        } else {
            throw std::invalid_argument(
                std::string("Can only add function and variable definitions,") +
                " and variable declarations to the scope."
            );
        }
    }

    /**
     * Check if a function or variable definition exists in this scope. 
     */
    template<typename T>
    bool hasSymbol(std::string const& key, bool deep = false)
    {
        bool res = false;
        if constexpr (std::is_same_v<T, CleanFunctionDefinition>) {
            res = fun_defs.hasSymbol(key);
        } else if constexpr (std::is_same_v<T, CleanVariableDefinition>) {
            res = var_defs.hasSymbol(key);
        } else {
            throw std::invalid_argument(
                std::string("Can only check if function and variable definitions,") +
                " and variable declarations exists in the scope."
            );
        }

        if (!res && deep && parent)
            return parent->hasSymbol<T>(key, deep);

        return res;
    }

    /**
     * Returns the function or variable definition if it exists.
     */
    template<typename T>
    std::unique_ptr<T>& getSymbol(std::string const& key, bool deep = false)
    {
        try {
            if constexpr (std::is_same_v<T, CleanFunctionDefinition>) {
                return fun_defs.getSymbol(key);
            } else if constexpr (std::is_same_v<T, CleanVariableDefinition>) {
                return var_defs.getSymbol(key);
            } else {
                throw std::invalid_argument(
                    std::string("Can only get function and variable definitions,") +
                    "and variable declarations from this scope."
                );
            }
        } catch (std::out_of_range const& e) {
            if (deep && parent)
                return parent->getSymbol<T>(key, deep);
            
            throw;
        }
    }

    std::shared_ptr<CleanScope> parent;
    CleanSymtable<std::unique_ptr<CleanFunctionDefinition>> fun_defs;
    CleanSymtable<std::unique_ptr<CleanVariableDefinition>> var_defs;
};

#endif
