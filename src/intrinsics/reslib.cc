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
#include <memory>
#include <string>
#include <map>

#include "parsetree/declarations/type.h"
#include "intrinsics/reslib.h"
#include "utils/inference.h"


ReslibFunctionsSymtable::ReslibFunctionsSymtable()
{
    // Unary operators
    // Plus
    fun_return_types["__pos__(int)"]            =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__pos__(uint)"]           =
        createSimpleTypeDeclaration(false,"uint");
    fun_return_types["__pos__(float)"]          =
        createSimpleTypeDeclaration(false,"float");
    // Minus
    fun_return_types["__neg__(int)"]            =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__neg__(uint)"]           =
        createSimpleTypeDeclaration(false,"uint");
    fun_return_types["__neg__(float)"]          =
        createSimpleTypeDeclaration(false,"float");
    // Bitwise not
    fun_return_types["__bnot__(int)"]           =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__bnot__(uint)"]          =
        createSimpleTypeDeclaration(false,"uint");
    // Logical not
    fun_return_types["__not__(bool)"]           =
        createSimpleTypeDeclaration(false,"bool");

    // Binary operators
    // Addition
    fun_return_types["__add__(int,int)"]        =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__add__(uint,uint)"]      =
        createSimpleTypeDeclaration(false,"uint");
    fun_return_types["__add__(float,float)"]    =
        createSimpleTypeDeclaration(false,"float");
    // Substraction
    fun_return_types["__sub__(int,int)"]        =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__sub__(uint,uint)"]      =
        createSimpleTypeDeclaration(false,"uint");
    fun_return_types["__sub__(float,float)"]    =
        createSimpleTypeDeclaration(false,"float");
    // Multiplication
    fun_return_types["__mul__(int,int)"]        =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__mul__(uint,uint)"]      =
        createSimpleTypeDeclaration(false,"uint");
    fun_return_types["__mul__(float,float)"]    =
        createSimpleTypeDeclaration(false,"float");
    // Division
    fun_return_types["__div__(int,int)"]        =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__div__(uint,uint)"]      =
        createSimpleTypeDeclaration(false,"uint");
    fun_return_types["__div__(float,float)"]    =
        createSimpleTypeDeclaration(false,"float");
    // Remainder
    fun_return_types["__rem__(int,int)"]        =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__rem__(uint,uint)"]      =
        createSimpleTypeDeclaration(false,"uint");
    fun_return_types["__rem__(float,float)"]    =
        createSimpleTypeDeclaration(false,"float");
    // Power
    fun_return_types["__pow__(int,int)"]        =
        createSimpleTypeDeclaration(false,"float");
    fun_return_types["__pow__(uint,uint)"]      =
        createSimpleTypeDeclaration(false,"uint");
    fun_return_types["__pow__(float,float)"]    =
        createSimpleTypeDeclaration(false,"float");
    // Bitwise and
    fun_return_types["__band__(int,int)"]       =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__band__(uint,uint)"]     =
        createSimpleTypeDeclaration(false,"uint");
    // Bitwise or
    fun_return_types["__bor__(int,int)"]        =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__bor__(uint,uint)"]      =
        createSimpleTypeDeclaration(false,"uint");
    // Bitwise xor
    fun_return_types["__xor__(int,int)"]        =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__xor__(uint,uint)"]      =
        createSimpleTypeDeclaration(false,"uint");
    // Left shift
    fun_return_types["__lshift__(int,uint)"]    =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__lshift__(uint,uint)"]   =
        createSimpleTypeDeclaration(false,"uint");
    // Right shift
    fun_return_types["__rshift__(int,uint)"]    =
        createSimpleTypeDeclaration(false,"int");
    fun_return_types["__rshift__(uint,uint)"]   =
        createSimpleTypeDeclaration(false,"uint");
    // Logical and
    fun_return_types["__and__(bool,bool)"]      =
        createSimpleTypeDeclaration(false,"bool");
    // Logical or
    fun_return_types["__or__(bool,bool)"]       =
        createSimpleTypeDeclaration(false,"bool");
    // Equal
    fun_return_types["__eq__(int,int)"]         =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__eq__(uint,uint)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__eq__(float,float)"]     =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__eq__(bool,bool)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__eq__(string,string)"]   =
        createSimpleTypeDeclaration(false,"bool");
    // Not equal
    fun_return_types["__ne__(int,int)"]         =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__ne__(uint,uint)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__ne__(float,float)"]     =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__ne__(bool,bool)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__ne__(string,string)"]   =
        createSimpleTypeDeclaration(false,"bool");
    // Greater
    fun_return_types["__gt__(int,int)"]         =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__gt__(uint,uint)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__gt__(float,float)"]     =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__gt__(bool,bool)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__gt__(string,string)"]   =
        createSimpleTypeDeclaration(false,"bool");
    // Greater or equal
    fun_return_types["__ge__(int,int)"]         =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__ge__(uint,uint)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__ge__(float,float)"]     =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__ge__(bool,bool)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__ge__(string,string)"]   =
        createSimpleTypeDeclaration(false,"bool");
    // Less
    fun_return_types["__lt__(int,int)"]         =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__lt__(uint,uint)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__lt__(float,float)"]     =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__lt__(bool,bool)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__lt__(string,string)"]   =
        createSimpleTypeDeclaration(false,"bool");
    // Less or equal
    fun_return_types["__le__(int,int)"]         =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__le__(uint,uint)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__le__(float,float)"]     =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__le__(bool,bool)"]       =
        createSimpleTypeDeclaration(false,"bool");
    fun_return_types["__le__(string,string)"]   =
        createSimpleTypeDeclaration(false,"bool");

    // Cast operators
    // unsigned int to signed int
    fun_return_types["__cast@int__(uint)"]      =
        createSimpleTypeDeclaration(false,"int");
    // unsigned int to float
    fun_return_types["__cast@float__(uint)"]    =
        createSimpleTypeDeclaration(false,"float");
    // unsigned int to string
    fun_return_types["__cast@string__(uint)"]   =
        createSimpleTypeDeclaration(false,"string");
    // unsigned int to bool
    fun_return_types["__cast@bool__(uint)"]     =
        createSimpleTypeDeclaration(false,"bool");
    // signed int to unsigned int
    fun_return_types["__cast@uint__(int)"]      =
        createSimpleTypeDeclaration(false,"uint");
    // signed int to float
    fun_return_types["__cast@float__(int)"]     =
        createSimpleTypeDeclaration(false,"float");
    // signed int to string
    fun_return_types["__cast@string__(int)"]    =
        createSimpleTypeDeclaration(false,"string");
    // signed int to bool
    fun_return_types["__cast@bool__(int)"]      =
        createSimpleTypeDeclaration(false,"bool");
    // float to unsigned int
    fun_return_types["__cast@uint__(float)"]    =
        createSimpleTypeDeclaration(false,"uint");
    // float to signed int
    fun_return_types["__cast@int__(float)"]     =
        createSimpleTypeDeclaration(false,"int");
    // float to string
    fun_return_types["__cast@string__(float)"]  =
        createSimpleTypeDeclaration(false,"string");
}

std::unique_ptr<TypeDeclaration>&
ReslibFunctionsSymtable::getReturnType(std::string& function_mangled_name)
{
    return fun_return_types.at(function_mangled_name);
}

bool
ReslibFunctionsSymtable::hasFunctionDefinition(std::string const& function_mangled_name)
{
    try {
        fun_return_types.at(function_mangled_name);
        return true;
    } catch (std::out_of_range const& e) {
        return false;
    }
}
