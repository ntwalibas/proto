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

#include <iterator>
#include <cstddef>
#include <string>

#include <iostream>

#include "common/token.h"


Token::Token(
) : type(PROTO_ERROR),
    source(nullptr),
    source_path(""),
    start(source_path.end()), // ugly hack to make sure to have a valid iterator
    length(0),
    line(1),
    column(1)
{}


Token::Token(
    enum TokenType type,
    std::shared_ptr<std::string>& source,
    std::string const& source_path,
    std::string::iterator start,
    std::string::size_type length,
    std::string::size_type line,
    std::string::size_type column
) : type(type),
    source(source),
    source_path(source_path),
    start(start),
    length(length),
    line(line),
    column(column)
{}


 /**
 * Copy constructor.
 */
Token::Token(Token const& token)
{
    type = token.type;
    source = token.source;
    source_path = token.source_path;
    start = token.start;
    length = token.length;
    line = token.line;
    column = token.column;
}

/**
 * Copy assignment.
 */
Token&
Token::operator=(Token const& token)
{
    if (this != &token) {
        type = token.type;
        source = token.source;
        source_path = token.source_path;
        start = token.start;
        length = token.length;
        line = token.line;
        column = token.column;
    }

    return *this;
}

/**
 * Returns the lexeme for the given token.
 */
std::string
Token::getLexeme() const
{
    return source->substr(
        std::distance(source->begin(), start),
        length
    );
}

/**
 * Returns line information where this token was found.
 */
TokenLine
Token::getLine() const
{
    return TokenLine(* this);
}


/**
 * Returns the struct with the line where the given token was found.
 * Attached to the struct is information that points to the lexeme,
 * but within the line, not the source.
 */
TokenLine::TokenLine(
    Token const& token
) : line(""),
    offset(0)
{
    std::string::size_type length = 0;

    // Find the start of the line this token is located at
    auto line_start = token.start;
    while (line_start != token.source->begin() && * line_start != '\n') {
        line_start--;
        offset++;
    }

    // Renormalize in case we hit a newline before the current line
    if (* line_start == '\n') {
        line_start++;
        if (offset > 0)
            offset--;
    }
    
    // Calculate the length of the line
    auto line_end = line_start;
    while (line_end != token.source->end() && * line_end != '\n') {
        line_end++;
        length++;
    }

    line = token.source->substr(
        std::distance(token.source->begin(), line_start),
        length
    );
}
