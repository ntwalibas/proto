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

#include <exception>
#include <utility>
#include <memory>
#include <string>

#include "ast/definitions/definition.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/variable.h"
#include "ast/definitions/variable.h"
#include "ast/expressions/literal.h"
#include "ast/declarations/type.h"
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
            "a program must have at least one definition",
            "",
            true
        );
    }

    while (! atEnd()) {
        try {
            program.addDefinition(parseDefinition());
            while (match(PROTO_NEWLINE));
        } catch (ParserError const& e) {
            if (e.isFatal())
                throw;
            
            errors.push_back(e);
            synchronize();
        }
    }

    return program;
}

// Definitions
std::unique_ptr<Definition>
Parser::parseDefinition()
{
    Token def_token;
    try {
        def_token = consume(PROTO_IDENTIFIER);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peek(),
            "failed to parse the beginning of a definition",
            "expected an identifier",
            true
        );
    }

    try {
        consume(PROTO_COLON);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peek(),
            "missing colon after definition name",
            "expected a colon after this definition name",
            true
        );
    }

    // Depending on the token that follows,
    // we decide if we have a function or a variable definition
    switch (peek().type) {
        case PROTO_FUNCTION:
            throw ParserError(
                peek(),
                "function definitions are not currently supported",
                "unexpected function definition",
                true
            );
        
        default: {
            return parseVariableDefinition(def_token);
        }
    }
}

std::unique_ptr<VariableDefinition>
Parser::parseVariableDefinition(Token& var_token)
{
    std::unique_ptr<TypeDeclaration> var_type = parseTypeDeclaration();

    try {
        consume(PROTO_EQUAL);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peek(),
            "missing equal sign before variable initializer",
            "expected an equal sign in order to initialize the variable definition",
            false
        );
    }

    std::unique_ptr<Expression> var_init = parseExpression();

    return std::make_unique<VariableDefinition>(
        VariableDefinition(var_token, std::move(var_type), std::move(var_init))
    );
}


// Declarations
std::unique_ptr<TypeDeclaration>
Parser::parseTypeDeclaration()
{
    bool is_const = match(PROTO_CONST);

    if (check(PROTO_IDENTIFIER)) {
        return parseSimpleTypeDeclaration(is_const);
    }
    else {
        throw ParserError(
            peek(),
            "expected a type",
            "a type is either a simple type or an array type",
            false
        );
    }
}

std::unique_ptr<SimpleTypeDeclaration>
Parser::parseSimpleTypeDeclaration(bool is_const)
{
    try {
        Token& type_token = consume(PROTO_IDENTIFIER);
        return std::make_unique<SimpleTypeDeclaration>(is_const, type_token);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peek(),
            "expected a type",
            "type name missing",
            false
        );
    }
}


// Expressions
std::unique_ptr<Expression>
Parser::parseExpression()
{
    return parsePrimaryExpression();
}

std::unique_ptr<Expression>
Parser::parsePrimaryExpression()
{
    if (check(PROTO_IDENTIFIER)) {
        return parseVariableExpression();
    }
    else if(
        check(PROTO_INT)    ||
        check(PROTO_FLOAT)  ||
        check(PROTO_STRING)
    ) {
        return parseLiteralExpression();
    }
    else {
        throw ParserError(
            peek(),
            "expected a primary expression: variable or literal",
            "",
            false
        );
    }
}

std::unique_ptr<VariableExpression>
Parser::parseVariableExpression()
{
    return std::make_unique<VariableExpression>(consume(PROTO_IDENTIFIER));
}

std::unique_ptr<LiteralExpression>
Parser::parseLiteralExpression()
{
    if (match(PROTO_INT)) {
        return std::make_unique<LiteralExpression>(
            peekBack(),
            LiteralType::Integer
        );
    }
    else if (match(PROTO_FLOAT)) {
        return std::make_unique<LiteralExpression>(
            peekBack(),
            LiteralType::Float
        );
    }
    else if (match(PROTO_STRING)) {
        return std::make_unique<LiteralExpression>(
            peekBack(),
            LiteralType::String
        );
    }
    else {
        throw ParserError(
            peek(),
            "expected a literal expression: integer, float or string",
            "",
            false
        );
    }
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


// Synchronize the parser so we can consume more tokens even under failure.
void
Parser::synchronize()
{
    advance();
    
    while (! atEnd()) {
        switch(peek().type) {
            case PROTO_IDENTIFIER:
                if (checkNext(PROTO_COLON))
                    return;
            
            // Avoid a compiler warning about unhandled token types
            default:
                break;
        }

        advance();
    }
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
