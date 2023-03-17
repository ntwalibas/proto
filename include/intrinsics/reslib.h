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

#ifndef PROTO_INTRISINCS_RESLIB_H
#define PROTO_INTRISINCS_RESLIB_H

#include <memory>
#include <string>
#include <map>

#include "parsetree/declarations/type.h"


class ReslibFunctionsSymtable
{
    public:
        ReslibFunctionsSymtable();

        std::unique_ptr<TypeDeclaration>& getReturnType(std::string& function_mangled_name);

        bool hasFunctionDefinition(std::string const& function_mangled_name);
    
    private:
        std::map<std::string, std::unique_ptr<TypeDeclaration>> fun_return_types;
};

#endif
