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

#ifndef PROTO_AST_ASSIGNMENT_EXPRESSION_H
#define PROTO_AST_ASSIGNMENT_EXPRESSION_H

#include <memory>

#include "parsetree/definitions/definition.h"
#include "common/token.h"
#include "expression.h"


enum class AssignmentType
{
    Simple,
    Iadd,
    Isub,
    Imul,
    Idiv,
    Irem,
    Ipow,
    Iand,
    Ior,
    Ixor,
    Ilshift,
    Irshift
};


class AssignmentExpression : public Expression
{
    public:
        AssignmentExpression(
            Token& token,
            enum AssignmentType type,
            std::unique_ptr<Expression>&& lvalue,
            std::unique_ptr<Expression>&& rvalue
        );

        /**
         * Returns the token associated with this assignment expression.
         */
        Token& getToken();

        /**
         * Returns the type of assignment.
         */
        enum AssignmentType getAssignmentType();

        /**
         * Returns the expression on the left side of the assignment operator.
         */
        std::unique_ptr<Expression>& getLvalue();

        /**
         * Sets the expression on the right side of the assignment expression.
         */
        void setRvalue(std::unique_ptr<Expression>&& rvalue_);

        /**
         * Returns the expression on the right side of the assignment operator.
         */
        std::unique_ptr<Expression>& getRvalue();

        /**
         * Sets the variable definition introduced by this assignment, if any.
         */
        void setVariableDefinition(std::unique_ptr<Definition>&& var_def_);

        /**
         * Returns the variable definition introduced by this assignment, if any.
         */
        std::unique_ptr<Definition>& getVariableDefinition();

        /**
         * Set the name of the function introduced by in-place assignments.
         */
        void setFunctionName(std::string const& fun_name_);

        /**
         * Returns the name of the function introduced by in-place assignments.
         */
        std::string& getFunctionName();

    protected:
        Token                       token;  /* Token associated with this assignment expression. */
        enum AssignmentType         type;   /* The type of assignment we have. */
        std::unique_ptr<Expression> lvalue; /* Expression on the left side of the assignment operator. */
        std::unique_ptr<Expression> rvalue; /* Expression on the right side of the assignment operator. */
        std::unique_ptr<Definition> var_def;/* Variable definition introduced by this assignment, if any. */
        std::string                 fun_name;/* Name of function introduced by in-place assignments. */
};

#endif
