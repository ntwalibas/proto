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

#include <cstddef>
#include <memory>
#include <vector>

#include "cleaner/parsetree/definitions/function.h"
#include "cleaner/parsetree/definitions/variable.h"
#include "cleaner/cleaner_warning.h"
#include "cleaner/symbols/scope.h"
#include "cleaner/cleaner.h"


Cleaner::Cleaner(
    Program& program
) : program(program)
{}

/**
 * Constructs a pruned AST and symbol tables without extraneous info.
 */
std::shared_ptr<CleanScope>
Cleaner::clean()
{
    std::shared_ptr<CleanScope> scope =
        std::make_shared<CleanScope>(nullptr);

    std::vector<std::unique_ptr<Definition>>& definitions =
        program.getDefinitions();

    for (auto& definition: definitions) {
        if (definition->getToken().getLexeme() != "main" && ! definition->isUsed()) {
            warnings.emplace_back(
                definition->getToken(),
                "unsed definition",
                (definition->getType() == DefinitionType::Function)
                ? "function was defined but not used"
                : "variable was defined but not used"
            );
            continue;
        }

        if (definition->getType() == DefinitionType::Function) {
            FunctionDefinition* fun_def =
                static_cast<FunctionDefinition*>(definition.get());
            FunctionDefinitionCleaner(fun_def, scope).clean();
        }
        else if (definition->getType() == DefinitionType::Variable) {
            VariableDefinition* var_def =
                static_cast<VariableDefinition*>(definition.get());
            
            VariableDefinitionCleaner(var_def, scope).clean();
        }
        else {
            throw std::runtime_error("Unexpected definition type in program.");
        }
    }

    return scope;
}
