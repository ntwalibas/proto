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

#include <utility>
#include <memory>

#include "cleaner/parsetree/declarations/variable.h"
#include "cleaner/parsetree/definitions/function.h"
#include "cleaner/parsetree/statements/statement.h"
#include "cleaner/parsetree/declarations/type.h"
#include "cleaner/ast/definitions/function.h"
#include "cleaner/ast/definitions/variable.h"
#include "parsetree/definitions/definition.h"
#include "parsetree/definitions/function.h"
#include "parsetree/statements/block.h"
#include "cleaner/symbols/scope.h"


FunctionDefinitionCleaner::FunctionDefinitionCleaner
(
    FunctionDefinition* function_def,
    std::shared_ptr<CleanScope> const& scope
) : function_def(function_def),
    scope(scope)
{}

/**
 * Produces a function AST for its parsetree
 */
void
FunctionDefinitionCleaner::clean()
{
    std::string fun_name = function_def->getMangledName();
    std::shared_ptr<CleanScope> fun_scope =
        std::make_shared<CleanScope>(scope);
    
    std::unique_ptr<CleanFunctionDefinition> clean_fun =
        std::make_unique<CleanFunctionDefinition>(
            fun_name,
            fun_scope
        );
    
    cleanHeader(clean_fun);
    cleanBody(clean_fun);

    scope->addSymbol<CleanFunctionDefinition>(
        fun_name,
        std::move(clean_fun)
    );
}

// Clean the function's parameters and return type
void
FunctionDefinitionCleaner::cleanHeader(
    std::unique_ptr<CleanFunctionDefinition>& clean_fun
)
{
    // Handle parameters:
    // - Create parameters to be used during code generation
    // - Generate incomplete variable definitions in the scope for interpretation
    for (auto const& parameter : function_def->getParameters()) {
        std::string clean_param_name = parameter->getToken().getLexeme();

        // Variable declarations to be used during code generation
        clean_fun->parameters.push_back(
            VariableDeclarationCleaner(parameter.get()).clean()
        );

        // Variable definitions to be used during interpretation
        clean_fun->scope->addSymbol<CleanVariableDefinition>(
            clean_param_name,
            std::make_unique<CleanVariableDefinition>(
                clean_param_name,
                TypeDeclarationCleaner(parameter->getTypeDeclaration().get()).clean(),
                nullptr
            )
        );
    }

    // Generate a clean return type declaration
    clean_fun->return_type = TypeDeclarationCleaner(
        function_def->getReturnType().get()
    ).clean();
}

// Clean the function body
void
FunctionDefinitionCleaner::cleanBody(
    std::unique_ptr<CleanFunctionDefinition>& clean_fun
)
{
    clean_fun->body = StatementCleaner().cleanBlock(
        function_def->getBody().get(),
        clean_fun->scope
    );
}
