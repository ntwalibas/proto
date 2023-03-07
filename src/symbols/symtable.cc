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
#include "utils/inference.h"


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


// Builtin functions symtable
// Given a builtin function mangled name, we need its return type
// To avoid over-engineering, we encode the function parameters'
// types in the function's mangled name
BuiltinFunctionsSymtable::BuiltinFunctionsSymtable()
{
    // Unary operators
    // Plus
    fun_return_types["__pos__(int)"]            =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__pos__(uint)"]           =
        createSimpleTypeDeclaration(true, "uint");
    fun_return_types["__pos__(float)"]          =
        createSimpleTypeDeclaration(true, "float");
    // Minus
    fun_return_types["__neg__(int)"]            =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__neg__(uint)"]           =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__neg__(float)"]          =
        createSimpleTypeDeclaration(true, "float");
    // Bitwise not
    fun_return_types["__bnot__(int)"]           =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__bnot__(uint)"]          =
        createSimpleTypeDeclaration(true, "uint");
    // Logical not
    fun_return_types["__not__(bool)"]           =
        createSimpleTypeDeclaration(true, "bool");

    // Binary operators
    // Addition
    fun_return_types["__add__(int,int)"]        =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__add__(uint,uint)"]      =
        createSimpleTypeDeclaration(true, "uint");
    fun_return_types["__add__(float,float)"]    =
        createSimpleTypeDeclaration(true, "float");
    // Substraction
    fun_return_types["__sub__(int,int)"]        =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__sub__(uint,uint)"]      =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__sub__(float,float)"]    =
        createSimpleTypeDeclaration(true, "float");
    // Multiplication
    fun_return_types["__mul__(int,int)"]        =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__mul__(uint,uint)"]      =
        createSimpleTypeDeclaration(true, "uint");
    fun_return_types["__mul__(float,float)"]    =
        createSimpleTypeDeclaration(true, "float");
    // Division
    fun_return_types["__div__(int,int)"]        =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__div__(uint,uint)"]      =
        createSimpleTypeDeclaration(true, "uint");
    fun_return_types["__div__(float,float)"]    =
        createSimpleTypeDeclaration(true, "float");
    // Remainder
    fun_return_types["__rem__(int,int)"]        =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__rem__(uint,uint)"]      =
        createSimpleTypeDeclaration(true, "uint");
    fun_return_types["__rem__(float,float)"]    =
        createSimpleTypeDeclaration(true, "float");
    // Power
    fun_return_types["__pow__(int,int)"]        =
        createSimpleTypeDeclaration(true, "float");
    fun_return_types["__pow__(uint,uint)"]      =
        createSimpleTypeDeclaration(true, "uint");
    fun_return_types["__pow__(float,float)"]    =
        createSimpleTypeDeclaration(true, "float");
    // Bitwise and
    fun_return_types["__band__(int,int)"]       =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__band__(uint,uint)"]     =
        createSimpleTypeDeclaration(true, "uint");
    // Bitwise or
    fun_return_types["__bor__(int,int)"]        =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__bor__(uint,uint)"]      =
        createSimpleTypeDeclaration(true, "uint");
    // Bitwise xor
    fun_return_types["__xor__(int,int)"]        =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__xor__(uint,uint)"]      =
        createSimpleTypeDeclaration(true, "uint");
    // Left shift
    fun_return_types["__lshift__(int,uint)"]    =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__lshift__(uint,uint)"]   =
        createSimpleTypeDeclaration(true, "uint");
    // Right shift
    fun_return_types["__rshift__(int,uint)"]    =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__rshift__(uint,uint)"]   =
        createSimpleTypeDeclaration(true, "uint");
    // Logical and
    fun_return_types["__and__(bool,bool)"]      =
        createSimpleTypeDeclaration(true, "bool");
    // Logical or
    fun_return_types["__or__(bool,bool)"]       =
        createSimpleTypeDeclaration(true, "bool");
    // Equal
    fun_return_types["__eq__(int,int)"]         =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__eq__(uint,uint)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__eq__(float,float)"]     =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__eq__(bool,bool)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__eq__(string,string)"]   =
        createSimpleTypeDeclaration(true, "bool");
    // Not equal
    fun_return_types["__ne__(int,int)"]         =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__ne__(uint,uint)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__ne__(float,float)"]     =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__ne__(bool,bool)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__ne__(string,string)"]   =
        createSimpleTypeDeclaration(true, "bool");
    // Greater
    fun_return_types["__gt__(int,int)"]         =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__gt__(uint,uint)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__gt__(float,float)"]     =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__gt__(bool,bool)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__gt__(string,string)"]   =
        createSimpleTypeDeclaration(true, "bool");
    // Greater or equal
    fun_return_types["__ge__(int,int)"]         =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__ge__(uint,uint)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__ge__(float,float)"]     =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__ge__(bool,bool)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__ge__(string,string)"]   =
        createSimpleTypeDeclaration(true, "bool");
    // Less
    fun_return_types["__lt__(int,int)"]         =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__lt__(uint,uint)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__lt__(float,float)"]     =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__lt__(bool,bool)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__lt__(string,string)"]   =
        createSimpleTypeDeclaration(true, "bool");
    // Less or equal
    fun_return_types["__le__(int,int)"]         =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__le__(uint,uint)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__le__(float,float)"]     =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__le__(bool,bool)"]       =
        createSimpleTypeDeclaration(true, "bool");
    fun_return_types["__le__(string,string)"]   =
        createSimpleTypeDeclaration(true, "bool");
    // In-place addition
    fun_return_types["__iadd__(int,int)"]       =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__iadd__(uint,uint)"]     =
        createSimpleTypeDeclaration(true, "uint");
    fun_return_types["__iadd__(float,float)"]   =
        createSimpleTypeDeclaration(true, "float");
    // In-place substraction
    fun_return_types["__isub__(int,int)"]       =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__isub__(uint,uint)"]     =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__isub__(float,float)"]   =
        createSimpleTypeDeclaration(true, "float");
    // In-place multiplication
    fun_return_types["__imul__(int,int)"]       =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__imul__(uint,uint)"]     =
        createSimpleTypeDeclaration(true, "uint");
    fun_return_types["__imul__(float,float)"]   =
        createSimpleTypeDeclaration(true, "float");
    // In-place division
    fun_return_types["__idiv__(int,int)"]       =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__idiv__(uint,uint)"]     =
        createSimpleTypeDeclaration(true, "uint");
    fun_return_types["__idiv__(float,float)"]   =
        createSimpleTypeDeclaration(true, "float");
    // In-place remainder
    fun_return_types["__irem__(int,int)"]       =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__irem__(uint,uint)"]     =
        createSimpleTypeDeclaration(true, "uint");
    fun_return_types["__irem__(float,float)"]   =
        createSimpleTypeDeclaration(true, "float");
    // In-place power
    fun_return_types["__ipow__(int,int)"]       =
        createSimpleTypeDeclaration(true, "float");
    fun_return_types["__ipow__(uint,uint)"]     =
        createSimpleTypeDeclaration(true, "uint");
    fun_return_types["__ipow__(float,float)"]   =
        createSimpleTypeDeclaration(true, "float");
    // In-place bitwise and
    fun_return_types["__iand__(int,int)"]       =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__iand__(uint,uint)"]     =
        createSimpleTypeDeclaration(true, "uint");
    // In-place bitwise or
    fun_return_types["__ior__(int,int)"]        =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__ior__(uint,uint)"]      =
        createSimpleTypeDeclaration(true, "uint");
    // In-place bitwise xor
    fun_return_types["__ixor__(int,int)"]       =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__ixor__(uint,uint)"]     =
        createSimpleTypeDeclaration(true, "uint");
    // In-place left shift
    fun_return_types["__ilshift__(int,uint)"]   =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__ilshift__(uint,uint)"]  =
        createSimpleTypeDeclaration(true, "uint");
    // In-place right shift
    fun_return_types["__irshift__(int,uint)"]   =
        createSimpleTypeDeclaration(true, "int");
    fun_return_types["__irshift__(uint,uint)"]  =
        createSimpleTypeDeclaration(true, "uint");

    // Cast operators
    // unsigned int to signed int
    fun_return_types["__cast@int__(uint)"]      =
        createSimpleTypeDeclaration(true, "int");
    // unsigned int to float
    fun_return_types["__cast@float__(uint)"]    =
        createSimpleTypeDeclaration(true, "float");
    // unsigned int to string
    fun_return_types["__cast@string__(uint)"]   =
        createSimpleTypeDeclaration(true, "string");
    // unsigned int to bool
    fun_return_types["__cast@bool__(uint)"]     =
        createSimpleTypeDeclaration(true, "bool");
    // signed int to unsigned int
    fun_return_types["__cast@uint__(int)"]      =
        createSimpleTypeDeclaration(true, "uint");
    // signed int to float
    fun_return_types["__cast@float__(int)"]     =
        createSimpleTypeDeclaration(true, "float");
    // signed int to string
    fun_return_types["__cast@string__(int)"]    =
        createSimpleTypeDeclaration(true, "string");
    // signed int to bool
    fun_return_types["__cast@bool__(int)"]      =
        createSimpleTypeDeclaration(true, "bool");
    // float to unsigned int
    fun_return_types["__cast@uint__(float)"]    =
        createSimpleTypeDeclaration(true, "uint");
    // float to signed int
    fun_return_types["__cast@int__(float)"]     =
        createSimpleTypeDeclaration(true, "int");
    // float to string
    fun_return_types["__cast@string__(float)"]  =
        createSimpleTypeDeclaration(true, "string");
}

std::unique_ptr<TypeDeclaration>&
BuiltinFunctionsSymtable::getReturnType(std::string& function_mangled_name)
{
    return fun_return_types.at(function_mangled_name);
}


bool
BuiltinFunctionsSymtable::hasFunctionDefinition(std::string const& function_mangled_name)
{
    try {
        fun_return_types.at(function_mangled_name);
        return true;
    } catch (std::out_of_range const& e) {
        return false;
    }
}