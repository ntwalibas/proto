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


#ifndef PROTO_PARSER_H
#define PROTO_PARSER_H

#include <exception>
#include <string>

#include "common/token.h"
#include "lexer/lexer.h"
#include "ast/program.h"


class Parser
{
    public:
        Parser(Lexer& lexer);

        /**
         * Parses the token stream and returns an AST representing the program.
         */
        Program parse();

        Program parseProgram();

    private:
        Lexer lexer;    /* Lexer to generate tokens */
        Token previous; /* Last token to be consumed. */
        Token current;  /* Token to be consumed. */
        Token next;     /* Next token to be consumed. */
};


class ParserError : public std::runtime_error
{
    public:
        ParserError(
            Token error_tok,
            std::string const& primary_message,
            std::string const& secondary_message,
            bool fatal
        )
        : std::runtime_error(primary_message),
          error_tok(error_tok),
          secondary_message(secondary_message.c_str()),
          fatal(fatal) {}

        ParserError(
            Token error_tok,
            char const * primary_message,
            char const * secondary_message,
            bool fatal
        )
        : std::runtime_error(primary_message),
          error_tok(error_tok),
          secondary_message(secondary_message),
          fatal(fatal) {}
        
        /**
         * Returns the primary error message.
         */
        char const * getPrimaryMessage() const {
            return what();
        }
        
        /**
         * Returns the secondary error message.
         */
        char const * getSecondaryMessage() const {
            return secondary_message;
        }

        /**
         * Returns true if we can't recover from this error.
         */
        bool isFatal() const
        {
            return fatal;
        }

    private:
        Token error_tok;                /* Where the error happened. */
        char const * secondary_message; /* Error secondary message. */
        bool fatal;                     /* Is this an error we can recover from. */
};

#endif
