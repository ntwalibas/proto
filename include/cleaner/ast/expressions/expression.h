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

#ifndef PROTO_AST_CLEAN_EXPRESSION_H
#define PROTO_AST_CLEAN_EXPRESSION_H

#include <cstdint>
#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "cleaner/ast/statements/statement.h"


enum class CleanExpressionType {
    Boolean,
    SignedInt,
    UnsignedInt,
    Float,
    String,
    Variable,
    Group,
    Call,
    TernaryIf,
    Assignment
};

struct CleanExpression : public CleanStatement
{
    CleanExpression(
        enum CleanExpressionType type
    ) : CleanStatement(CleanStatementType::Expression),
        type(type)
    {}

    virtual ~CleanExpression()
    {}

    enum CleanExpressionType type;
};

struct CleanBoolExpression : public CleanExpression
{
    CleanBoolExpression(
        bool value
    ) : CleanExpression(CleanExpressionType::Boolean),
        value(value)
    {}

    CleanBoolExpression(
        CleanBoolExpression const& other
    ) : CleanExpression(CleanExpressionType::Boolean),
        value(other.value)
    {}

    CleanBoolExpression(
        CleanBoolExpression const * const other
    ) : CleanExpression(CleanExpressionType::Boolean),
        value(other->value)
    {}

    CleanBoolExpression& operator=(
        CleanBoolExpression const& other
    )
    {
        if (this != &other) {
            value = other.value;
        }

        return *this;
    }

    CleanBoolExpression& operator=(
        CleanBoolExpression const * const other
    ) 
    {
        if (this != other) {
            value = other->value;
        }

        return *this;
    }

    bool value;
};

struct CleanSignedIntExpression : public CleanExpression
{
    CleanSignedIntExpression(
        int64_t value
    ) : CleanExpression(CleanExpressionType::SignedInt),
        value(value)
    {}

    CleanSignedIntExpression(
        CleanSignedIntExpression const& other
    ) : CleanExpression(CleanExpressionType::SignedInt),
        value(other.value)
    {}

    CleanSignedIntExpression(
        CleanSignedIntExpression const * const other
    ) : CleanExpression(CleanExpressionType::SignedInt),
        value(other->value)
    {}

    CleanSignedIntExpression& operator=(
        CleanSignedIntExpression const& other
    )
    {
        if (this != &other) {
            value = other.value;
        }

        return *this;
    }

    CleanSignedIntExpression& operator=(
        CleanSignedIntExpression const * const other
    ) 
    {
        if (this != other) {
            value = other->value;
        }

        return *this;
    }

    int64_t value;
};

struct CleanUnsignedIntExpression : public CleanExpression
{
    CleanUnsignedIntExpression(
        uint64_t value
    ) : CleanExpression(CleanExpressionType::UnsignedInt),
        value(value)
    {}

    CleanUnsignedIntExpression(
        CleanUnsignedIntExpression const& other
    ) : CleanExpression(CleanExpressionType::UnsignedInt),
        value(other.value)
    {}

    CleanUnsignedIntExpression(
        CleanUnsignedIntExpression const * const other
    ) : CleanExpression(CleanExpressionType::UnsignedInt),
        value(other->value)
    {}

    CleanUnsignedIntExpression& operator=(
        CleanUnsignedIntExpression const& other
    )
    {
        if (this != &other) {
            value = other.value;
        }

        return *this;
    }

    CleanUnsignedIntExpression& operator=(
        CleanUnsignedIntExpression const * const other
    ) 
    {
        if (this != other) {
            value = other->value;
        }

        return *this;
    }

    uint64_t value;
};

struct CleanFloatExpression : public CleanExpression
{
    CleanFloatExpression(
        double value
    ) : CleanExpression(CleanExpressionType::Float),
        value(value)
    {}

    CleanFloatExpression(
        CleanFloatExpression const& other
    ) : CleanExpression(CleanExpressionType::Float),
        value(other.value)
    {}

    CleanFloatExpression(
        CleanFloatExpression const * const other
    ) : CleanExpression(CleanExpressionType::Float),
        value(other->value)
    {}

    CleanFloatExpression& operator=(
        CleanFloatExpression const& other
    )
    {
        if (this != &other) {
            value = other.value;
        }

        return *this;
    }

    CleanFloatExpression& operator=(
        CleanFloatExpression const * const other
    ) 
    {
        if (this != other) {
            value = other->value;
        }

        return *this;
    }

    double value;
};

struct CleanStringExpression : public CleanExpression
{
    CleanStringExpression(
        std::string const& value
    ) : CleanExpression(CleanExpressionType::String),
        value(value)
    {}

    CleanStringExpression(
        CleanStringExpression const& other
    ) : CleanExpression(CleanExpressionType::String),
        value(other.value)
    {}

    CleanStringExpression(
        CleanStringExpression const * const other
    ) : CleanExpression(CleanExpressionType::String),
        value(other->value)
    {}

    CleanStringExpression& operator=(
        CleanStringExpression const& other
    )
    {
        if (this != &other) {
            value = other.value;
        }

        return *this;
    }

    CleanStringExpression& operator=(
        CleanStringExpression const * const other
    ) 
    {
        if (this != other) {
            value = other->value;
        }

        return * this;
    }

    std::string value;
};

struct CleanVariableExpression : public CleanExpression
{
    CleanVariableExpression(
        std::string const& var_name
    ) : CleanExpression(CleanExpressionType::Variable),
        var_name(var_name)
    {}

    std::string var_name;
};

struct CleanGroupExpression : public CleanExpression
{
    CleanGroupExpression(
        std::unique_ptr<CleanExpression> expression
    ) : CleanExpression(CleanExpressionType::Group),
        expression(std::move(expression))
    {}

    std::unique_ptr<CleanExpression> expression;
};

struct CleanCallExpression : public CleanExpression
{
    CleanCallExpression(
        std::string const& fun_name
    ) : CleanExpression(CleanExpressionType::Call),
        fun_name(fun_name)
    {}

    std::string fun_name;
    std::vector<std::unique_ptr<CleanExpression>> arguments;
};

struct CleanTernaryIfExpression : public CleanExpression
{
    CleanTernaryIfExpression(
        std::unique_ptr<CleanExpression> condition,
        std::unique_ptr<CleanExpression> then_branch,
        std::unique_ptr<CleanExpression> else_branch
    ) : CleanExpression(CleanExpressionType::TernaryIf),
        condition(std::move(condition)),
        then_branch(std::move(then_branch)),
        else_branch(std::move(else_branch))
    {}

    std::unique_ptr<CleanExpression> condition;
    std::unique_ptr<CleanExpression> then_branch;
    std::unique_ptr<CleanExpression> else_branch;
};

struct CleanAssignmentExpression : public CleanExpression
{
    CleanAssignmentExpression(
        std::unique_ptr<CleanExpression> lvalue,
        std::unique_ptr<CleanExpression> rvalue
    ) : CleanExpression(CleanExpressionType::Assignment),
        lvalue(std::move(lvalue)),
        rvalue(std::move(rvalue))
    {}

    std::unique_ptr<CleanExpression> lvalue;
    std::unique_ptr<CleanExpression> rvalue;
};

#endif
