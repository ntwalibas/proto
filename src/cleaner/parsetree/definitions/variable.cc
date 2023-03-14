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
#include <string>

#include "cleaner/parsetree/expressions/expression.h"
#include "cleaner/parsetree/definitions/variable.h"
#include "cleaner/parsetree/declarations/type.h"
#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/definitions/variable.h"
#include "parsetree/definitions/variable.h"
#include "cleaner/ast/declarations/type.h"
#include "cleaner/symbols/scope.h"


VariableDefinitionCleaner::VariableDefinitionCleaner
(
    VariableDefinition* variable_def,
    std::shared_ptr<CleanScope> const& scope
) : variable_def(variable_def),
    scope(scope)
{}

/**
 * Creates a variable definition on the AST from the parse tree.
 */
void
VariableDefinitionCleaner::clean()
{
    std::string var_name = variable_def->getToken().getLexeme();
    scope->addSymbol<CleanVariableDefinition>(
        var_name,
        std::make_unique<CleanVariableDefinition>(
            var_name,
            cleanHeader(),
            cleanBody() 
        )
    );
}

// Clean the type of the variable definition
std::unique_ptr<CleanTypeDeclaration>
VariableDefinitionCleaner::cleanHeader()
{
    return TypeDeclarationCleaner(
        variable_def->getTypeDeclaration().get()
    ).clean();
}

// Clean the initializer expression of the variable definition
std::unique_ptr<CleanExpression>
VariableDefinitionCleaner::cleanBody()
{
    return ExpressionCleaner(scope).clean(
        variable_def->getInitializer().get()
    );
}
