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

#ifndef PROTO_AST_CAST_EXPRESSION_H
#define PROTO_AST_CAST_EXPRESSION_H

#include <memory>
#include <string>

#include "parsetree/declarations/type.h"
#include "common/token.h"
#include "expression.h"


class CastExpression : public Expression
{
    public:
        CastExpression(
            Token& token,
            std::unique_ptr<Expression>&& expression,
            std::unique_ptr<TypeDeclaration>&& dest_type
        );

        /**
         * Returns the token associated with this cast expression.
         */
        Token& getToken();

        /**
         * Returns the expression to be cast to the given type.
         */
        std::unique_ptr<Expression>& getExpression();

        /**
         * Returns the type the expression is to be cast to.
         */
        std::unique_ptr<TypeDeclaration>& getTypeDeclaration();

        /**
         * Set the name of the function that corresponds to this cast.
         */
        void setFunctionName(std::string const& fun_name_);

        /**
         * Returns the name of the function that corresponds to this cast.
         */
        std::string& getFunctionName();

    protected:
        Token                       token;      /* Token associated with this cast expression. */
        std::unique_ptr<Expression> expression; /* Expression the cast operator is applied to. */
        std::unique_ptr<TypeDeclaration>
                                    dest_type;  /* Type the expression is to be cast to. */
        std::string                 fun_name;   /* Name of the function that corresponds to this cast. */
};

#endif
