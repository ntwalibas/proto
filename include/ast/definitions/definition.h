/*  This file is part of the Nova programming language
 * 
 *  Copyright (c) 2022- Ntwali Bashige Toussaint
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

#ifndef PROTO_AST_DEFINITION_H
#define PROTO_AST_DEFINITION_H

enum class DefinitionType {
    Function,
    Variable
};


class Definition
{
    public:
        Definition(enum DefinitionType type) : type(type) {}

        /**
         * Returns the type of this definition.
         */
        enum DefinitionType getType() const
        {
            return type;
        }

    protected:
        enum DefinitionType type;     /* The type of definition of the derived class. */
};

#endif
