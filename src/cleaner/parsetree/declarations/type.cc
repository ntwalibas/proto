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

#include <utility>
#include <memory>

#include "cleaner/parsetree/declarations/type.h"
#include "cleaner/ast/declarations/type.h"
#include "parsetree/declarations/type.h"

static std::unique_ptr<CleanTypeDeclaration>
cleanSimpleType(SimpleTypeDeclaration* simple_type_decl);


TypeDeclarationCleaner::TypeDeclarationCleaner(
    TypeDeclaration* type_decl
) : type_decl(type_decl)
{}


/**
 * Produces a type declaration on the AST from the parse tree.
 */
std::unique_ptr<CleanTypeDeclaration>
TypeDeclarationCleaner::clean()
{
    SimpleTypeDeclaration* simple_type_decl =
        static_cast<SimpleTypeDeclaration*>(type_decl);
    return cleanSimpleType(simple_type_decl);
}


static std::unique_ptr<CleanTypeDeclaration>
cleanSimpleType(SimpleTypeDeclaration* simple_type_decl)
{
    return std::make_unique<CleanSimpleTypeDeclaration>(
        simple_type_decl->isConst(),
        simple_type_decl->getTypeName()
    );
}
