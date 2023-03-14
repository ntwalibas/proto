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

#ifndef PROTO_AST_CLEAN_VARIABLE_DEFINITION_H
#define PROTO_AST_CLEAN_VARIABLE_DEFINITION_H

#include <utility>
#include <memory>
#include <string>

#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/declarations/type.h"


struct CleanVariableDefinition
{
    CleanVariableDefinition(
        std::string const& name,
        std::unique_ptr<CleanTypeDeclaration>&& type,
        std::unique_ptr<CleanExpression>&& initializer
    ) : name(name),
        type(std::move(type)),
        initializer(std::move(initializer))
    {}

    std::string name;
    std::unique_ptr<CleanTypeDeclaration> type;
    std::unique_ptr<CleanExpression> initializer;
};

#endif
