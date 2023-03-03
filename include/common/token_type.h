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

#ifndef PROTO_COMMON_TOKEN_TYPE_H
#define PROTO_COMMON_TOKEN_TYPE_H

#include <string>


enum TokenType {
    /* single-character tokens */
    PROTO_LOGICAL_NOT,        // !  (not)
    PROTO_BITWISE_NOT,        // ~  (bnot)
    PROTO_BITWISE_OR,         // |  (bor)
    PROTO_BITWISE_AND,        // &  (band)
    PROTO_BITWISE_XOR,        // ^  (xor)
    PROTO_LEFT_PAREN,         // (
    PROTO_RIGHT_PAREN,        // )
    PROTO_LEFT_BRACE,         // {
    PROTO_RIGHT_BRACE,        // }
    PROTO_LEFT_BRACKET,       // [
    PROTO_RIGHT_BRACKET,      // ]
    PROTO_PLUS,               // +
    PROTO_DIV,                // /
    PROTO_REM,                // %
    PROTO_COMMA,              // ,
    PROTO_SEMICOLON,          // ;
    PROTO_COLON,              // :
    PROTO_QUESTION,           // ?
    PROTO_NEWLINE,            // \n

    /* one or two character tokens */
    PROTO_EQUAL,              // =
    PROTO_IADD,               // +=
    PROTO_ISUB,               // -=
    PROTO_IMUL,               // *=
    PROTO_IDIV,               // /=
    PROTO_IREM,               // %=
    PROTO_IPOW,               // **=
    PROTO_IAND,               // &=
    PROTO_IOR,                // |=
    PROTO_IXOR,               // ^=
    PROTO_ILSHIFT,            // <<=
    PROTO_IRSHIFT,            // >>=
    PROTO_EQUAL_EQUAL,        // ==
    PROTO_NOT_EQUAL,          // !=
    PROTO_GREATER,            // >
    PROTO_GREATER_EQUAL,      // >=
    PROTO_LESS,               // <
    PROTO_LESS_EQUAL,         // <=
    PROTO_BRANCH,             // <>
    PROTO_MINUS,              // -
    PROTO_RETURN_TYPE,        // ->
    PROTO_LOGICAL_OR,         // || (or)
    PROTO_LOGICAL_AND,        // && (and)
    PROTO_LEFT_SHIFT,         // << (lsh)
    PROTO_RIGHT_SHIFT,        // >> (rsh)
    PROTO_MUL,                // *
    PROTO_POW,                // **

    /* literals */
    PROTO_IDENTIFIER,         // variable, function or type name
    PROTO_INT,                // integers
    PROTO_FLOAT,              // floating point
    PROTO_STRING,             // string

    /* keywords */
    PROTO_TRUE,               // boolean True
    PROTO_FALSE,              // boolean False
    PROTO_FUNCTION,           // function
    PROTO_CONST,              // const
    PROTO_IF,                 // if
    PROTO_ELIF,               // else if
    PROTO_ELSE,               // else
    PROTO_FOR,                // for
    PROTO_WHILE,              // while
    PROTO_CONTINUE,           // continue
    PROTO_BREAK,              // break
    PROTO_RETURN,             // return

    /* specials */
    PROTO_EOF,                // end file
    PROTO_ERROR               // token representing a lexer error
};


/**
 * Returns the string representation of a token type
 *
 * @param       type the token type to get the string representation of.
 *
 * @return      the string representation of a token type.
 */
std::string tokenTypeToString(enum TokenType type);

#endif
