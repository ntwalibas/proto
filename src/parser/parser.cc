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
#include <cstddef>
#include <utility>
#include <memory>
#include <string>

#include "ast/definitions/definition.h"
#include "ast/expressions/expression.h"
#include "ast/declarations/variable.h"
#include "ast/expressions/variable.h"
#include "ast/definitions/variable.h"
#include "ast/definitions/function.h"
#include "ast/statements/statement.h"
#include "ast/expressions/literal.h"
#include "ast/expressions/binary.h"
#include "ast/expressions/unary.h"
#include "ast/expressions/group.h"
#include "ast/expressions/array.h"
#include "ast/declarations/type.h"
#include "ast/expressions/call.h"
#include "ast/statements/block.h"
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

    // Consume irrelevant newlines before hitting the first significant token
    while (match(PROTO_NEWLINE));

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
            return parseFunctionDefinition(def_token);
        
        default: {
            return parseVariableDefinition(def_token);
        }
    }
}

std::unique_ptr<VariableDefinition>
Parser::parseVariableDefinition(Token& var_token)
{
    std::unique_ptr<TypeDeclaration> var_type = nullptr;
    try {
        var_type = parseTypeDeclaration();
    } catch (ParserError const& e) {
        if (e.getPrimaryMessage() == std::string("expected a type")) {
            throw ParserError(
                peekBack(),
                "missing type",
                "expected the type of this variable definition",
                false
            );
        }
    }

    try {
        consume(PROTO_EQUAL);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing equal sign before variable initializer",
            "expected an equal sign in order to initialize the variable definition",
            false
        );
    }

    std::unique_ptr<Expression> var_init = nullptr;
    try {
        var_init = parseExpression();
    } catch (ParserError const& e) {
        if (e.getPrimaryMessage() == std::string("missing expression")) {
            throw ParserError(
                peekBack(),
                "missing initializer expression",
                "expected an initializer expression after the equal sign [=]",
                false
            );
        }
    }

    return std::make_unique<VariableDefinition>(
        VariableDefinition(var_token, std::move(var_type), std::move(var_init))
    );
}

std::unique_ptr<FunctionDefinition>
Parser::parseFunctionDefinition(Token& fun_token)
{
    consume(PROTO_FUNCTION);
    std::unique_ptr<FunctionDefinition> fun_def =
        std::make_unique<FunctionDefinition>(fun_token);
    
    try {
        consume(PROTO_LEFT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peek(),
            "missing opening left parenthesis before function parameters",
            "expected an opening left parenthesis before function parameters",
            false
        );
    }

    do {
        // Consume extra newlines before the next parameter
        while (match(PROTO_NEWLINE));

        if (check(PROTO_RIGHT_PAREN))
            break;

        fun_def->addParameter(parseVariableDeclaration());
    } while (match(PROTO_COMMA));

    try {
        consume(PROTO_RIGHT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing opening right parenthesis after function parameters",
            "expected an opening right parenthesis after function parameters",
            false
        );
    }

    try {
        consume(PROTO_RETURN_TYPE);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing return type indicator",
            "expected the return type indicator [->] after function parameters",
            false
        );
    }

    // Show custom error for missing return type
    try {
        fun_def->setReturnType(parseTypeDeclaration());
    } catch (ParserError const& e) {
        if (e.getPrimaryMessage() == std::string("expected a type")) {
            throw ParserError(
                peekBack(),
                "missing return type",
                "expected the function return type after the return type indicator [->]",
                false
            );
        }
    }

    fun_def->setBody(parseBlockStatement());

    return fun_def;
}


// Declarations
std::unique_ptr<TypeDeclaration>
Parser::parseTypeDeclaration()
{
    bool is_const = match(PROTO_CONST);

    if (check(PROTO_IDENTIFIER)) {
        return parseSimpleTypeDeclaration(is_const);
    }
    else if (check(PROTO_LEFT_BRACKET)) {
        return parseArrayTypeDeclaration(is_const);
    }
    else {
        throw ParserError(
            peekBack(),
            "expected a type",
            "a type is either a simple type or an array type",
            false
        );
    }
}

std::unique_ptr<SimpleTypeDeclaration>
Parser::parseSimpleTypeDeclaration(bool is_const)
{
    return std::make_unique<SimpleTypeDeclaration>(is_const, consume(PROTO_IDENTIFIER));
}

std::unique_ptr<ArrayTypeDeclaration>
Parser::parseArrayTypeDeclaration(bool is_const)
{
    Token type_token;
    try {
        type_token = consume(PROTO_LEFT_BRACKET);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peek(),
            "missing left bracket",
            "expected a left bracket to declare an array type",
            false
        );
    }

    long size = 0;
    try {
        Token& size_str = consume(PROTO_INT);
        size = std::stol(size_str.getLexeme());
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peek(),
            "missing array size",
            "expected the array size",
            false
        );
    }

    try {
        consume(PROTO_RIGHT_BRACKET);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peek(),
            "missing right bracket",
            "expected a right bracket after array size",
            false
        );
    }

    bool is_simple_const = match(PROTO_CONST);
    std::unique_ptr<SimpleTypeDeclaration> simple_type = 
        parseSimpleTypeDeclaration(is_simple_const);
    
    return std::make_unique<ArrayTypeDeclaration>(
        is_const,
        type_token,
        size,
        *simple_type
    );
}

std::unique_ptr<VariableDeclaration>
Parser::parseVariableDeclaration()
{
    Token var_token;
    try {
        var_token = consume(PROTO_IDENTIFIER);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing variable declaration name",
            "expected the name of the variable being declared",
            false
        );
    }

    try {
        consume(PROTO_COLON);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing colon after variable declaration name",
            "expected a colon after variable declaration in anticipation of the type",
            false
        );
    }

    std::unique_ptr<TypeDeclaration> var_type = parseTypeDeclaration();

    return std::make_unique<VariableDeclaration>(
        var_token,
        std::move(var_type)
    );
}


// Statements
std::unique_ptr<Statement>
Parser::parseStatement()
{
    switch (peek().type) {
        case PROTO_LEFT_BRACE:
            return parseBlockStatement();
        
        // case PROTO_IF:
        //     return parseIfStatement();
        
        // case PROTO_FOR:
        //     return parseForStatement();
        
        // case PROTO_WHILE:
        //     return parseWhileStatement();
        
        // case PROTO_BREAK:
        //     return parseBreakStatement();
        
        // case PROTO_CONTINUE:
        //     return parseContinueStatement();
        
        // case PROTO_RETURN:
        //     return parseReturnStatement();
        
        default:
            return parseExpression();
    }
}

std::unique_ptr<BlockStatement>
Parser::parseBlockStatement()
{
    Token& block_token = consume(PROTO_LEFT_BRACE);
    std::unique_ptr<BlockStatement> block_stmt = 
        std::make_unique<BlockStatement>(block_token);

    // Consume extra newlines before the first definition in the block
    while (match(PROTO_NEWLINE));

    do {
        // Consume extra newlines before the next definition
        while (match(PROTO_NEWLINE));

        if (check(PROTO_RIGHT_BRACE))
            break;

        if (check(PROTO_IDENTIFIER) && checkNext(PROTO_COLON))
            block_stmt->addDefinition(parseDefinition());
        else
            block_stmt->addDefinition(parseStatement());
    } while (match(PROTO_NEWLINE));

    // Consume extra newlines after the last definition in the block
    while (match(PROTO_NEWLINE));

    try {
        consume(PROTO_RIGHT_BRACE);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing closing right brace to end the block",
            "expected a closing right brace to end the block",
            false
        );
    }

    return block_stmt;
}


// Expressions
std::unique_ptr<Expression>
Parser::parseExpression()
{
    return parseSubscriptExpression();
}

std::unique_ptr<Expression>
Parser::parseBitwiseNotExpression()
{
    if (match(PROTO_BITWISE_NOT)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> rec_expr = parseBitwiseNotExpression();
        std::unique_ptr<UnaryExpression> bitnot_expr =
            std::make_unique<UnaryExpression>(
                op_token,
                UnaryType::BitwiseNot,
                std::move(rec_expr)
            );

        return bitnot_expr;
    }

    return parseSubscriptExpression();
}

std::unique_ptr<Expression>
Parser::parseSubscriptExpression()
{
    std::unique_ptr<Expression> lvalue = parsePrimaryExpression();

    // Notice the `if` statement and not `while` statement
    // That's because we don't allow multi-dimensional arrays at the moment
    if (match(PROTO_LEFT_BRACKET)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> rvalue = parsePrimaryExpression();

        try {
            consume(PROTO_RIGHT_BRACKET);
        } catch (std::invalid_argument const& e) {
            throw ParserError(
                peekBack(),
                "missing right closing bracket in subscript expression",
                "expected a closing bracket after the subscript",
                false
            );
        }

        std::unique_ptr<BinaryExpression> sub_expr = 
            std::make_unique<BinaryExpression>(
                op_token,
                BinaryType::Subscript,
                std::move(lvalue),
                std::move(rvalue)
            );
        
        lvalue = std::move(sub_expr);
    }

    return lvalue;
}

std::unique_ptr<Expression>
Parser::parsePrimaryExpression()
{
    if (check(PROTO_LEFT_PAREN)) {
        return parseGroupExpression();
    }
    else if (check(PROTO_LEFT_BRACKET)) {
        return parseArrayExpression();
    }
    else if (check(PROTO_IDENTIFIER)) {
        if (checkNext(PROTO_LEFT_PAREN)) {
            return parseCallExpression();
        }
        else {
            return parseVariableExpression();
        }
    }
    else if (
        check(PROTO_TRUE)   ||
        check(PROTO_FALSE)  ||
        check(PROTO_INT)    ||
        check(PROTO_FLOAT)  ||
        check(PROTO_STRING)
    ) {
        return parseLiteralExpression();
    }
    else {
        throw ParserError(
            peek(),
            "missing expression",
            "expected an expression",
            false
        );
    }
}

std::unique_ptr<CallExpression>
Parser::parseCallExpression()
{
    std::unique_ptr<CallExpression> call_expr =
        std::make_unique<CallExpression>(consume(PROTO_IDENTIFIER));

    consume(PROTO_LEFT_PAREN);

    do {
        // Consume extra newlines before the next argument
        while (match(PROTO_NEWLINE));

        if (check(PROTO_RIGHT_PAREN))
            break;

        call_expr->addArgument(parseExpression());
    } while (match(PROTO_COMMA));

    try {
        consume(PROTO_RIGHT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing closing right parenthesis",
            "expected a closing parenthesis to terminate the function call",
            false
        );
    }

    return call_expr;
}

std::unique_ptr<GroupExpression>
Parser::parseGroupExpression()
{
    Token token = consume(PROTO_LEFT_PAREN);
    std::unique_ptr<Expression> expr = parseExpression();
    try {
        consume(PROTO_RIGHT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peek(),
            "missing closing right parenthesis",
            "expected a closing parenthesis to end a group expression",
            false
        );
    }

    return std::make_unique<GroupExpression>(
        token,
        std::move(expr)
    );
}

std::unique_ptr<ArrayExpression>
Parser::parseArrayExpression()
{
    Token& array_token = consume(PROTO_LEFT_BRACKET);
    std::unique_ptr<ArrayExpression> array_exp =
        std::make_unique<ArrayExpression>(array_token);

    do {
        // Consume extra newlines before the next element
        while (match(PROTO_NEWLINE));

        if (check(PROTO_RIGHT_BRACKET))
            break;

        array_exp->addContent(parsePrimaryExpression());
    } while (match(PROTO_COMMA));

    try {
        consume(PROTO_RIGHT_BRACKET);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing closing right bracket after array initializer",
            "expected a closing right bracket after initializing an array",
            false
        );
    }

    return array_exp;
}

std::unique_ptr<VariableExpression>
Parser::parseVariableExpression()
{
    return std::make_unique<VariableExpression>(consume(PROTO_IDENTIFIER));
}

std::unique_ptr<LiteralExpression>
Parser::parseLiteralExpression()
{
    if (
        match(PROTO_TRUE)   ||
        match(PROTO_FALSE)
    ) {
        return std::make_unique<LiteralExpression>(
            peekBack(),
            LiteralType::Boolean
        );
    }
    else if (match(PROTO_INT)) {
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
            "missing expression",
            "expected an expression",
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
