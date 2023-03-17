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

#include <cstdbool>
#include <utility>
#include <memory>
#include <string>
#include <map>

#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/definitions/function.h"
#include "cleaner/ast/declarations/type.h"
#include "cleaner/ast/statements/return.h"
#include "cleaner/ast/statements/block.h"
#include "cleaner/symbols/scope.h"
#include "utils/intrinsics.h"


/**
 * Generate an intrinsic function
 */
std::unique_ptr<CleanFunctionDefinition> intrinsicGenerator(
    std::string const& name,
    std::map<std::string, std::string>& params,
    std::string const& ret_type,
    std::function<std::unique_ptr<CleanExpression>(CleanScope*)> callable
)
{
    std::shared_ptr<CleanScope> intrinsic_scope =
        std::make_shared<CleanScope>(nullptr);
    
    std::unique_ptr<CleanFunctionDefinition> intrinsic_fun =
        std::make_unique<CleanFunctionDefinition>(
            name,
            intrinsic_scope
        );
    
    // Header
    for (auto&& [name, type]: params) {
        std::unique_ptr<CleanVariableDeclaration> intrinsic_param =
            std::make_unique<CleanVariableDeclaration>(
                name,
                std::make_unique<CleanSimpleTypeDeclaration>(true, type)
            );
        intrinsic_fun->parameters.push_back(std::move(intrinsic_param));
    }

    intrinsic_fun->return_type = std::make_unique<CleanSimpleTypeDeclaration>(
        true,
        ret_type
    );

    // Body
    std::shared_ptr<CleanScope> body_scope =
        std::make_shared<CleanScope>(intrinsic_scope);
    std::unique_ptr<CleanBlockStatement> body =
        std::make_unique<CleanBlockStatement>(body_scope);
    body->statements.push_back(
        std::make_unique<CleanReturnStatement>(
            std::make_unique<CleanIntrinsicExpression>(callable)
        )
    );
    intrinsic_fun->body = std::move(body);

    return intrinsic_fun;
}
