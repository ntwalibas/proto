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

#include "checker/parsetree/declarations/variable.h"
#include "checker/parsetree/declarations/type.h"
#include "parsetree/declarations/variable.h"
#include "parsetree/declarations/type.h"


VariableDeclarationChecker::VariableDeclarationChecker(
    std::unique_ptr<VariableDeclaration>& variable_decl
) : variable_decl(variable_decl)
{}


/**
 * Checking variable declarations amounts to checking their types.
 * That is all this function does.
 */
void
VariableDeclarationChecker::check()
{
    std::unique_ptr<TypeDeclaration>& type_decl = 
        variable_decl->getTypeDeclaration();
    
    TypeDeclarationChecker type_checker(type_decl);
    type_checker.check();
}
