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

#include <stdexcept>
#include <memory>
#include <vector>
#include <string>

#include "checker/ast/definitions/variable.h"
#include "checker/ast/statements/statement.h"
#include "ast/definitions/definition.h"
#include "checker/checker_error.h"
#include "ast/statements/block.h"
#include "symbols/symtable.h"
#include "symbols/scope.h"


StatementChecker::StatementChecker(
    Statement* stmt,
    std::shared_ptr<Scope> const& scope
) : stmt(stmt),
    scope(scope)
{}


/**
 * Checks that any given statement obeys the language semantics
 */
void
StatementChecker::check()
{
    switch (stmt->getType()) {
        case StatementType::Block:
            checkBlock();
            break;
        
        // case StatementType::If:
        //     checkIf();
        //     break;
        
        // case StatementType::For:
        //     checkFor();
        //     break;
        
        // case StatementType::While:
        //     checkWhile();
        //     break;
        
        // case StatementType::Break:
        //     checkBreak();
        //     break;
        
        // case StatementType::Continue:
        //     checkContinue();
        //     break;
        
        // case StatementType::Return:
        //     checkReturn();
        //     break;
        
        // case StatementType::Expression:
        //     checkExpression();
        //     break;
        
        default:
            throw std::invalid_argument("Given statement cannot be currently checked.");
    }
}

// Block
void
StatementChecker::checkBlock()
{
    BlockStatement* block_stmt = static_cast<BlockStatement*>(stmt);
    std::vector<std::unique_ptr<Definition>>& definitions =
        block_stmt->getDefinitions();

    for (auto& definition: definitions) {
        if (definition->getType() == DefinitionType::Variable) {
            VariableDefinition* var_def =
                static_cast<VariableDefinition*>(definition.get());
            VariableDefinitionChecker(var_def, block_stmt->getScope()).check();
        }
        else if (definition->getType() == DefinitionType::Statement) {
            Statement* stmt_def = static_cast<Statement*>(definition.get());
            StatementChecker(stmt_def, block_stmt->getScope());
        }
        else {
            throw CheckerError(
                definition->getToken(),
                "unexpected definition inside block",
                std::string("only variable and statement definitions ") +
                "can occur within block statements",
                true
            );
        }
    }
}