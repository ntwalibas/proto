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

#ifndef PROTO_UTILS_INTRINSICS_H
#define PROTO_UTILS_INTRINSICS_H

#include <memory>
#include <string>
#include <map>

#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/definitions/function.h"


/**
 * Generate an intrinsic function
 */
std::unique_ptr<CleanFunctionDefinition> intrinsicGenerator(
    std::string const& name,
    std::map<std::string, std::string>& params,
    std::string const& ret_type,
    std::function<std::unique_ptr<CleanExpression>(CleanScope*)> callable
);

#endif
