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

#ifndef PROTO_AST_CLEAN_FUNCTION_DEFINITION_H
#define PROTO_AST_CLEAN_FUNCTION_DEFINITION_H

#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include <stack>

#include "cleaner/ast/declarations/variable.h"
#include "cleaner/ast/definitions/variable.h"
#include "cleaner/symbols/scope.forward.h"
#include "cleaner/ast/declarations/type.h"
#include "cleaner/ast/statements/block.h"


struct CleanFunctionDefinition
{
    CleanFunctionDefinition(
        std::string const& name,
        std::shared_ptr<CleanScope>& scope
    ) : name(name),
        scope(scope),
        return_type(nullptr),
        body(nullptr)
    {}

    std::string name;
    std::shared_ptr<CleanScope> scope;
    std::vector<
        std::unique_ptr<CleanVariableDeclaration>> parameters;
    std::unique_ptr<CleanTypeDeclaration> return_type;
    std::unique_ptr<CleanBlockStatement> body;
    std::stack<std::vector<std::unique_ptr<CleanVariableDefinition>>> stack_frame;
};

#endif
