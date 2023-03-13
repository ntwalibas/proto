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
#include <cstddef>
#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "checker/parsetree/expressions/expression.h"
#include "checker/parsetree/definitions/variable.h"
#include "checker/parsetree/statements/statement.h"
#include "parsetree/expressions/expression.h"
#include "parsetree/definitions/definition.h"
#include "parsetree/statements/statement.h"
#include "parsetree/statements/continue.h"
#include "parsetree/statements/return.h"
#include "checker/checker_error.h"
#include "parsetree/declarations/type.h"
#include "parsetree/statements/while.h"
#include "parsetree/statements/block.h"
#include "parsetree/statements/break.h"
#include "parsetree/statements/for.h"
#include "parsetree/statements/if.h"
#include "symbols/symtable.h"
#include "symbols/scope.h"


StatementChecker::StatementChecker(
    std::unique_ptr<TypeDeclaration>& ret_type_decl
) : inside_loop(false),
    ret_type_decl(ret_type_decl)
{}


/**
 * Checks that any given statement obeys the language semantics
 */
void
StatementChecker::check(
    Statement* stmt,
    std::shared_ptr<Scope> const& scope
)
{
    switch (stmt->getType()) {
        case StatementType::Block: {
            BlockStatement* block_stmt = static_cast<BlockStatement*>(stmt);
            checkBlock(block_stmt, scope);
            break;
        }
        
        case StatementType::If: {
            IfStatement * if_stmt = static_cast<IfStatement*>(stmt);
            checkIf(if_stmt, scope);
            break;
        }
        
        case StatementType::For: {
            ForStatement* for_stmt = static_cast<ForStatement*>(stmt);
            checkFor(for_stmt, scope);
            break;
        }
        
        case StatementType::While: {
            WhileStatement* while_stmt = static_cast<WhileStatement*>(stmt);
            checkWhile(while_stmt, scope);
            break;
        }
        
        case StatementType::Break: {
            BreakStatement* break_stmt = static_cast<BreakStatement*>(stmt);
            checkBreak(break_stmt, scope);
            break;
        }
        
        case StatementType::Continue: {
            ContinueStatement* continue_stmt = static_cast<ContinueStatement*>(stmt);
            checkContinue(continue_stmt, scope);
            break;
        }
        
        case StatementType::Return: {
            ReturnStatement* return_stmt = static_cast<ReturnStatement*>(stmt);
            checkReturn(return_stmt, scope);
            break;
        }
        
        case StatementType::Expression: {
            Expression* expression_stmt = static_cast<Expression*>(stmt);
            checkExpression(expression_stmt, scope);
            break;
        }
        
        default:
            throw std::invalid_argument("Given statement cannot be currently checked.");
    }
}


// Block
void
StatementChecker::checkBlock(
    BlockStatement* block_stmt,
    std::shared_ptr<Scope> const& scope
)
{
    std::vector<std::unique_ptr<Definition>>& definitions =
        block_stmt->getDefinitions();

    for (auto& definition: definitions) {
        if (definition->getType() == DefinitionType::Variable) {
            VariableDefinition* var_def =
                static_cast<VariableDefinition*>(definition.get());
            VariableDefinitionChecker(var_def, scope).check();
            scope->addDefinition(
                var_def->getToken().getLexeme(),
                definition
            );
        }
        else if (definition->getType() == DefinitionType::Statement) {
            Statement* stmt_def = static_cast<Statement*>(definition.get());
            check(
                stmt_def,
                scope
            );
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


// If
void
StatementChecker::checkIf(
    IfStatement* if_stmt,
    std::shared_ptr<Scope> const& scope
)
{
    // Validate the if branch
    std::unique_ptr<Expression>& if_cond = if_stmt->getCondition();
    std::unique_ptr<TypeDeclaration>& if_cond_type_decl =
        ExpressionChecker(if_cond.get(), scope).check();
    if (if_cond_type_decl->getTypeName() != "bool") {
        throw CheckerError(
            if_cond->getToken(),
            "invalid condition for if statement",
            "the condition for an if statement must be a boolean",
            true
        );
    }
    std::unique_ptr<BlockStatement>& if_body = if_stmt->getBody();
    std::shared_ptr<Scope> if_scope = std::make_shared<Scope>(scope);
    if_body->setScope(if_scope);
    check(
        static_cast<Statement*>(if_body.get()),
        if_scope
    );

    // Validate the elif branches, if any
    std::vector<std::unique_ptr<ElifBranch>>& elif_branches =
        if_stmt->getElifBranches();
    for (auto& branch : elif_branches) {
        std::unique_ptr<Expression>& branch_cond = branch->getCondition();
        std::unique_ptr<TypeDeclaration>& branch_cond_type_decl =
            ExpressionChecker(branch_cond.get(), scope).check();
        if (branch_cond_type_decl->getTypeName() != "bool") {
            throw CheckerError(
                branch_cond->getToken(),
                "invalid condition for elif branch",
                "the condition for an elif branch must be a boolean",
                true
            );
        }
        std::unique_ptr<BlockStatement>& branch_body = branch->getBody();
        std::shared_ptr<Scope> branch_scope = std::make_shared<Scope>(scope);
        branch_body->setScope(branch_scope);
        check(
            static_cast<Statement*>(branch_body.get()),
            branch_scope
        );
    }

    // Validate the else branch, if any
    std::unique_ptr<ElseBranch>& else_branch = if_stmt->getElseBranch();
    if (else_branch != nullptr) {
        std::unique_ptr<BlockStatement>& else_branch_body = else_branch->getBody();
        std::shared_ptr<Scope> else_branch_scope = std::make_shared<Scope>(scope);
        else_branch_body->setScope(else_branch_scope);
        check(
            static_cast<Statement*>(else_branch_body.get()),
            else_branch_scope
        );
    }
}


// For
void 
StatementChecker::checkFor(
    ForStatement* for_stmt,
    std::shared_ptr<Scope> const& scope
)
{
    std::shared_ptr<Scope> for_scope = std::make_shared<Scope>(scope);

    // Validate the initialization clause
    std::unique_ptr<Definition>& init_clause = for_stmt->getInitClause();
    if (init_clause != nullptr) {
            // make sure to add it to the scope introduced by the loop
        if (init_clause->getType() == DefinitionType::Variable) {
            VariableDefinition* var_def =
                static_cast<VariableDefinition*>(init_clause.get());
            VariableDefinitionChecker(var_def, for_scope).check();
            for_scope->addDefinition(
                var_def->getToken().getLexeme(),
                init_clause
            );
        }
        // If statement, make sure it is an assignment expression statement
        else if (init_clause->getType() == DefinitionType::Statement) {
            Statement* stmt_def = static_cast<Statement*>(init_clause.get());
            if (stmt_def->getType() != StatementType::Expression) {
                throw CheckerError(
                    stmt_def->getToken(),
                    "unexpected statement in for loop initiazation clause",
                    std::string("only variable definitions and assignment expression ") +
                    "statements can initialize a for loop",
                    true
                );
            }

            Expression* expr_stmt = static_cast<Expression*>(stmt_def);
            if (expr_stmt->getType() != ExpressionType::Assignment) {
                throw CheckerError(
                    stmt_def->getToken(),
                    "unexpected expression in for loop initiazation clause",
                    std::string("only variable definitions and assignment expression ") +
                    "statements can initialize a for loop",
                    true
                );
            }
            ExpressionChecker(expr_stmt, for_scope).check();
        }
        // Any other kind of definition is prohibited in the init clause
        else {
            throw CheckerError(
                init_clause->getToken(),
                "unexpected definition in for loop initiazation clause",
                std::string("only variable definitions and assignment expression ") +
                "statements can initialize a for loop",
                true
            );
        }
    }

    // Validate the termination clause
    // we require that it evaluates to a boolean
    std::unique_ptr<Expression>& term_clause = for_stmt->getTermClause();
    if (term_clause != nullptr) {
        std::unique_ptr<TypeDeclaration>& term_type_decl =
            ExpressionChecker(term_clause.get(), for_scope).check();

        if (term_type_decl->getTypeName() != "bool") {
            throw CheckerError(
                term_clause->getToken(),
                "unexpected expression in for loop termination clause",
                "only boolean expressions may appear in the termination clause, if any",
                true
            );
        }
    }

    // Validate the increment clause
    // the only requirement is that it is a valid expression
    std::unique_ptr<Expression>& incr_clause = for_stmt->getIncrClause();
    if (incr_clause != nullptr) {
        ExpressionChecker(incr_clause.get(), for_scope).check();
    }

    // Last, validate the body
    std::unique_ptr<BlockStatement>& for_body = for_stmt->getBody();
    for_body->setScope(for_scope);
    bool upper_loop_found = inside_loop;
    inside_loop = true;
    check(
        static_cast<Statement*>(for_body.get()),
        for_scope
    );
    if(upper_loop_found == false)
        inside_loop = false;
}


// While
void
StatementChecker::checkWhile(
    WhileStatement* while_stmt,
    std::shared_ptr<Scope> const& scope
)
{
    std::unique_ptr<Expression>& while_cond = while_stmt->getCondition();
    std::unique_ptr<TypeDeclaration>& while_cond_type_decl =
        ExpressionChecker(while_cond.get(), scope).check();
    if (while_cond_type_decl->getTypeName() != "bool") {
        throw CheckerError(
            while_cond->getToken(),
            "invalid condition for while statement",
            "the condition for an while statement must be a boolean",
            true
        );
    }
    std::unique_ptr<BlockStatement>& while_body = while_stmt->getBody();
    std::shared_ptr<Scope> while_scope = std::make_shared<Scope>(scope);
    while_body->setScope(while_scope);

    bool upper_loop_found = inside_loop;
    inside_loop = true;
    check(
        static_cast<Statement*>(while_body.get()),
        while_scope
    );
    if(upper_loop_found == false)
        inside_loop = false;
}


// Break
void
StatementChecker::checkBreak(
    BreakStatement* break_stmt,
    std::shared_ptr<Scope> const& scope
)
{
    if (!inside_loop) {
        throw CheckerError(
            break_stmt->getToken(),
            "unexpected break statement",
            "a break statement can only occur within a loop body",
            false
        );
    }
}


// Continue
void
StatementChecker::checkContinue(
    ContinueStatement* continue_stmt,
    std::shared_ptr<Scope> const& scope
)
{
    if (!inside_loop) {
        throw CheckerError(
            continue_stmt->getToken(),
            "unexpected continue statement",
            "a continue statement can only occur within a loop body",
            false
        );
    }
}


 // Return
void 
StatementChecker::checkReturn(
    ReturnStatement* return_stmt,
    std::shared_ptr<Scope> const& scope
) {
    std::unique_ptr<Expression>& ret_expr = return_stmt->getExpression();

    // If the return statement has no expression to return,
    // then the function's return type must be void
    if (ret_expr == nullptr) {
        if (ret_type_decl->getTypeName() != "void") {
            throw CheckerError(
                return_stmt->getToken(),
                "missing expression to return",
                "return statement must specify the expression of type `" +
                ret_type_decl->getTypeName() + "` which the function returns",
                false
            );
        }

        return;
    }

    // If the return statement has an expression to return,
    // its type must match that of the function where this return is found
    std::unique_ptr<TypeDeclaration>& ret_type =
        ExpressionChecker(ret_expr.get(), scope).check();
    
    if (! typeDeclarationEquals(ret_type, ret_type_decl)) {
        throw CheckerError(
            return_stmt->getToken(),
            "returned expression of incorrect type",
            "expression returned has type `" + ret_type->getTypeName() +
            "` while function has return type `" + ret_type_decl->getTypeName() + "`",
            false
        );
    }
}


// Expression
void
StatementChecker::checkExpression(
    Expression* expression_stmt,
    std::shared_ptr<Scope> const& scope
)
{
    ExpressionChecker(expression_stmt, scope).check();
}
