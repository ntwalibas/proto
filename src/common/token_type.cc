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
#include <string>

#include "common/token_type.h"

/**
 * Returns the string representation of a token type
 *
 * @param       type the token type to get the string form of.
 *
 * @return      the string representation of a token type.
 */
std::string
tokenTypeToString(enum TokenType type)
{
    char const * token_types[] = {
        "LOGICAL_NOT",          // !  (not)
        "BITWISE_NOT",          // ~  (bnot)
        "BITWISE_OR",           // |  (bor)
        "BITWISE_AND",          // &  (band)
        "BITWISE_XOR",          // ^  (xor)
        "LEFT_PAREN",           // (
        "RIGHT_PAREN",          // )
        "LEFT_BRACE",           // {
        "RIGHT_BRACE",          // }
        "LEFT_BRACKET",         // [
        "RIGHT_BRACKET",        // ]
        "PLUS",                 // +
        "DIV",                  // /
        "REM",                  // %
        "COMMA",                // ,
        "SEMICOLON",            // ;
        "COLON",                // :
        "QUESTION",             // ?
        "NEWLINE",              // \n

        /* one or two character tokens */
        "EQUAL",                // =
        "IADD",                 // +=
        "ISUB",                 // -=
        "IMUL",                 // *=
        "IDIV",                 // /=
        "IREM",                 // %=
        "IPOW",                 // **=
        "IAND",                 // &=
        "IOR",                  // |=
        "IXOR",                 // ^=
        "ILSHIFT",              // <<=
        "IRSHIFT",              // >>=
        "EQUAL_EQUAL",          // ==
        "NOT_EQUAL",            // !=
        "GREATER",              // >
        "GREATER_EQUAL",        // >=
        "LESS",                 // <
        "LESS_EQUAL",           // <=
        "LESS_BRANCH",          // <>
        "MINUS",                // -
        "RETURN_TYPE",          // ->
        "LOGICAL_OR",           // || (or)
        "LOGICAL_AND",          // && (and)
        "LEFT_SHIFT",           // << (lsh)
        "RIGHT_SHIFT",          // >> (rsh)
        "MUL",                  // *
        "POW",                  // **

        /* literals */
        "IDENTIFIER",           // variable, function or type name
        "INT",                  // integers
        "FLOAT",                // floating point
        "STRING",               // string

        /* keywords */
        "TRUE",                 // boolean True
        "FALSE",                // boolean False
        "FUNCTION",             // function
        "CONST",                // const
        "IF",                   // if
        "ELIF",                 // else if
        "ELSE",                 // else
        "FOR",                  // for
        "WHILE",                // while
        "CONTINUE",             // continue
        "BREAK",                // break
        "RETURN",               // return

        /* specials */
        "EOF",                  // end file
        "ERROR"                 // token representing a lexer error
    };

    return std::string(token_types[(int) type]);
}
