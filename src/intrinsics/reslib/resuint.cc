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
#include "intrinsics/reslib/resuint.h"
#include "cleaner/symbols/scope.h"
#include "utils/intrinsics.h"


// Unary positive
static std::unique_ptr<CleanFunctionDefinition> pos()
{
    std::map<std::string, std::string> params{
        {"__param__", "uint"}
    };

    return intrinsicGenerator(
        "__pos__(uint)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );

            return std::make_unique<CleanSignedIntExpression>(
                uint_expr->value
            );
        }
    );
}

// Unary negative
static std::unique_ptr<CleanFunctionDefinition> neg()
{
    std::map<std::string, std::string> params{
        {"__param__", "uint"}
    };

    return intrinsicGenerator(
        "__neg__(uint)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanSignedIntExpression>(
               (uint64_t) -uint_expr->value
            );
        }
    );
}

// Addition
static std::unique_ptr<CleanFunctionDefinition> add()
{
    std::map<std::string, std::string> params{
        {"__param1__", "uint"},
        {"__param2__", "uint"},
    };

    return intrinsicGenerator(
        "__add__(uint,uint)",
        params,
        "uint",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* uint_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanSignedIntExpression>(
                uint_expr_1->value + uint_expr_2->value
            );
        }
    );
}

// Substraction
static std::unique_ptr<CleanFunctionDefinition> sub()
{
    std::map<std::string, std::string> params{
        {"__param1__", "uint"},
        {"__param2__", "uint"},
    };

    return intrinsicGenerator(
        "__sub__(uint,uint)",
        params,
        "uint",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* uint_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanSignedIntExpression>(
                (uint64_t) (uint_expr_1->value - uint_expr_2->value)
            );
        }
    );
}

// Multiplication
static std::unique_ptr<CleanFunctionDefinition> mul()
{
    std::map<std::string, std::string> params{
        {"__param1__", "uint"},
        {"__param2__", "uint"},
    };

    return intrinsicGenerator(
        "__mul__(uint,uint)",
        params,
        "uint",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* uint_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanSignedIntExpression>(
                uint_expr_1->value * uint_expr_2->value
            );
        }
    );
}

// Division
static std::unique_ptr<CleanFunctionDefinition> div()
{
    std::map<std::string, std::string> params{
        {"__param1__", "uint"},
        {"__param2__", "uint"},
    };

    return intrinsicGenerator(
        "__div__(uint,uint)",
        params,
        "uint",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* uint_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );

            if (uint_expr_2->value == 0)
                std::abort();
            
            return std::make_unique<CleanSignedIntExpression>(
                uint_expr_1->value / uint_expr_2->value
            );
        }
    );
}

// Remainder
static std::unique_ptr<CleanFunctionDefinition> rem()
{
    std::map<std::string, std::string> params{
        {"__param1__", "uint"},
        {"__param2__", "uint"},
    };

    return intrinsicGenerator(
        "__rem__(uint,uint)",
        params,
        "uint",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* uint_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );

            if (uint_expr_2->value == 0)
                std::abort();
            
            return std::make_unique<CleanSignedIntExpression>(
                uint_expr_1->value % uint_expr_2->value
            );
        }
    );
}

// Equal
static std::unique_ptr<CleanFunctionDefinition> eq()
{
    std::map<std::string, std::string> params{
        {"__param1__", "uint"},
        {"__param2__", "uint"},
    };

    return intrinsicGenerator(
        "__eq__(uint,uint)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* uint_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                uint_expr_1->value == uint_expr_2->value
            );
        }
    );
}

// Not equal
static std::unique_ptr<CleanFunctionDefinition> ne()
{
    std::map<std::string, std::string> params{
        {"__param1__", "uint"},
        {"__param2__", "uint"},
    };

    return intrinsicGenerator(
        "__ne__(uint,uint)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* uint_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                uint_expr_1->value != uint_expr_2->value
            );
        }
    );
}

// Greater
static std::unique_ptr<CleanFunctionDefinition> gt()
{
    std::map<std::string, std::string> params{
        {"__param1__", "uint"},
        {"__param2__", "uint"},
    };

    return intrinsicGenerator(
        "__gt__(uint,uint)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* uint_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                uint_expr_1->value > uint_expr_2->value
            );
        }
    );
}

// Greater or equal
static std::unique_ptr<CleanFunctionDefinition> ge()
{
    std::map<std::string, std::string> params{
        {"__param1__", "uint"},
        {"__param2__", "uint"},
    };

    return intrinsicGenerator(
        "__ge__(uint,uint)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* uint_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                uint_expr_1->value >= uint_expr_2->value
            );
        }
    );
}

// Less
static std::unique_ptr<CleanFunctionDefinition> lt()
{
    std::map<std::string, std::string> params{
        {"__param1__", "uint"},
        {"__param2__", "uint"},
    };

    return intrinsicGenerator(
        "__lt__(uint,uint)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* uint_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                uint_expr_1->value < uint_expr_2->value
            );
        }
    );
}

// Less or equal
static std::unique_ptr<CleanFunctionDefinition> le()
{
    std::map<std::string, std::string> params{
        {"__param1__", "uint"},
        {"__param2__", "uint"},
    };

    return intrinsicGenerator(
        "__le__(uint,uint)",
        params,
        "bool",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr_1 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param1__", true)
                    ->initializer.get()
            );
            CleanSignedIntExpression* uint_expr_2 = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param2__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanBoolExpression>(
                uint_expr_1->value <= uint_expr_2->value
            );
        }
    );
}

// Bitwise not
static std::unique_ptr<CleanFunctionDefinition> bnot()
{
    std::map<std::string, std::string> params{
        {"__param__", "uint"}
    };

    return intrinsicGenerator(
        "__bnot__(uint)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* uint_expr = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            
            return std::make_unique<CleanSignedIntExpression>(
                ~uint_expr->value
            );
        }
    );
}

/**
 * Add all definitions in this library into the program.
 */
void
Resuint::load(CleanScope* scope)
{
    scope->addSymbol<CleanFunctionDefinition>(
        "__pos__(uint)",
        pos()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__neg__(uint)",
        neg()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__add__(uint,uint)",
        add()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__sub__(uint,uint)",
        sub()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__mul__(uint,uint)",
        mul()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__div__(uint,uint)",
        div()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__rem__(uint,uint)",
        rem()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__eq__(uint,uint)",
        eq()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__ne__(uint,uint)",
        ne()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__gt__(uint,uint)",
        gt()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__ge__(uint,uint)",
        ge()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__lt__(uint,uint)",
        lt()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__le__(uint,uint)",
        le()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "__bnot__(uint)",
        bnot()
    );
}
