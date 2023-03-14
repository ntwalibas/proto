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

#ifndef PROTO_AST_CLEAN_TYPE_DECLARATION_H
#define PROTO_AST_CLEAN_TYPE_DECLARATION_H

#include <cstdbool>
#include <string>


enum class CleanTypeCategory
{
    Simple
};

struct CleanTypeDeclaration
{
    CleanTypeDeclaration(enum CleanTypeCategory category)
      : category(category)
    {}

    virtual ~CleanTypeDeclaration()
    {}

    enum CleanTypeCategory category;
};


struct CleanSimpleTypeDeclaration : public CleanTypeDeclaration
{
    CleanSimpleTypeDeclaration(
        bool is_const,
        std::string const& name
    ) : CleanTypeDeclaration(CleanTypeCategory::Simple),
        is_const(is_const),
        name(name)
    {}

    bool is_const;
    std::string name;
};

#endif
