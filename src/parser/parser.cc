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
#include "parser/parser.h"
#include "utils/parser.h"
#include "common/token.h"
#include "ast/program.h"
#include "lexer/lexer.h"


Parser::Parser(
    Lexer& lexer
) : lexer(lexer),
    previous(
        PROTO_ERROR,
        lexer.getSource(),
        lexer.getSourcePath(),
        lexer.getStart(),
        0,
        0,
        0
    ),
    current(lexer.lex()),
    next(lexer.lex())
{}


/**
 * Parses the token stream and returns an AST representing the program.
 */
Program
Parser::parse()
{
    return parseProgram();
}


Program
Parser::parseProgram()
{
    Program program;

    if (atEnd()) {
        throw ParserError(
            peek(),
            std::string("a program must have at least one definition"),
            std::string(""),
            true
        );
    }

    return program;
}


// Returns the token that comes before the one currently being parsed.
inline Token&
Parser::peekBack()
{
    return previous;
}


// Returns the current token.
inline Token&
Parser::peek()
{
    return current;
}


// Returns the token that comes after the one currently being parsed.
inline Token&
Parser::peekFront()
{
    return next;
}


// Returns true if we are at the end of the token stream.
inline bool
Parser::atEnd()
{
    return current.type == PROTO_EOF;
}


// Returns true if we are one token away from the end of the token stream.
inline bool
Parser::pastEnd()
{
    return next.type == PROTO_EOF;
}


// Returns true if the previous token type matches the given type.
inline bool
Parser::checkPrevious(enum TokenType type)
{
    return previous.type == type;
}


// Returns true if the current token type matches the given type.
inline bool
Parser::check(enum TokenType type)
{
    return current.type == type;
}


// Returns true if the next token type matches the given type.
inline bool
Parser::checkNext(enum TokenType type)
{
    return next.type == type;
}


// Advances in the token stream and returns the current token.
Token&
Parser::advance()
{
    Token new_next = lexer.lex();

    previous = current;
    current = next;
    next = new_next;

    return previous;
}

// Returns true if the current token type matches the given token type.
// Advances to the next token only if the match was true.
bool
Parser::match(enum TokenType type)
{
    if (check(type)) {
        advance();
        return true;
    }

    return false;
}


// If the current token is of the given type, return it and advance.
// Throws an invalid argument exception otherwise.
Token&
Parser::consume(enum TokenType type)
{
    if (check(type) == true)
        return advance();

    throw std::invalid_argument("The current token is not of the given type.");
}


// Create a token.
Token
Parser::makeToken(enum TokenType type) {
    return Token(
        type,
        current.source,
        current.source_path,
        current.start,
        current.length,
        current.line,
        current.column
    );
}
