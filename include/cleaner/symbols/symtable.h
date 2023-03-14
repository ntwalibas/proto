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

#ifndef PROTO_CLEANER_SYTAMBLE_H
#define PROTO_CLEANER_SYTAMBLE_H

#include <stdexcept>
#include <utility>
#include <string>
#include <map>


template<typename T>
struct CleanSymtable
{
    /**
     * Add a symbol to this table.
     */
    void addSymbol(std::string const& key, T&& value)
    {
        symbols[key] = std::move(value);
    }

    /**
     * Verify if a symbol exists in this table.
     */
    bool hasSymbol(std::string const& key)
    {
        try {
            symbols.at(key);
            return true;
        } catch (std::out_of_range const& e) {
            return false;
        }
    }

    /**
     * Returns the symbol given the associated key.
     * Throws std::out_of_range if not exists.
     */
    T& getSymbol(std::string const& key)
    {
        return symbols.at(key);
    }

    std::map<std::string, T> symbols;       
};

#endif
