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

#include <iostream>
#include <string>

#include "include/ansi_colors.h"
#include "common/token.h"
#include "utils/parser.h"

/**
 * Given a token and an error message, display that error in a visually appealing way.
 */
void
printError(
   Token const& token,
    std::string const& primary_message,
    std::string const& secondary_message,
    std::string const& file_path
)
{
    // if (token -> type == PROTO_EOF || token -> type == PROTO_ERROR) {
    //     printf(
    //         ANSI_BRIGHT_BOLD_RED "error " ANSI_COLOR_RESET
    //         ANSI_BRIGHT_BOLD_WHITE "[%s]: %s.\n" ANSI_COLOR_RESET,
    //         file_path,
    //         primary_message
    //     );
    //     return;
    // }

    // struct TokenLine token_line = getTokenLine(token);
    // printf(
    //     ANSI_BRIGHT_BOLD_RED "error " ANSI_COLOR_RESET
    //     ANSI_BRIGHT_BOLD_WHITE "[%s:%d:%d]: %s:\n" ANSI_COLOR_RESET,
    //     file_path,
    //     token -> line,
    //     token -> column,
    //     primary_message
    // );
    // printf("%*s|\n", 6, "");
    // printf("%5u", token -> line);
    // printf("%2s%-4s", "|", "");
    // printf("%s\n", token_line.line);
    // printf("%*s|", 6, "");
    // printf("%*s", (int) (token_line.offset + 4), "");
    // for (unsigned i = 0; i < token -> length - 1; i++)
    //     printf(ANSI_COLOR_GREEN "~" ANSI_COLOR_RESET);
    // printf(ANSI_COLOR_GREEN "^" ANSI_COLOR_RESET);
    // if (secondary_message != NULL)
    //     printf(ANSI_COLOR_GREEN " %s." ANSI_COLOR_RESET, secondary_message);
    // printf("\n\n");
    // free(token_line.line);
}
