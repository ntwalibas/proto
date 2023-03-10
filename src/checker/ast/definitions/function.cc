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

#include <memory>
#include <vector>

#include "checker/ast/declarations/variable.h"
#include "checker/ast/definitions/function.h"
#include "checker/ast/statements/statement.h"
#include "checker/ast/declarations/type.h"
#include "ast/definitions/definition.h"
#include "ast/definitions/function.h"
#include "ast/declarations/type.h"
#include "checker/checker_error.h"
#include "symbols/scope.h"


FunctionDefinitionChecker::FunctionDefinitionChecker(
    FunctionDefinition* function_def,
    std::shared_ptr<Scope> const& scope
) : function_def(function_def),
    scope(scope)
{}


/**
 * Checks that:
 * 1. All parameters are valid variable declarations, and the return type is correct.
 * 2. The function body has valid statements.
 */
void
FunctionDefinitionChecker::check(std::unique_ptr<Definition>& definition)
{
    // Check for redefinition
    if (scope->hasDefinition(function_def->getMangledName())) {
        throw CheckerError(
            function_def->getToken(),
            "function redefinition",
            "there already exists a function with signature `" +
            function_def->getMangledName() + "` in the current scope",
            false
        );
    }

    // Check the header and if we succeed, check the body
    try {
        std::shared_ptr<Scope> fun_scope = std::make_shared<Scope>(scope);
        checkHeader(fun_scope);

        // If the header checks out, we add the function to the program scope
        scope->addDefinition(
            function_def->getMangledName(),
            definition
        );

        // Check the body here
        checkBody(fun_scope);
    } catch (CheckerError& e) {
        throw;
    }
}

// Check that all parameters are valid variable declarations.
// Check that the return type is valid.
void
FunctionDefinitionChecker::checkHeader(std::shared_ptr<Scope>& fun_scope)
{
    // Parameters
    std::vector<std::unique_ptr<VariableDeclaration>>& params =
        function_def->getParameters();
    
    for (auto& param: params) {
        VariableDeclarationChecker param_checker(param);
        param_checker.check();

        // Add the variable declaration to the scope here
        fun_scope->addVariableDeclaration(param->getToken().getLexeme(), param);
    }

    // Return type
    std::unique_ptr<TypeDeclaration>& ret_type =
        function_def->getReturnType();

    TypeDeclarationChecker type_checker(ret_type);
    type_checker.check();
}

// Check that the body contains valid statements.
void
FunctionDefinitionChecker::checkBody(std::shared_ptr<Scope>& fun_scope)
{
    std::unique_ptr<BlockStatement>& body = function_def->getBody();
    body->setScope(fun_scope);
    Statement* stmt = static_cast<Statement*>(body.get());
    StatementChecker(function_def->getReturnType()).check(stmt, fun_scope);
}
