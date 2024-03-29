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

#include <functional>
#include <cinttypes>
#include <cstdbool>
#include <cstddef>
#include <cstdlib>
#include <utility>
#include <cstdio>
#include <memory>
#include <string>
#include <map>

#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/declarations/variable.h"
#include "cleaner/ast/definitions/function.h"
#include "intrinsics/reslib/resint.h"
#include "cleaner/symbols/scope.h"
#include "utils/intrinsics.h"


// Unary positive
static std::unique_ptr<CleanFunctionDefinition> pos()
{
    std::map<std::string, std::string> params{
        {"__param__", "int"}
    };

    return intrinsicGenerator(
        "__pos__(int)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );

            return std::make_unique<CleanSignedIntExpression>(
                int_expr->value
            );
        }
    );
}

// Unary negative
static std::unique_ptr<CleanFunctionDefinition> neg()
{
    std::map<std::string, std::string> params{
        {"__param__", "int"}
    };

    return intrinsicGenerator(
        "__neg__(int)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanSignedIntExpression>(
                -int_expr->value
            );
        }
    );
}

// Addition
static std::unique_ptr<CleanFunctionDefinition> add()
{
    std::map<std::string, std::string> params{
        {"__param1__", "int"},
        {"__param2__", "int"},
    };

    return intrinsicGenerator(
        "__add__(int,int)",
        params,
        "int",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* int_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanSignedIntExpression>(
                int_expr_1->value + int_expr_2->value
            );
        }
    );
}

// Substraction
static std::unique_ptr<CleanFunctionDefinition> sub()
{
    std::map<std::string, std::string> params{
        {"__param1__", "int"},
        {"__param2__", "int"},
    };

    return intrinsicGenerator(
        "__sub__(int,int)",
        params,
        "int",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* int_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanSignedIntExpression>(
                int_expr_1->value - int_expr_2->value
            );
        }
    );
}

// Multiplication
static std::unique_ptr<CleanFunctionDefinition> mul()
{
    std::map<std::string, std::string> params{
        {"__param1__", "int"},
        {"__param2__", "int"},
    };

    return intrinsicGenerator(
        "__mul__(int,int)",
        params,
        "int",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* int_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanSignedIntExpression>(
                int_expr_1->value * int_expr_2->value
            );
        }
    );
}

// Division
static std::unique_ptr<CleanFunctionDefinition> div()
{
    std::map<std::string, std::string> params{
        {"__param1__", "int"},
        {"__param2__", "int"},
    };

    return intrinsicGenerator(
        "__div__(int,int)",
        params,
        "int",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* int_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );

            if (int_expr_2->value == 0)
                std::abort();
            
            return std::make_unique<CleanSignedIntExpression>(
                int_expr_1->value / int_expr_2->value
            );
        }
    );
}

// Remainder
static std::unique_ptr<CleanFunctionDefinition> rem()
{
    std::map<std::string, std::string> params{
        {"__param1__", "int"},
        {"__param2__", "int"},
    };

    return intrinsicGenerator(
        "__rem__(int,int)",
        params,
        "int",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* int_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );

            if (int_expr_2->value == 0)
                std::abort();
            
            return std::make_unique<CleanSignedIntExpression>(
                int_expr_1->value % int_expr_2->value
            );
        }
    );
}

// Equal
static std::unique_ptr<CleanFunctionDefinition> eq()
{
    std::map<std::string, std::string> params{
        {"__param1__", "int"},
        {"__param2__", "int"},
    };

    return intrinsicGenerator(
        "__eq__(int,int)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* int_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                int_expr_1->value == int_expr_2->value
            );
        }
    );
}

// Not equal
static std::unique_ptr<CleanFunctionDefinition> ne()
{
    std::map<std::string, std::string> params{
        {"__param1__", "int"},
        {"__param2__", "int"},
    };

    return intrinsicGenerator(
        "__ne__(int,int)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* int_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                int_expr_1->value != int_expr_2->value
            );
        }
    );
}

// Greater
static std::unique_ptr<CleanFunctionDefinition> gt()
{
    std::map<std::string, std::string> params{
        {"__param1__", "int"},
        {"__param2__", "int"},
    };

    return intrinsicGenerator(
        "__gt__(int,int)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* int_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                int_expr_1->value > int_expr_2->value
            );
        }
    );
}

// Greater or equal
static std::unique_ptr<CleanFunctionDefinition> ge()
{
    std::map<std::string, std::string> params{
        {"__param1__", "int"},
        {"__param2__", "int"},
    };

    return intrinsicGenerator(
        "__ge__(int,int)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* int_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                int_expr_1->value >= int_expr_2->value
            );
        }
    );
}

// Less
static std::unique_ptr<CleanFunctionDefinition> lt()
{
    std::map<std::string, std::string> params{
        {"__param1__", "int"},
        {"__param2__", "int"},
    };

    return intrinsicGenerator(
        "__lt__(int,int)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* int_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                int_expr_1->value < int_expr_2->value
            );
        }
    );
}

// Less or equal
static std::unique_ptr<CleanFunctionDefinition> le()
{
    std::map<std::string, std::string> params{
        {"__param1__", "int"},
        {"__param2__", "int"},
    };

    return intrinsicGenerator(
        "__le__(int,int)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* int_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                int_expr_1->value <= int_expr_2->value
            );
        }
    );
}

// Bitwise not
static std::unique_ptr<CleanFunctionDefinition> bnot()
{
    std::map<std::string, std::string> params{
        {"__param__", "int"}
    };

    return intrinsicGenerator(
        "__bnot__(int)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanSignedIntExpression>(
                ~int_expr->value
            );
        }
    );
}

/**
 * Add all definitions in this library into the program.
 */
void
Resint::load(CleanScope* scope)
{
    scope->addSymbol<CleanFunctionDefinition>(
        "__pos__(int)",
        pos()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__neg__(int)",
        neg()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__add__(int,int)",
        add()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__sub__(int,int)",
        sub()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__mul__(int,int)",
        mul()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__div__(int,int)",
        div()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__rem__(int,int)",
        rem()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__eq__(int,int)",
        eq()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__ne__(int,int)",
        ne()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__gt__(int,int)",
        gt()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__ge__(int,int)",
        ge()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__lt__(int,int)",
        lt()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__le__(int,int)",
        le()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__bnot__(int)",
        bnot()
    );
}
