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

#include <stdexcept>
#include <cstdbool>
#include <fstream>
#include <string>
#include <ios>

#include "utils/file.h"

/**
 * Returns true if the given file exists, false otherwise.
 */
bool
fileExists(std::string const& path)
{
    std::ifstream f(path.c_str());
    return f.good();
}


/**
 * Reads the content of the given file and returns a string with the content.
 */
std::string
readFile(std::string const& path)
{
    std::ifstream f(path.c_str());
    if (!f.good())
        throw std::invalid_argument("File [" + path + "] to read not found. Make sure to check for existence first using fileExists().");

    f.seekg(0, std::ios::end);
    std::ifstream::pos_type size = f.tellg();
    std::string buffer(size, ' ');
    f.seekg(0);
    f.read(&buffer[0], size);
    return buffer;
}
