/*  This file is part of the Nova programming language
 * 
 *  Copyright (c) 2022- Ntwali Bashige Toussaint
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


#ifndef PROTO_LEXER_H
#define PROTO_LEXER_H

#include <exception>
#include <cstdbool>
#include <memory>
#include <string>
#include <map>

#include "common/token_type.h"
#include "common/token.h"


class Lexer
{
    public:
        Lexer(std::shared_ptr<std::string> const& source, std::string const& source_path);

        /**
         * Returns the next token in the stream with each call.
         */
        Token lex();

    private:
        std::shared_ptr<std::string>    source;         /* Source code to lex. */
        std::string                     source_path;    /* Path to the source code for error message. */
        std::string::iterator           start;          /* Start of the token currently being scanned. */
        std::string::iterator           current;        /* Pointer to the current character in the source. */
        std::string::size_type          line;           /* Line where the scanned token was found. */
        std::string::size_type          column;         /* Column where the scanned token was found. */
        std::size_t                     num_tokens;     /* Total number of tokens scanned in the source. */

        std::map<std::string,
            enum TokenType>             keywords;       /* All supported keywords */

        /**
         * Lex a number.
         */
        Token number();

        /**
         * Lex a string.
         */
        Token string();

        /**
         * Lex an identifier.
         */
        Token identifier();

        /**
         * Handle whitespace, this including comments.
         */
        void skipWhitespace();
        void skipSingleComment(bool issue_new_line);
        void skipMultiComment(bool issue_new_line);

        /**
         * Utilities to navigate through the stream.
         */
        // Returns true if we are at the beginning of the source.
        bool atStart();

        // Returns true if we are at the end of the source.
        bool atEnd();

        // Returns current char in the stream and advance to the next.
        char advance();

        // Return true if current character matches given one,
        // Advance to the next character if match.
        bool match(char expected);

        // Returns current character in the stream.
        char peek();

        // Returns next character in the stream.
        char peekNext();

        // Returns the previous character in the stream.
        char peekBack();

        // Create a token.
        Token makeToken(enum TokenType type);
};


class LexerError : public std::runtime_error
{
    public:
        LexerError(std::string const & what_arg, Token error_tok)
        : std::runtime_error(what_arg), error_tok(error_tok) {}

        LexerError(const char * what_arg, Token error_tok)
        : std::runtime_error(what_arg), error_tok(error_tok) {}

    private:
        Token error_tok;
};

#endif
