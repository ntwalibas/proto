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
#include <string>

#include "common/token.h"


Token::Token(
    enum TokenType type,
    const std::shared_ptr<std::string>& source,
    const std::string& source_path,
    const std::string::iterator start,
    const std::string::size_type length,
    const std::string::size_type line,
    const std::string::size_type column
) : type(type),
source(source),
source_path(source_path),
start(start),
length(length),
line(line),
column(column) {}


/**
 * Returns the lexeme for the given token.
 */
std::string Token::getLexeme() const
{
    return source->substr(
        std::distance(source->begin(), start),
        length
    );
}

/**
 * Returns line information where this token was found.
 */
TokenLine Token::getLine()
{
    return TokenLine(* this);
}


/**
 * Returns the struct with the line where the given token was found.
 * Attached to the struct is information that points to the lexeme,
 * but within the line, not the source.
 */
TokenLine::TokenLine(Token const& token)
{
    std::string::size_type length = 0, offset = 0;

    // Find the start of the line this token is located at
    auto line_start = token.start;
    while (line_start != token.source->begin() && * line_start != '\n') {
        line_start--;
        offset++;
    }

    // Renormalize in case we hit a newline before the current line
    if (* line_start == '\n') {
        line_start++;
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
