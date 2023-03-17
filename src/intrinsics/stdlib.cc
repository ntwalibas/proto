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
#include <string>
#include <map>

#include "parsetree/declarations/type.h"
#include "intrinsics/stdlib.h"
#include "utils/inference.h"

StdlibFunctionsSymtable::StdlibFunctionsSymtable()
{
    // Print booleans without newline
    fun_return_types["print(bool)"] =
        createSimpleTypeDeclaration(false,"void");

    // Print booleans with newline
    fun_return_types["println(bool)"]  =
        createSimpleTypeDeclaration(false,"void");

    // Print signed int without newline
    fun_return_types["print(int)"] =
        createSimpleTypeDeclaration(false,"void");

    // Print signed int with newline
    fun_return_types["println(int)"] =
        createSimpleTypeDeclaration(false,"void");

    // Print unsigned int without newline
    fun_return_types["print(uint)"] =
        createSimpleTypeDeclaration(false,"void");

    // Print unsigned int with newline
    fun_return_types["println(uint)"] =
        createSimpleTypeDeclaration(false,"void");

    // Print float without newline
    fun_return_types["print(float)"] =
        createSimpleTypeDeclaration(false,"void");

    // Print float with newline
    fun_return_types["println(float)"] =
        createSimpleTypeDeclaration(false,"void");

    // Print string without newline
    fun_return_types["print(string)"] =
        createSimpleTypeDeclaration(false,"void");

    // Print string with newline
    fun_return_types["println(string)"] =
        createSimpleTypeDeclaration(false,"void");
}

std::unique_ptr<TypeDeclaration>&
StdlibFunctionsSymtable::getReturnType(std::string& function_mangled_name)
{
    return fun_return_types.at(function_mangled_name);
}

bool
StdlibFunctionsSymtable::hasFunctionDefinition(std::string const& function_mangled_name)
{
    try {
        fun_return_types.at(function_mangled_name);
        return true;
    } catch (std::out_of_range const& e) {
        return false;
    }
}
