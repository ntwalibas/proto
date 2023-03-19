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
#include <utility>
#include <memory>

#include "cleaner/parsetree/expressions/expression.h"
#include "cleaner/parsetree/statements/statement.h"
#include "cleaner/parsetree/definitions/variable.h"
#include "parsetree/definitions/definition.h"
#include "cleaner/ast/statements/statement.h"
#include "cleaner/ast/statements/continue.h"
#include "parsetree/definitions/variable.h"
#include "parsetree/statements/statement.h"
#include "cleaner/ast/statements/return.h"
#include "parsetree/statements/continue.h"
#include "cleaner/ast/statements/break.h"
#include "cleaner/ast/statements/block.h"
#include "cleaner/ast/statements/while.h"
#include "parsetree/statements/return.h"
#include "parsetree/statements/while.h"
#include "parsetree/statements/block.h"
#include "parsetree/statements/break.h"
#include "cleaner/ast/statements/for.h"
#include "cleaner/ast/statements/if.h"
#include "parsetree/statements/for.h"
#include "parsetree/statements/if.h"
#include "cleaner/symbols/scope.h"


StatementCleaner::StatementCleaner()
{}

/**
 * Returns the statement AST node corresponding to the parse tree node.
 */
std::unique_ptr<CleanStatement>
StatementCleaner::clean(
    Statement* stmt,
    std::shared_ptr<CleanScope> const& scope
)
{
    switch (stmt->getType()) {
        case StatementType::Block: {
            BlockStatement* block_stmt = static_cast<BlockStatement*>(stmt);
            return cleanBlock(block_stmt, scope);
        }

        case StatementType::If: {
            IfStatement * if_stmt = static_cast<IfStatement*>(stmt);
            return cleanIf(if_stmt, scope);
        }

        case StatementType::For: {
            ForStatement* for_stmt = static_cast<ForStatement*>(stmt);
            return cleanFor(for_stmt, scope);
        }

        case StatementType::While: {
            WhileStatement* while_stmt = static_cast<WhileStatement*>(stmt);
            return cleanWhile(while_stmt, scope);
        }

        case StatementType::Break: {
            return cleanBreak();
        }

        case StatementType::Continue: {
            return cleanContinue();
        }

        case StatementType::Return: {
            ReturnStatement* return_stmt = static_cast<ReturnStatement*>(stmt);
            return cleanReturn(return_stmt, scope);
        }

        case StatementType::Expression: {
            Expression* expression_stmt = static_cast<Expression*>(stmt);
            return cleanExpression(expression_stmt, scope);
        }
        
        default:
            throw std::invalid_argument("Given statement cannot be currently cleaned.");
    }
}


// Block
std::unique_ptr<CleanBlockStatement>
StatementCleaner::cleanBlock(
    BlockStatement* block_stmt,
    std::shared_ptr<CleanScope> const& scope
)
{
    std::shared_ptr<CleanScope> block_scope =
        std::make_shared<CleanScope>(scope);
    
    std::unique_ptr<struct CleanBlockStatement> clean_block =
        std::make_unique<struct CleanBlockStatement>(block_scope);
    
    for (auto const& definition: block_stmt->getDefinitions()) {
        if (definition->getType() == DefinitionType::Variable) {
            VariableDefinition* var_def =
                static_cast<VariableDefinition*>(definition.get());
            VariableDefinitionCleaner(var_def, block_scope).clean();
        }
        else if (definition->getType() == DefinitionType::Statement) {
            Statement* stmt_def = static_cast<Statement*>(definition.get());
            clean_block->statements.push_back(
                clean(stmt_def, block_scope)
            );
        }
        else {
            throw std::invalid_argument("Unexpected definition inside a block.");
        }
    }

    return clean_block;
}


// If
std::unique_ptr<CleanIfStatement>
StatementCleaner::cleanIf(
    IfStatement* if_stmt,
    std::shared_ptr<CleanScope> const& scope
)
{
    std::unique_ptr<CleanIfStatement> clean_if =
        std::make_unique<CleanIfStatement>(
            cleanExpression(if_stmt->getCondition().get(), scope),
            cleanBlock(if_stmt->getBody().get(), scope)
        );
    
    // Handle possible elif branches
    for (auto const& elif_branch: if_stmt->getElifBranches()) {
        clean_if->elif_branches.push_back(
            std::make_unique<CleanElifBranch>(
                cleanExpression(elif_branch->getCondition().get(), scope),
                cleanBlock(elif_branch->getBody().get(), scope)
            )
        );
    }

    std::unique_ptr<ElseBranch>& else_branch = if_stmt->getElseBranch();
    if (else_branch != nullptr) {
        clean_if->else_branch = std::make_unique<CleanElseBranch>(
            cleanBlock(else_branch->getBody().get(), scope)
        );
    }

    return clean_if;
}


// For
std::unique_ptr<CleanForStatement>
StatementCleaner::cleanFor(
    ForStatement* for_stmt,
    std::shared_ptr<CleanScope> const& scope
)
{
    std::shared_ptr<CleanScope> for_scope =
        std::make_shared<CleanScope>(scope);

    std::unique_ptr<CleanExpression> clean_init = nullptr;
    std::unique_ptr<Definition>& init_clause = for_stmt->getInitClause();
    std::unique_ptr<Expression>& term_clause = for_stmt->getTermClause();
    std::unique_ptr<Expression>& incr_clause = for_stmt->getIncrClause();

    if (init_clause && init_clause->getType() == DefinitionType::Variable) {
        VariableDefinition* var_def =
            static_cast<VariableDefinition*>(init_clause.get());
        VariableDefinitionCleaner(var_def, for_scope).clean();
    }
    else if (init_clause && init_clause->getType() == DefinitionType::Statement) {
        Expression* expr_def = static_cast<Expression*>(init_clause.get());
        clean_init = cleanExpression(expr_def, for_scope);
    }

    return std::make_unique<CleanForStatement>(
        std::move(clean_init),
        term_clause ? cleanExpression(term_clause.get(), for_scope)
                    : nullptr,
        incr_clause ? cleanExpression(incr_clause.get(), for_scope)
                    : nullptr,
        cleanBlock(for_stmt->getBody().get(), for_scope),
        for_scope
    );
}


// While
std::unique_ptr<CleanWhileStatement>
StatementCleaner::cleanWhile(
    WhileStatement* while_stmt,
    std::shared_ptr<CleanScope> const& scope
)
{
    return std::make_unique<CleanWhileStatement>(
        cleanExpression(while_stmt->getCondition().get(), scope),
        cleanBlock(while_stmt->getBody().get(), scope)
    );
}


// Break
std::unique_ptr<CleanBreakStatement>
StatementCleaner::cleanBreak()
{
    return std::make_unique<CleanBreakStatement>();
}


// Contiue
std::unique_ptr<CleanStatement>
StatementCleaner::cleanContinue()
{
    return std::make_unique<CleanContinueStatement>();
}


// Return
std::unique_ptr<CleanReturnStatement>
StatementCleaner::cleanReturn(
    ReturnStatement* return_stmt,
    std::shared_ptr<CleanScope> const& scope
)
{
    return std::make_unique<CleanReturnStatement>(
        cleanExpression(return_stmt->getExpression().get(), scope)
    );
}


// Expression
std::unique_ptr<CleanExpression>
StatementCleaner::cleanExpression(
    Expression* expression_stmt,
    std::shared_ptr<CleanScope> const& scope
)
{
    return ExpressionCleaner(scope).clean(expression_stmt);
}
