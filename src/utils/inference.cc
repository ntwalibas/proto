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
#include <string>

#include "ast/declarations/type.h"
#include "common/token_type.h"
#include "utils/inference.h"
#include "common/token.h"

static Token
makeToken(enum TokenType token_type, std::string const& lexeme);


/**
 * Given a builtin type name, return a simple type declaration with that name.
 */
std::unique_ptr<SimpleTypeDeclaration>
createSimpleTypeDeclaration(bool is_const, std::string const& name)
{
    Token token = makeToken(PROTO_IDENTIFIER, name);
    return std::make_unique<SimpleTypeDeclaration>(
        is_const,
        token
    );
}


/**
 * Given a builtin type name and size, return an array type declaration with that name.
 */
std::unique_ptr<ArrayTypeDeclaration>
createArrayTypeDeclaration(bool is_const, long size, std::string const& name)
{
    Token simple_token = makeToken(PROTO_IDENTIFIER, name);
    std::unique_ptr<SimpleTypeDeclaration> simple_decl =
        std::make_unique<SimpleTypeDeclaration>(
            is_const,
            simple_token
        );
    
    Token array_token = makeToken(PROTO_LEFT_BRACKET, "[");
    return std::make_unique<ArrayTypeDeclaration>(
        is_const,
        array_token,
        size,
        * simple_decl
    );
}

static Token
makeToken(enum TokenType token_type, std::string const& lexeme)
{
    std::shared_ptr<std::string> source =
        std::make_shared<std::string>(lexeme);

    return Token(
        token_type,
        source,
        std::string("__builtin__"),
        source->begin(),
        source->length(),
        1,
        source->length() + 1
    );
}