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

#include "checker/ast/declarations/type.h"
#include "checker/checker_error.h"
#include "ast/declarations/type.h"
#include "checker/symtable.h"

static void checkSimpleType(SimpleTypeDeclaration& simple_type_decl);

TypeDeclarationChecker::TypeDeclarationChecker(
    std::unique_ptr<TypeDeclaration>& type_decl
) : type_decl(type_decl)
{}


/**
 * If we have a simple type, make sure it is in the builtin types symtable.
 * If an array, just check the simple type it contains.
 */
void
TypeDeclarationChecker::check()
{
    if (type_decl->getTypeCategory() == TypeCategory::Simple) {
        SimpleTypeDeclaration& simple_type_decl =
            dynamic_cast<SimpleTypeDeclaration&>(*type_decl);
        checkSimpleType(simple_type_decl);
    }
    else {
        ArrayTypeDeclaration& array_type_decl =
            dynamic_cast<ArrayTypeDeclaration&>(*type_decl);
        checkSimpleType(array_type_decl.getSimpleType());
    }
}

static void
checkSimpleType(SimpleTypeDeclaration& simple_type_decl)
{
    Token& decl_token = simple_type_decl.getToken();
    std::string decl_lexeme = decl_token.getLexeme();

    if (BuiltinTypesSymtable::isBuiltinType(decl_lexeme) == false)
        throw CheckerError(
            decl_token,
            "unknown type",
            std::string("type [") + decl_lexeme + std::string("] does not exist"),
            true
        );
}
