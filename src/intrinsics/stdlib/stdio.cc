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
#include <utility>
#include <cstdio>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/declarations/variable.h"
#include "cleaner/ast/definitions/function.h"
#include "cleaner/ast/declarations/type.h"
#include "intrinsics/stdlib/stdio.h"
#include "cleaner/symbols/scope.h"


static std::unique_ptr<CleanFunctionDefinition> printGenerator(
    std::string const& name,
    std::map<std::string, std::string>& params,
    std::string const& ret_type,
    std::function<std::unique_ptr<CleanExpression>(CleanScope*)> callable
)
{
    std::shared_ptr<CleanScope> print_scope =
        std::make_shared<CleanScope>(nullptr);
    
    std::unique_ptr<CleanFunctionDefinition> print_fun =
        std::make_unique<CleanFunctionDefinition>(
            name,
            print_scope
        );
    
    // Header
    for (auto&& [name, type]: params) {
        std::unique_ptr<CleanVariableDeclaration> print_param =
            std::make_unique<CleanVariableDeclaration>(
                name,
                std::make_unique<CleanSimpleTypeDeclaration>(true, type)
            );
        print_fun->parameters.push_back(std::move(print_param));
    }

    print_fun->return_type = std::make_unique<CleanSimpleTypeDeclaration>(
        true,
        ret_type
    );

    // Body
    std::shared_ptr<CleanScope> body_scope =
        std::make_shared<CleanScope>(print_scope);
    std::unique_ptr<CleanBlockStatement> body =
        std::make_unique<CleanBlockStatement>(body_scope);
    body->statements.push_back(
        std::make_unique<CleanIntrinsicExpression>(callable)
    );
    print_fun->body = std::move(body);

    return print_fun;
}

// Print booleans without newline
static std::unique_ptr<CleanFunctionDefinition> printBool()
{
    std::map<std::string, std::string> params{
        {"__param__", "bool"}
    };

    return printGenerator(
        "println(bool)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanBoolExpression* bool_expr = static_cast<CleanBoolExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            printf("%s", bool_expr->value ? "true" : "false");
            return nullptr;
        }
    );
}

// Print booleans with newline
static std::unique_ptr<CleanFunctionDefinition> printlnBool()
{
    std::map<std::string, std::string> params{
        {"__param__", "bool"}
    };

    return printGenerator(
        "println(bool)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanBoolExpression* bool_expr = static_cast<CleanBoolExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            printf("%s\n", bool_expr->value ? "true" : "false");
            return nullptr;
        }
    );
}

// Print signed int without newline
static std::unique_ptr<CleanFunctionDefinition> printInt()
{
    std::map<std::string, std::string> params{
        {"__param__", "int"}
    };

    return printGenerator(
        "print(int)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                        ->initializer.get()
            );
            printf("%" PRIi64, int_expr->value);
            return nullptr;
        }
    );
}

// Print signed int with newline
static std::unique_ptr<CleanFunctionDefinition> printlnInt()
{
    std::map<std::string, std::string> params{
        {"__param__", "int"}
    };

    return printGenerator(
        "println(int)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanSignedIntExpression* int_expr = static_cast<CleanSignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                        ->initializer.get()
            );
            printf("%" PRIi64 "\n", int_expr->value);
            return nullptr;
        }
    );
}

// Print unsigned int without newline
static std::unique_ptr<CleanFunctionDefinition> printUint()
{
    std::map<std::string, std::string> params{
        {"__param__", "uint"}
    };

    return printGenerator(
        "println(uint)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanUnsignedIntExpression* uint_expr = static_cast<CleanUnsignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            printf("%" PRIu64, uint_expr->value);
            return nullptr;
        }
    );
}

// Print unsigned int with newline
static std::unique_ptr<CleanFunctionDefinition> printlnUint()
{
    std::map<std::string, std::string> params{
        {"__param__", "uint"}
    };

    return printGenerator(
        "println(uint)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanUnsignedIntExpression* uint_expr = static_cast<CleanUnsignedIntExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            printf("%" PRIu64 "\n", uint_expr->value);
            return nullptr;
        }
    );
}

// Print float without newline
static std::unique_ptr<CleanFunctionDefinition> printFloat()
{
    std::map<std::string, std::string> params{
        {"__param__", "float"}
    };

    return printGenerator(
        "println(float)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanFloatExpression* float_expr = static_cast<CleanFloatExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            printf("%f", float_expr->value);
            return nullptr;
        }
    );
}

// Print float with newline
static std::unique_ptr<CleanFunctionDefinition> printlnFloat()
{
    std::map<std::string, std::string> params{
        {"__param__", "float"}
    };

    return printGenerator(
        "println(float)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanFloatExpression* float_expr = static_cast<CleanFloatExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            printf("%f\n", float_expr->value);
            return nullptr;
        }
    );
}

// Print string without newline
static std::unique_ptr<CleanFunctionDefinition> printString()
{
    std::map<std::string, std::string> params{
        {"__param__", "string"}
    };

    return printGenerator(
        "println(string)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanStringExpression* string_expr = static_cast<CleanStringExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            printf("%s", string_expr->value.c_str());
            return nullptr;
        }
    );
}

// Print string with newline
static std::unique_ptr<CleanFunctionDefinition> printlnString()
{
    std::map<std::string, std::string> params{
        {"__param__", "string"}
    };

    return printGenerator(
        "println(string)",
        params,
        "void",
        [](CleanScope* scope)->std::unique_ptr<CleanExpression> {
            CleanStringExpression* string_expr = static_cast<CleanStringExpression*>(
                scope->getSymbol<CleanVariableDefinition>("__param__", true)
                    ->initializer.get()
            );
            printf("%s\n", string_expr->value.c_str());
            return nullptr;
        }
    );
}

/**
 * Add all definitions in this library into the program.
 */
void
Stdio::load(CleanScope* scope)
{
    scope->addSymbol<CleanFunctionDefinition>(
        "print(bool)",
        printBool()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "println(bool)",
        printlnBool()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "print(int)",
        printInt()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "println(int)",
        printlnInt()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "print(uint)",
        printUint()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "println(uint)",
        printlnUint()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "print(float)",
        printFloat()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "println(float)",
        printlnFloat()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "print(string)",
        printString()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "println(string)",
        printlnString()
    );
}
