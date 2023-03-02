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
#include "ast/expressions/assignment.h"
#include "ast/declarations/variable.h"
#include "ast/expressions/variable.h"
#include "ast/definitions/variable.h"
#include "ast/definitions/function.h"
#include "ast/statements/statement.h"
#include "ast/statements/continue.h"
#include "ast/expressions/literal.h"
#include "ast/expressions/binary.h"
#include "ast/expressions/unary.h"
#include "ast/expressions/group.h"
#include "ast/declarations/type.h"
#include "ast/statements/return.h"
#include "ast/statements/break.h"
#include "ast/statements/while.h"
#include "ast/expressions/call.h"
#include "ast/statements/block.h"
#include "ast/statements/for.h"
#include "ast/statements/if.h"
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
            
            // Require a newline after each definition
            if (! match(PROTO_NEWLINE) && !atEnd()) {
                throw ParserError(
                    peekBack(),
                    "missing newline",
                    "expected a new line after definition",
                    true
                );
            }

            // Consume superfluous new lines if any
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

    // Allow newlines before closing parenthesis
    while (match(PROTO_NEWLINE));

    try {
        consume(PROTO_RIGHT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing closing right parenthesis after function parameters",
            "expected a closing right parenthesis after function parameters",
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

    // Allow newlines before function body
    while (match(PROTO_NEWLINE));

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
    else {
        throw ParserError(
            peekBack(),
            "expected a type",
            "expected a type declaration",
            false
        );
    }
}

std::unique_ptr<SimpleTypeDeclaration>
Parser::parseSimpleTypeDeclaration(bool is_const)
{
    return std::make_unique<SimpleTypeDeclaration>(is_const, consume(PROTO_IDENTIFIER));
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
        
        case PROTO_IF:
            return parseIfStatement();
        
        case PROTO_FOR:
            return parseForStatement();
        
        case PROTO_WHILE:
            return parseWhileStatement();
        
        case PROTO_CONTINUE:
            return parseContinueStatement();
        
        case PROTO_BREAK:
            return parseBreakStatement();
        
        case PROTO_RETURN:
            return parseReturnStatement();
        
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

std::unique_ptr<IfStatement>
Parser::parseIfStatement()
{
    Token if_token = consume(PROTO_IF);

    try {
        consume(PROTO_LEFT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing left opening parenthesis",
            "expected a left opening parenthesis before if condition",
            false
        );
    }

    while (match(PROTO_NEWLINE));
    std::unique_ptr<Expression> cond_expr = parseExpression();
    while (match(PROTO_NEWLINE));

    try {
        consume(PROTO_RIGHT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing right closing parenthesis",
            "expected right closing parenthesis after if condition",
            false
        );
    }

    // Consume possible newlines after condition, before body
    while (match(PROTO_NEWLINE));

    std::unique_ptr<IfStatement> if_stmt = std::make_unique<IfStatement>(
        if_token,
        std::move(cond_expr),
        parseBlockStatement()
    );

    // Consume possible newlines before `elif` header, after `if` body
    // But we only do so if the next two tokens are both newlines
    // We do this because newlines are the separators between statements
    // so we don't want to consume the `if` end-of-statement marker
    while (checkNext(PROTO_NEWLINE) && match(PROTO_NEWLINE));

    // We parse `elif` branches
    while (
        (check(PROTO_NEWLINE) && checkNext(PROTO_ELIF)) ||
        check(PROTO_ELIF)
    ) {
        // We consume the current newline, if any
        match(PROTO_NEWLINE);

        if_stmt->addElifBranch(parseElifBranch());

        // We continue consuming newline, but not the end-of-statement marker
        while (checkNext(PROTO_NEWLINE) && match(PROTO_NEWLINE));
    }

    // Consume newlines without consuming the end-of-statement marker
    while (checkNext(PROTO_NEWLINE) && match(PROTO_NEWLINE));

    // We parse `else` branch
    while (
        (check(PROTO_NEWLINE) && checkNext(PROTO_ELSE)) ||
        check(PROTO_ELSE)
    ) {
        // We consume the current newline, if any
        match(PROTO_NEWLINE);

        if_stmt->setElseBranch(parseElseBranch());

        // We continue consuming newline, but not the end-of-statement marker
        while (checkNext(PROTO_NEWLINE) && match(PROTO_NEWLINE));
    }

    return if_stmt;
}

std::unique_ptr<ElifBranch>
Parser::parseElifBranch()
{
    Token elif_token = consume(PROTO_ELIF);

    try {
        consume(PROTO_LEFT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing left opening parenthesis",
            "expected a left opening parenthesis before elif condition",
            false
        );
    }

    while (match(PROTO_NEWLINE));
    std::unique_ptr<Expression> cond_expr = parseExpression();
    while (match(PROTO_NEWLINE));

    try {
        consume(PROTO_RIGHT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing right closing parenthesis",
            "expected right closing parenthesis after elif condition",
            false
        );
    }

    // Consume possible newlines after condition, before body
    while (match(PROTO_NEWLINE));

    return std::make_unique<ElifBranch>(
        elif_token,
        std::move(cond_expr),
        parseBlockStatement()
    );
}

std::unique_ptr<ElseBranch>
Parser::parseElseBranch()
{
    Token else_token = consume(PROTO_ELSE);

    // Consume possible newlines before the body
    while (match(PROTO_NEWLINE));

    return std::make_unique<ElseBranch>(
        else_token,
        parseBlockStatement()
    );
}

std::unique_ptr<ForStatement>
Parser::parseForStatement()
{
    Token for_token = consume(PROTO_FOR);

    try {
        consume(PROTO_LEFT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing left opening parenthesis",
            "expected a left opening parenthesis before loop condition",
            false
        );
    }
    
    // Initialization clause
    std::unique_ptr<Definition> init_clause = nullptr;
    while (match(PROTO_NEWLINE));
    if (! match(PROTO_SEMICOLON)) {
        if (check(PROTO_IDENTIFIER) && checkNext(PROTO_COLON))
            init_clause = parseDefinition();
        else
            init_clause = parseExpression();

        while (match(PROTO_NEWLINE));

        try {
            consume(PROTO_SEMICOLON);
        } catch (std::invalid_argument const& e) {
            throw ParserError(
                peekBack(),
                "missing semicolon after for loop initialization clause",
                "expected a semicolon after initialization clause",
                false
            );
        }
    }
    
    // Termination clause
    std::unique_ptr<Expression> term_clause = nullptr;
    while (match(PROTO_NEWLINE));
    if (! match(PROTO_SEMICOLON)) {
        term_clause = parseExpression();
        while (match(PROTO_NEWLINE));

        try {
            consume(PROTO_SEMICOLON);
        } catch (std::invalid_argument const& e) {
            throw ParserError(
                peekBack(),
                "missing semicolon after for loop termination clause",
                "expected a semicolon after termination clause",
                false
            );
        }
    }
    
    // Increment clause
    std::unique_ptr<Expression> incr_clause = nullptr;
    while (match(PROTO_NEWLINE));
    if (! check(PROTO_RIGHT_PAREN)) {
        incr_clause = parseExpression();
        while (match(PROTO_NEWLINE));
    }

    try {
        consume(PROTO_RIGHT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing right closing parenthesis",
            "expected right closing parenthesis after loop increment clause",
            false
        );
    }

    // Consume possible newlines after loop header, before body
    while (match(PROTO_NEWLINE));

    return std::make_unique<ForStatement>(
        for_token,
        std::move(init_clause),
        std::move(term_clause),
        std::move(incr_clause),
        parseBlockStatement()
    );
}

std::unique_ptr<WhileStatement>
Parser::parseWhileStatement()
{
    Token wh_token = consume(PROTO_WHILE);

    try {
        consume(PROTO_LEFT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing left opening parenthesis",
            "expected a left opening parenthesis before loop condition",
            false
        );
    }

    while (match(PROTO_NEWLINE));
    std::unique_ptr<Expression> cond_expr = parseExpression();
    while (match(PROTO_NEWLINE));

    try {
        consume(PROTO_RIGHT_PAREN);
    } catch (std::invalid_argument const& e) {
        throw ParserError(
            peekBack(),
            "missing right closing parenthesis",
            "expected right closing parenthesis after loop condition",
            false
        );
    }

    // Consume possible newlines after condition, before body
    while (match(PROTO_NEWLINE));

    return std::make_unique<WhileStatement>(
        wh_token,
        std::move(cond_expr),
        parseBlockStatement()
    );
}

std::unique_ptr<ContinueStatement>
Parser::parseContinueStatement()
{
    return std::make_unique<ContinueStatement>(
        consume(PROTO_CONTINUE)
    );
}

std::unique_ptr<BreakStatement>
Parser::parseBreakStatement()
{
    return std::make_unique<BreakStatement>(
        consume(PROTO_BREAK)
    );
}

std::unique_ptr<ReturnStatement>
Parser::parseReturnStatement()
{
    Token ret_token = consume(PROTO_RETURN);
    return std::make_unique<ReturnStatement>(
        ret_token,
        parseExpression()
    );
}


// Expressions
std::unique_ptr<Expression>
Parser::parseExpression()
{
    return parseAssignmentExpression();
}

std::unique_ptr<Expression>
Parser::parseAssignmentExpression()
{
    std::unique_ptr<Expression> lvalue = parseLogicalOrExpression();

    while (match(PROTO_EQUAL)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> rvalue = parseAssignmentExpression();
        std::unique_ptr<Expression> assign_expr =
            std::make_unique<AssignmentExpression>(
                op_token,
                std::move(lvalue),
                std::move(rvalue)
            );

        lvalue = std::move(assign_expr);
    }

    return lvalue;
}

std::unique_ptr<Expression>
Parser::parseLogicalOrExpression()
{
    std::unique_ptr<Expression> left = parseLogicalAndExpression();

    while (match(PROTO_LOGICAL_OR)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> right = parseLogicalAndExpression();
        std::unique_ptr<Expression> logicor_expr =
            std::make_unique<BinaryExpression>(
                op_token,
                BinaryType::LogicalOr,
                std::move(left),
                std::move(right)
            );

        left = std::move(logicor_expr);
    }

    return left;
}

std::unique_ptr<Expression>
Parser::parseLogicalAndExpression()
{
    std::unique_ptr<Expression> left = parseComparisonExpression();

    while (match(PROTO_LOGICAL_AND)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> right = parseComparisonExpression();
        std::unique_ptr<Expression> logicand_expr =
            std::make_unique<BinaryExpression>(
                op_token,
                BinaryType::LogicalAnd,
                std::move(left),
                std::move(right)
            );

        left = std::move(logicand_expr);
    }

    return left;
}

std::unique_ptr<Expression>
Parser::parseComparisonExpression()
{
    std::unique_ptr<Expression> left = parseBitwiseOrExpression();

    while (
        match(PROTO_EQUAL_EQUAL)    ||
        match(PROTO_NOT_EQUAL)      ||
        match(PROTO_LESS)           ||
        match(PROTO_LESS_EQUAL)     ||
        match(PROTO_GREATER)        ||
        match(PROTO_GREATER_EQUAL)
    ) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> right = parseBitwiseOrExpression();
        std::unique_ptr<Expression> comp_expr = nullptr;
        switch (op_token.type) {
            case PROTO_EQUAL_EQUAL:
                comp_expr = std::make_unique<BinaryExpression>(
                    op_token,
                    BinaryType::Equal,
                    std::move(left),
                    std::move(right)
                );
                break;

            case PROTO_NOT_EQUAL:
                comp_expr = std::make_unique<BinaryExpression>(
                    op_token,
                    BinaryType::NotEqual,
                    std::move(left),
                    std::move(right)
                );
                break;

            case PROTO_LESS:
                comp_expr = std::make_unique<BinaryExpression>(
                    op_token,
                    BinaryType::Less,
                    std::move(left),
                    std::move(right)
                );
                break;

            case PROTO_LESS_EQUAL:
                comp_expr =  std::make_unique<BinaryExpression>(
                    op_token,
                    BinaryType::LessOrEqual,
                    std::move(left),
                    std::move(right)
                );
                break;

            case PROTO_GREATER:
                comp_expr =  std::make_unique<BinaryExpression>(
                    op_token,
                    BinaryType::Greater,
                    std::move(left),
                    std::move(right)
                );
                break;

            case PROTO_GREATER_EQUAL:
                comp_expr =  std::make_unique<BinaryExpression>(
                    op_token,
                    BinaryType::GreaterOrEqual,
                    std::move(left),
                    std::move(right)
                );
                break;
            
            default:;
        }

        left = std::move(comp_expr);
    }

    return left;
}

std::unique_ptr<Expression>
Parser::parseBitwiseOrExpression()
{
    std::unique_ptr<Expression> left = parseBitwiseXorExpression();

    while (match(PROTO_BITWISE_OR)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> right = parseBitwiseXorExpression();
        std::unique_ptr<Expression> bitor_expr =
            std::make_unique<BinaryExpression>(
                op_token,
                BinaryType::BitwiseOr,
                std::move(left),
                std::move(right)
            );

        left = std::move(bitor_expr);
    }

    return left;
}

std::unique_ptr<Expression>
Parser::parseBitwiseXorExpression()
{
    std::unique_ptr<Expression> left = parseBitwiseAndExpression();

    while (match(PROTO_BITWISE_XOR)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> right = parseBitwiseAndExpression();
        std::unique_ptr<Expression> bitxor_expr =
            std::make_unique<BinaryExpression>(
                op_token,
                BinaryType::BitwiseXor,
                std::move(left),
                std::move(right)
            );

        left = std::move(bitxor_expr);
    }

    return left;
}

std::unique_ptr<Expression>
Parser::parseBitwiseAndExpression()
{
    std::unique_ptr<Expression> left = parseBitshiftExpression();

    while (match(PROTO_BITWISE_AND)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> right = parseBitshiftExpression();
        std::unique_ptr<Expression> bitand_expr =
            std::make_unique<BinaryExpression>(
                op_token,
                BinaryType::BitwiseAnd,
                std::move(left),
                std::move(right)
            );

        left = std::move(bitand_expr);
    }

    return left;
}

std::unique_ptr<Expression>
Parser::parseBitshiftExpression()
{
    std::unique_ptr<Expression> left = parseTermExpression();

    while (match(PROTO_LEFT_SHIFT) || match(PROTO_RIGHT_SHIFT)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> right = parseTermExpression();
        std::unique_ptr<Expression> shift_expr =
            std::make_unique<BinaryExpression>(
                op_token,
                (op_token.type == PROTO_LEFT_SHIFT) ?
                    BinaryType::LeftShift : BinaryType::RightShift,
                std::move(left),
                std::move(right)
            );

        left = std::move(shift_expr);
    }

    return left;
}

std::unique_ptr<Expression>
Parser::parseTermExpression()
{
    std::unique_ptr<Expression> left = parseFactorExpression();

    while (match(PROTO_PLUS) || match(PROTO_MINUS)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> right = parseFactorExpression();
        std::unique_ptr<Expression> term_expr =
            std::make_unique<BinaryExpression>(
                op_token,
                (op_token.type == PROTO_PLUS) ?
                    BinaryType::Plus : BinaryType::Minus,
                std::move(left),
                std::move(right)
            );

        left = std::move(term_expr);
    }

    return left;
}

std::unique_ptr<Expression>
Parser::parseFactorExpression()
{
    std::unique_ptr<Expression> left = parseSignExpression();

    while (
        match(PROTO_MUL)    ||
        match(PROTO_DIV)    ||
        match(PROTO_REM)    ||
        match(PROTO_POW)
    ) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> right = parseSignExpression();
        std::unique_ptr<Expression> fact_expr = nullptr;
        switch (op_token.type) {
            case PROTO_MUL:
                fact_expr = std::make_unique<BinaryExpression>(
                    op_token,
                    BinaryType::Mul,
                    std::move(left),
                    std::move(right)
                );
                break;

            case PROTO_DIV:
                fact_expr = std::make_unique<BinaryExpression>(
                    op_token,
                    BinaryType::Div,
                    std::move(left),
                    std::move(right)
                );
                break;

            case PROTO_REM:
                fact_expr = std::make_unique<BinaryExpression>(
                    op_token,
                    BinaryType::Rem,
                    std::move(left),
                    std::move(right)
                );
                break;

            case PROTO_POW:
                fact_expr =  std::make_unique<BinaryExpression>(
                    op_token,
                    BinaryType::Pow,
                    std::move(left),
                    std::move(right)
                );
                break;
            
            default:;
        }

        left = std::move(fact_expr);
    }

    return left;
}

std::unique_ptr<Expression>
Parser::parseSignExpression()
{
    if (match(PROTO_PLUS) || match(PROTO_MINUS)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> rec_expr = parseSignExpression();
        return std::make_unique<UnaryExpression>(
            op_token,
            (op_token.type == PROTO_PLUS) ? UnaryType::Plus : UnaryType::Minus,
            std::move(rec_expr)
        );
    }

    return parseNotExpression();
}

std::unique_ptr<Expression>
Parser::parseNotExpression()
{
    if (match(PROTO_BITWISE_NOT) || match(PROTO_LOGICAL_NOT)) {
        Token op_token = peekBack();
        std::unique_ptr<Expression> rec_expr = parseNotExpression();
        return std::make_unique<UnaryExpression>(
            op_token,
            (op_token.type == PROTO_BITWISE_NOT) ? 
                UnaryType::BitwiseNot : UnaryType::LogicalNot,
            std::move(rec_expr)
        );
    }

    return parsePrimaryExpression();
}

std::unique_ptr<Expression>
Parser::parsePrimaryExpression()
{
    if (check(PROTO_LEFT_PAREN)) {
        return parseGroupExpression();
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

    // Consume extra newlines before closing the call
    while (match(PROTO_NEWLINE));

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
            case PROTO_IF:
            case PROTO_FOR:
            case PROTO_WHILE:
                return;
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
