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
#include <cstdbool>
#include <memory>

#include "interpreter/ast/expressions/expression.h"
#include "interpreter/ast/statements/statement.h"
#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/statements/statement.h"
#include "cleaner/ast/statements/continue.h"
#include "cleaner/ast/statements/return.h"
#include "cleaner/ast/statements/while.h"
#include "cleaner/ast/statements/break.h"
#include "cleaner/ast/statements/block.h"
#include "cleaner/ast/statements/for.h"
#include "cleaner/ast/statements/if.h"
#include "cleaner/symbols/scope.h"


StatementInterpreter::StatementInterpreter(
) : returned(false),
    broke(false),
    continued(false)
{}

/**
 * Interprets the given statement.
 */
std::unique_ptr<CleanExpression>
StatementInterpreter::interpret(
    CleanStatement* stmt,
    CleanScope* scope
)
{
    switch (stmt->type) {
        case CleanStatementType::Block: {
            return interpretBlock(
                static_cast<CleanBlockStatement*>(stmt)
            );
        }

        case CleanStatementType::If: {
            return interpretIf(
                static_cast<CleanIfStatement*>(stmt), scope
            );
        }

        case CleanStatementType::For: {
            return interpretFor(
                static_cast<CleanForStatement*>(stmt), scope
            );
        }

        case CleanStatementType::While: {
            return interpretWhile(
                static_cast<CleanWhileStatement*>(stmt), scope
            );
        }

        case CleanStatementType::Break: {
            return interpretBreak(
                static_cast<CleanBreakStatement*>(stmt)
            );
        }

        case CleanStatementType::Continue: {
            return interpretContinue(
                static_cast<CleanContinueStatement*>(stmt)
            );
        }

        case CleanStatementType::Return: {
            return interpretReturn(
                static_cast<CleanReturnStatement*>(stmt), scope
            );
        }

        case CleanStatementType::Expression: {
            return interpretExpression(
                static_cast<CleanExpression*>(stmt), scope
            );
        }

        default:
            throw std::runtime_error(
                "Statement iterpretation failed: unknow statement type."
            );
    }

    throw std::runtime_error(
        "Statement iterpretation failed: statement switch failed."
    );
}

// Block
std::unique_ptr<CleanExpression>
StatementInterpreter::interpretBlock(
    CleanBlockStatement* block_stmt
)
{
    for (auto& statement: block_stmt->statements) {
        std::unique_ptr<CleanExpression> ret_expr =
            interpret(statement.get(), block_stmt->scope.get());
        
        if (
            returned    ||
            broke       ||
            continued
        )
            return ret_expr;
    }

    return nullptr;
}

// If
std::unique_ptr<CleanExpression>
StatementInterpreter::interpretIf(
    CleanIfStatement* if_stmt,
    CleanScope* scope
)
{
    std::unique_ptr<CleanExpression> ret_expr = nullptr;

    std::unique_ptr<CleanExpression> cond_expr =
        interpret(if_stmt->condition.get(), scope);
    CleanBoolExpression* cond_bool =
        static_cast<CleanBoolExpression*>(cond_expr.get());

    bool interpret_else = true;
    if (cond_bool->value) {
        interpret_else = false;
        ret_expr = interpretBlock(if_stmt->body.get());
    }
    else {
        for (auto& elif_branch: if_stmt->elif_branches) {
            std::unique_ptr<CleanExpression> elif_cond_expr =
                interpret(elif_branch->condition.get(), scope);
            CleanBoolExpression* elif_cond_bool =
                static_cast<CleanBoolExpression*>(elif_cond_expr.get());
            
            if (elif_cond_bool->value) {
                interpret_else = false;
                ret_expr = interpretBlock(elif_branch->body.get());
                break;
            }
        }
    }

    // If the main branch and elif branches failed, interpret else branch
    if (interpret_else && if_stmt->else_branch)
        ret_expr = interpretBlock(if_stmt->else_branch->body.get());

    return ret_expr;
}

// For
std::unique_ptr<CleanExpression>
StatementInterpreter::interpretFor(
    CleanForStatement* for_stmt,
    CleanScope* scope
)
{
    std::unique_ptr<CleanExpression> ret_expr = nullptr;

    // Initialize the init_clause first
    if (for_stmt->init_clause)
        interpret(for_stmt->init_clause.get(), for_stmt->scope.get());

    // Execute the body conditional on the termination and increment clause
    while(true) {
        // If the termination clause doesn't hold, we are done
        if (for_stmt->term_clause) {
            std::unique_ptr<CleanExpression> term_expr =
                interpret(for_stmt->term_clause.get(), for_stmt->scope.get());
            CleanBoolExpression* term_bool =
                static_cast<CleanBoolExpression*>(term_expr.get());

            if (! term_bool->value)
                break;
        }

        // Execute the body
        ret_expr = interpretBlock(for_stmt->body.get());

        // If the body returned, we are done
        if (returned)
            return ret_expr;
        
        // If a continue statement was encountered
        // we just keep chugging along
        if (continued) {
            if (for_stmt->incr_clause)
                interpret(for_stmt->incr_clause.get(), for_stmt->scope.get());
            continued = false;
            continue;
        }

        // If a break statement was encountered
        // We leave the loop and return nothing
        if (broke) {
            broke = false;
            break;
        }
        
        // In any other case, we execute the increment clause
        if (for_stmt->incr_clause)
            interpret(for_stmt->incr_clause.get(), for_stmt->scope.get());
    }

    return ret_expr;
}

// While
std::unique_ptr<CleanExpression>
StatementInterpreter::interpretWhile(
    CleanWhileStatement* while_stmt,
    CleanScope* scope
)
{
    std::unique_ptr<CleanExpression> ret_expr = nullptr;

    while (true) {
        // If the condition doesn't hold, we are done
        if (while_stmt->condition) {
            std::unique_ptr<CleanExpression> cond_expr =
                interpret(while_stmt->condition.get(), scope);
            CleanBoolExpression* cond_bool =
                static_cast<CleanBoolExpression*>(cond_expr.get());

            if (! cond_bool->value)
                break;
        }

        // Execute the body
        ret_expr = interpretBlock(while_stmt->body.get());

        // If the body returned, we are done
        if (returned)
            return ret_expr;
        
        // If a continue statement was encountered
        // we just keep chugging along
        if (continued) {
            continued = false;
            continue;
        }

        // If a break statement was encountered
        // We leave the loop and return nothing
        if (broke) {
            broke = false;
            break;
        }
    }

    return ret_expr;
}

// Break
std::unique_ptr<CleanExpression>
StatementInterpreter::interpretBreak(
    CleanBreakStatement* br_stmt
)
{
    broke = true;
    return nullptr;
}

// Continue
std::unique_ptr<CleanExpression>
StatementInterpreter::interpretContinue(
    CleanContinueStatement* cont_stmt
)
{
    continued = true;
    return nullptr;
}

// Return
std::unique_ptr<CleanExpression>
StatementInterpreter::interpretReturn(
    CleanReturnStatement* ret_stmt,
    CleanScope* scope
)
{
    returned = true;

    return ret_stmt->expression
           ? interpretExpression(ret_stmt->expression.get(), scope)
           : nullptr;
}

// Expression
std::unique_ptr<CleanExpression>
StatementInterpreter::interpretExpression(
    CleanExpression* expr_stmt,
    CleanScope* scope
)
{
    return ExpressionInterpreter(scope).interpret(expr_stmt);
}
