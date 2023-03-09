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

#include "checker/ast/definitions/variable.h"
#include "checker/ast/declarations/type.h"
#include "ast/expressions/expression.h"
#include "inference/inference_error.h"
#include "ast/definitions/variable.h"
#include "ast/declarations/type.h"
#include "checker/checker_error.h"
#include "inference/inference.h"


VariableDefinitionChecker::VariableDefinitionChecker(
    VariableDefinition* variable_def,
    std::shared_ptr<Scope> const& scope
) : variable_def(variable_def),
    scope(scope)
{}


/**
 * Checking a variable definition (like all definitions) is a two-step process:
 *
 * 1. Check the header: that is the type of the variable.
 *    If this fails, the second step will not be invoked as it depends on
 *    this first step to succeed.
 *
 * 2. Check the body: this checks the initializer expression
 *    of the variable, and compares its inferred type with the one
 *    given from the first step.
 */
void
VariableDefinitionChecker::check()
{
    // Check if this variable tries to shadow a function parameter
    if (scope->hasVariableDeclaration(variable_def->getToken().getLexeme(), true)) {
        throw CheckerError(
            variable_def->getToken(),
            "variable shadows function parameter",
            "a variable cannot overshadow a function parameter, "
            "consider renaming it",
            true
        );
    }

    // Check for redefinition
    if (scope->hasDefinition(variable_def->getToken().getLexeme())) {
        throw CheckerError(
            variable_def->getToken(),
            "variable redefinition",
            "there already exists a definition (function or variable) "
            "with the same name as this variable in the current scope",
            false
        );
    }

    // Check the validity of the definition
    try {
        std::unique_ptr<TypeDeclaration>& var_type = checkHeader();
        std::unique_ptr<TypeDeclaration>& init_type = checkBody();

        if (! typeDeclarationEquals(var_type, init_type)) {
            throw CheckerError(
                variable_def->getToken(),
                "mismatched types",
                "the variable definition is declared with type [" + var_type->getTypeName() +
                "] but the initializer expression has type [" + init_type->getTypeName() + "]",
                true
            );
        }
    } catch (CheckerError const& e) {
        throw;
    }
}

// Check the type of the variable definition
std::unique_ptr<TypeDeclaration>&
VariableDefinitionChecker::checkHeader()
{
    std::unique_ptr<TypeDeclaration>& type_decl = 
        variable_def->getTypeDeclaration();
    
    TypeDeclarationChecker type_checker(type_decl);
    type_checker.check();

    return type_decl;
}

// Check the initializer expression of the variable definition
std::unique_ptr<TypeDeclaration>&
VariableDefinitionChecker::checkBody()
{
    std::unique_ptr<Expression>& var_init = variable_def->getInitializer();

    // Global variables can only be initialized by compile time constants
    if (! scope->hasParent()) {
        if (var_init->getType() != ExpressionType::Literal) {
            throw CheckerError(
                variable_def->getToken(),
                "global variable initializer must be a literal",
                "a global variable initializer cannot involve possible computations",
                false
            );
        }
    }

    try {
        return Inference(var_init.get(), scope).infer();
    } catch (InferenceError & e) {
        throw CheckerError(
            e.getToken(),
            e.getPrimaryMessage(),
            e.getSecondaryMessage(),
            e.isFatal()
        );
    }
}
