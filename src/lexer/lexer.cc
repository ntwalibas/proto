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
#include <iterator>
#include <string>

#include "common/token_type.h"
#include "common/token.h"
#include "lexer/lexer.h"

static bool
isDigit(char c);

static bool
isAlpha(char c);

static bool
isAlphaNumeric(char c);


Lexer::Lexer(
    std::shared_ptr<std::string> const& source,
    std::string const& source_path
) : source(source),
    source_path(source_path),
    start(source->begin()),
    current(source->begin()),
    line(1),
    column(1),
    num_tokens(0)
{
    keywords["function"]    = PROTO_FUNCTION;
    keywords["const"]       = PROTO_CONST;
    keywords["if"]          = PROTO_IF;
    keywords["elif"]        = PROTO_ELIF;
    keywords["else"]        = PROTO_ELSE;
    keywords["for"]         = PROTO_FOR;
    keywords["while"]       = PROTO_WHILE;
    keywords["in"]          = PROTO_IN;
    keywords["continue"]    = PROTO_CONTINUE;
    keywords["break"]       = PROTO_BREAK;
    keywords["return"]      = PROTO_RETURN;
}


/**
 * Returns the next token in the stream with each call.
 */
Token
Lexer::lex()
{
    // Skip all whitespace before handling the next token.
    skipWhitespace();

    if (atEnd())
        return makeToken(PROTO_EOF);

    start = current;
    
    char c = advance();
    switch (c) {
        case '!':
            return makeToken(match('=') ? PROTO_NOT_EQUAL : PROTO_LOGICAL_NOT);

        case '~':
            return makeToken(PROTO_BITWISE_NOT);

        case '^':
            return makeToken(PROTO_BITWISE_XOR);

        case '+':
            return makeToken(PROTO_PLUS);

        case '-':
            if (match('>')) return makeToken(PROTO_RETURN_TYPE);
            return makeToken(PROTO_MINUS);

        case '*':
            return makeToken(match('*') ? PROTO_POW : PROTO_MUL);

        case '/':
            return makeToken(PROTO_DIV);

        case '%':
            return makeToken(PROTO_MOD);

        case '"':
            return string();

        case ',':
            return makeToken(PROTO_COMMA);

        case ';':
            return makeToken(PROTO_SEMICOLON);

        case ':':
            return makeToken(PROTO_COLON);

        case '=':
            if (match('=')) return makeToken(PROTO_EQUAL_EQUAL);
            return makeToken(PROTO_EQUAL);

        case '<':
            if (match('=')) return makeToken(PROTO_LESS_EQUAL);
            if (match('<')) return makeToken(PROTO_LEFT_SHIFT);
            if (match('>')) return makeToken(PROTO_NOT_EQUAL);
            return makeToken(PROTO_LESS);

        case '>':
            if (match('=')) return makeToken(PROTO_GREATER_EQUAL);
            if (match('>')) return makeToken(PROTO_RIGHT_SHIFT);
            return makeToken(PROTO_GREATER);

        case '|':
            return makeToken(match('|') ? PROTO_LOGICAL_OR : PROTO_BITWISE_OR);

        case '&':
            return makeToken(match('&') ? PROTO_LOGICAL_AND : PROTO_BITWISE_AND);

        case '(':
            return makeToken(PROTO_LEFT_PAREN);

        case ')':
            return makeToken(PROTO_RIGHT_PAREN);

        case '[':
            return makeToken(PROTO_LEFT_BRACKET);

        case ']':
            return makeToken(PROTO_RIGHT_PAREN);

        case '{':
            return makeToken(PROTO_LEFT_BRACE);

        case '}':
            return makeToken(PROTO_RIGHT_BRACE);

        case '_': {
            if (isAlpha(peek()))
                return identifier();
            else
                return makeToken(PROTO_ERROR);
        }
            

        case '\n': {
            while (peek() == '\n') {
                advance();
                line++;
            }
            Token token = makeToken(PROTO_NEWLINE);
            line++;
            column = 1;
            return token;
        }

        case '\t':
        case '\r':
        case ' ':
            return lex();

        default:
            if (isAlpha(c))
                return identifier();

            if (isDigit(c))
                return number();
    }

    return makeToken(PROTO_ERROR);
}


/**
 * Lex a number.
 */
Token
Lexer::number()
{
    bool is_decimal = false;

    while (isDigit(peek()))
        advance();

    // Match the fractional part if any
    if (peek() == '.' && isDigit(peekFront())) {
        // Consume the decimal point
        advance();

        // Mark the current lexeme as being decimal
        is_decimal = true;

        while (isDigit(peek()))
            advance();
    }

    if (is_decimal == false)
        return makeToken(PROTO_INT);
    else
        return makeToken(PROTO_FLOAT);
}


/**
 * Lex a string.
 */
Token
Lexer::string()
{
    char previous_char = peek();
    while ((peek() != '"' || (peek() == '"' && previous_char == '\\')) && !atEnd()) {
        previous_char = advance();
        if(previous_char == '\n')
            throw LexerError("new lines are not allowed inside strings", makeToken(PROTO_ERROR));
    }

    if (atEnd())
        throw LexerError("unterminated string", makeToken(PROTO_ERROR));

    // make sure we close with a double quote
    if(peek() != '"')
        throw LexerError("expected a double quotation mark to close a string", makeToken(PROTO_ERROR));
    else
        advance();
    
    Token string_token = makeToken(PROTO_STRING);
    
    // trim the surrounding quotes around the string
    string_token.start++;
    string_token.length -= 2;

    return string_token;
}


/**
 * Lex an identifier.
 */
Token
Lexer::identifier()
{
    while(isAlphaNumeric(peek()))
        advance();
    
    std::string identifier = source->substr(
        std::distance(source->begin(), start),
        current - start
    );

    try {
        enum TokenType type = keywords.at(identifier);
        return makeToken(type);
    } catch (std::out_of_range e) {
        return makeToken(PROTO_IDENTIFIER);
    }
}


/**
 * Handle whitespace, this including comments.
 */
void
Lexer::skipWhitespace()
{
    for (;;) {
        char c = peek();
        switch (c) {
            // We handle generic whitespace
            case '\r':
            case '\t':
            case ' ':
                advance();
                break;

            // We treat comments as whitespace and handle them here
            case '/': {
                bool issue_new_line = true;
                // We don't issue a new line if the comment started the file,
                // or it was already preceeded by a new line
                if (peekBack() == '\n' || num_tokens == 0)
                    issue_new_line = false;

                if (peekFront() == '/') {
                    skipSingleComment(issue_new_line);
                }
                else if (peekFront() == '*') {
                    // since we are sure we have a multi line comment,
                    // we consume the MUL token in order to avoid clashing with nested comments
                    advance();
                    skipMultiComment(issue_new_line);
                }
                else {
                    goto continue_skipping;
                }
                break;
            }

            default:
                goto continue_skipping;
        }
    }

continue_skipping:
    // If the last token before skipping whitespace characters was a new line and the next is a new line, we ignore the fresh new line
    while (peekBack() == '\n' && peek() == '\n') {
        advance();
        line++;
    }
}

void
Lexer::skipSingleComment(bool issue_new_line)
{
    while (peek() != '\n' && atEnd() == false)
        advance();
    
    if (! issue_new_line) {
        advance();
        line++;
        column = 1;
    }
}

void
Lexer::skipMultiComment(bool issue_new_line)
{
    std::string::size_type levels = 0;
    std::string::size_type start_line = line;
    bool terminated = false;

    while (atEnd() == false) {
        // if we have nested comments
        if (peek() == '/' && peekFront() == '*') {
            levels++;
        }
        
        // We handle comment closing
        if (peek() == '*' && peekFront() == '/') {
            // we make sure to consume the closing characters because the advance() at the end of the loop cannot
            advance();
            advance();

            if (levels == 0) {
                terminated = true;
                break;
            }
            else {
                levels--;
            }
        }
        
        // Gracefully handle newlines inside comments
        if (peek() == '\n') {
            line++;
            column = 1;
        }

        advance();
    }

    if (atEnd() && terminated == false)
        throw LexerError(
            std::string("unterminated multiline comment starting at line ") + std::to_string(start_line),
            makeToken(PROTO_ERROR)
        );
    
    if (! issue_new_line) {
        advance();
        line++;
        column = 1;
    }
}


/**
 * Utilities to navigate through the stream.
 */
// Returns true if we are at the beginning of the source.
bool
Lexer::atStart()
{
    return current == source->begin();
}

// Returns true if we are at the end of the source.
bool
Lexer::atEnd()
{
    return current == source->end();
}

// Returns current char in the stream and advance to the next.
char
Lexer::advance()
{
    current++;
    column++;
    return * (current - 1);
}

// Return true if current character matches given one,
// Advance to the next character if match.
bool
Lexer::match(char expected)
{
    if (atEnd())
        return false;

    if (* current != expected)
        return false;

    current++;
    column++;

    return true;
}

// Returns next character in the stream.
char
Lexer::peekFront()
{
    return * (current + 1);
}

// Returns current character in the stream.
char
Lexer::peek()
{
    return * current;
}

// Returns the previous character in the stream.
char
Lexer::peekBack()
{
    return * (current + 1);
}

// Create a token.
Token
Lexer::makeToken(enum TokenType type)
{
    return Token(
        type,
        source,
        source_path,
        start,
        static_cast<std::string::size_type>(current - start),
        line,
        column
    );
}


/**
 * Returns true if the given character is a digit.
 */
static bool
isDigit(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}


/**
 * Returns true if the given character is a letter or underscore.
 */
static bool
isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}


/**
 * Returns true if we have a digit or a letter or an underscore.
 */
static bool
isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}
