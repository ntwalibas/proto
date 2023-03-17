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

/**
 * Add all definitions in this library into the program.
 */
void
Stdio::load(CleanScope* scope)
{
    scope->addSymbol<CleanFunctionDefinition>(
        "print(int)",
        printInt()
    );
    scope->addSymbol<CleanFunctionDefinition>(
        "println(int)",
        printlnInt()
    );
}
