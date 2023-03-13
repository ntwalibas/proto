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

#ifndef PROTO_AST_CALL_EXPRESSION_H
#define PROTO_AST_CALL_EXPRESSION_H

#include <memory>
#include <vector>
#include <string>

#include "common/token.h"
#include "expression.h"


class CallExpression : public Expression
{
    public:
        CallExpression(Token& token);

        /**
         * Returns the token associated with this function call expression.
         */
        Token& getToken();
        
        /**
         * Add an argument to this function call.
         */
        void addArgument(std::unique_ptr<Expression>&& argument);

        /**
         * Returns the arguments passed to the function in this call.
         */
        std::vector<std::unique_ptr<Expression>>& getArguments();

        /**
         * Set the name of the function that corresponds to this call.
         */
        void setFunctionName(std::string const& fun_name_);

        /**
         * Returns the name of the function that corresponds to this call.
         */
        std::string& getFunctionName();

    protected:
        Token                                       token;      /* Token associated with this array. */
        std::vector<std::unique_ptr<Expression>>    arguments;  /* Function call arguments. */
        std::string                                 fun_name;   /* Name of the function that corresponds to this call. */
};

#endif
