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

#ifndef PROTO_COMMON_TOKEN_H
#define PROTO_COMMON_TOKEN_H

#include <memory>
#include <string>

#include "token_type.h"


struct Token
{
    Token();

    Token(
        enum TokenType type,
        std::shared_ptr<std::string>& source,
        std::string const& source_path,
        std::string::iterator start,
        std::string::size_type length,
        std::string::size_type line,
        std::string::size_type column
    );
    Token(Token const& token) = default;
    Token& operator=(Token const& token) noexcept = default;
    Token(Token&& token) noexcept = default;
    Token& operator=(Token&& token) noexcept = default;
    ~Token() noexcept = default;

    /**
     * Returns the lexeme for the given token.
     */
    std::string getLexeme() const;

    /**
     * Returns line information where this token was found.
     */
    struct TokenLine getLine() const;

    enum TokenType                  type;           /* Type of token */
    std::shared_ptr<std::string>    source;         /* Source containing this token */
    std::string                     source_path;    /* File containing the source where this token was found */
    std::string::iterator           start;          /* Index where this token occurs in the source */
    std::string::size_type          length;         /* Length of the token */
    std::string::size_type          line;           /* Line where the token occurs */
    std::string::size_type          column;         /* Column where the token occurs */
};


struct TokenLine
{
    TokenLine(Token const& token);

    std::string                     line;           /* Line where the given token lies */
    std::string::size_type          offset;         /* How far the token's lexeme is from the beginning of the line */
};

#endif
