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

#include <cstdio>
#include <string>

#include "include/ansi_colors.h"
#include "utils/messages.h"
#include "common/token.h"

/**
 * Given a token and an error message, display that error in a visually appealing way.
 * We stick to using C-style formatted output due to iomap being hard to use.
 */
void
printMessage(
    std::string const& title,
    Token const& token,
    std::string const& primary_message,
    std::string const& secondary_message,
    std::string const& source_path
)
{
    if (token.type == PROTO_EOF || token.type == PROTO_ERROR) {
        if (title == "error") {
            fprintf(
                stderr,
                ANSI_BRIGHT_BOLD_RED "error " ANSI_COLOR_RESET
                "[%s]: " ANSI_BRIGHT_BOLD_WHITE "%s.\n" ANSI_COLOR_RESET,
                source_path.c_str(),
                primary_message.c_str()
            );
        }
        else if (title == "warning") {
            fprintf(
                stderr,
                ANSI_BRIGHT_BOLD_MAGNETA "warning " ANSI_COLOR_RESET
                "[%s]: " ANSI_BRIGHT_BOLD_WHITE "%s.\n" ANSI_COLOR_RESET,
                source_path.c_str(),
                primary_message.c_str()
            );
        }
        else {
            // We will add notes here
        }
        return;
    }

    TokenLine token_line = token.getLine();
    if (title == "error") {
        fprintf(
            stderr,
            ANSI_BRIGHT_BOLD_RED "error " ANSI_COLOR_RESET
            "[%s:%zu:%zu]: " ANSI_BRIGHT_BOLD_WHITE "%s:\n" ANSI_COLOR_RESET,
            source_path.c_str(),
            token.line,
            token.column,
            primary_message.c_str()
        );
    }
    else if (title == "warning") {
        fprintf(
            stderr,
            ANSI_BRIGHT_BOLD_MAGNETA "warning " ANSI_COLOR_RESET
            "[%s:%zu:%zu]: " ANSI_BRIGHT_BOLD_WHITE "%s:\n" ANSI_COLOR_RESET,
            source_path.c_str(),
            token.line,
            token.column,
            primary_message.c_str()
        );
    }
    else {
        // We will add notes here
    }

    fprintf(stderr, "%*s|\n", 6, "");
    fprintf(stderr, "%5zu", token.line);
    fprintf(stderr, "%2s%-4s", "|", "");
    fprintf(stderr, "%s\n", token_line.line.c_str());
    fprintf(stderr, "%*s|", 6, "");
    fprintf(stderr, "%*s", static_cast<int>(token_line.offset) + 4, "");
    for (std::string::size_type i = 0; i < token.length - 1; i++) {
        if (title == "error") {
            fprintf(stderr, ANSI_GREEN "~" ANSI_COLOR_RESET);
        }
        else if (title == "warning") {
            fprintf(stderr, ANSI_BLUE "~" ANSI_COLOR_RESET);
        }
        else {
            // We will add notes here
        }
    }
    if (title == "error") {
        fprintf(stderr, ANSI_GREEN "^" ANSI_COLOR_RESET);
    }
    else if (title == "warning") {
        fprintf(stderr, ANSI_BLUE "^" ANSI_COLOR_RESET);
    }
    else {
        // We will add notes here
    }

    if (! secondary_message.empty()) {
        if (title == "error") {
            fprintf(stderr, ANSI_GREEN " %s." ANSI_COLOR_RESET, secondary_message.c_str());
        }
        else if (title == "warning") {
            fprintf(stderr, ANSI_BLUE " %s." ANSI_COLOR_RESET, secondary_message.c_str());
        }
        else {
            // We will add notes here
        }
    }

    fprintf(stderr, "\n\n");
}
