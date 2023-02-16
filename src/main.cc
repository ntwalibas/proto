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

#include "ansi_colors.h"
#include "utils/file.h"


int
compile(std::string const& source_path);


int
main(int argc, char const * argv[])
{
    if (argc != 2) {
        std::cout << "Usage: nova program" << std::endl;
    }
    else {
        return compile(std::string(argv[1]));
    }

    return 0;
}

int
compile(std::string const& source_path)
{
    /* We begin by making sure the given source path exists */
    if (fileExists(source_path) == false) {
        std::cerr << ANSI_BRIGHT_BOLD_RED "error" ANSI_COLOR_RESET
                ": file [" ANSI_RED << source_path << ANSI_COLOR_RESET
                "] was not found." << std::endl;
        return 1;
    }

    /* 2. Invoke the compiler */
    std::string source = readFile(source_path);
    std::cout << source << std::endl;

    return 0;
}
