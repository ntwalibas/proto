#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <string>

#include "checker/parsetree/expressions/expression.h"
#include "cleaner/parsetree/expressions/expression.h"
#include "cleaner/parsetree/definitions/variable.h"
#include "cleaner/ast/expressions/expression.h"
#include "cleaner/symbols/scope.h"
#include "cleaner/cleaner.h"
#include "checker/checker.h"
#include "parser/parser.h"
#include "symbols/scope.h"
#include "lexer/lexer.h"


class ExpressionCleanerTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
        std::unique_ptr<Definition> var_def = nullptr;
        std::shared_ptr<Scope> scope = std::make_shared<Scope>(nullptr);
        std::shared_ptr<CleanScope> clean_scope = std::make_shared<CleanScope>(nullptr);
};

// Literals
TEST_F(ExpressionCleanerTest, cleanLiteralTest) {
    // Boolean
    {
        std::string source = "true";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<CleanExpression> clean_expr =
            ExpressionCleaner(clean_scope).clean(expr.get());

        EXPECT_EQ(clean_expr->type, CleanExpressionType::Boolean);
        CleanBoolExpression* bool_expr =
            static_cast<CleanBoolExpression*>(clean_expr.get());
        EXPECT_EQ(bool_expr->value, true);
    }

    // Unsigned int
    {
        std::string source = "10";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<CleanExpression> clean_expr =
            ExpressionCleaner(clean_scope).clean(expr.get());

        EXPECT_EQ(clean_expr->type, CleanExpressionType::SignedInt);
        CleanSignedIntExpression* int_expr =
            static_cast<CleanSignedIntExpression*>(clean_expr.get());
        EXPECT_EQ(int_expr->value, (int64_t) 10);
    }

    // Signed int
    {
        std::string source = "-10";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<CleanExpression> clean_expr =
            ExpressionCleaner(clean_scope).clean(expr.get());

        EXPECT_EQ(clean_expr->type, CleanExpressionType::SignedInt);
        CleanSignedIntExpression* int_expr =
            static_cast<CleanSignedIntExpression*>(clean_expr.get());
        EXPECT_EQ(int_expr->value, (int64_t) -10);
    }

    // Float
    {
        std::string source = "1.0";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<CleanExpression> clean_expr =
            ExpressionCleaner(clean_scope).clean(expr.get());

        EXPECT_EQ(clean_expr->type, CleanExpressionType::Float);
        CleanFloatExpression* float_expr =
            static_cast<CleanFloatExpression*>(clean_expr.get());
        EXPECT_EQ(float_expr->value, (double) 1.0);
    }

    // String
    {
        std::string source = "\"Hello World!\"";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<CleanExpression> clean_expr =
            ExpressionCleaner(clean_scope).clean(expr.get());

        EXPECT_EQ(clean_expr->type, CleanExpressionType::String);
        CleanStringExpression* string_expr =
            static_cast<CleanStringExpression*>(clean_expr.get());
        EXPECT_EQ(string_expr->value, "Hello World!");
    }
}

// Cast
TEST_F(ExpressionCleanerTest, cleanCastTest) {
    // Int to string results in function call
    {
        std::string source = "1:string";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        ExpressionChecker(scope).checkCast(expr.get());
        std::unique_ptr<CleanExpression> clean_expr =
            ExpressionCleaner(clean_scope).clean(expr.get());

        EXPECT_EQ(clean_expr->type, CleanExpressionType::Call);
        CleanCallExpression* call_expr =
            static_cast<CleanCallExpression*>(clean_expr.get());
        EXPECT_EQ(call_expr->fun_name, "__cast@string__(int)");
    }

    // Int to unsigned int results in immediate cast to unsigned int literal
    {
        std::string source = "1:uint";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        ExpressionChecker(scope).checkCast(expr.get());
        std::unique_ptr<CleanExpression> clean_expr =
            ExpressionCleaner(clean_scope).clean(expr.get());

        EXPECT_EQ(clean_expr->type, CleanExpressionType::UnsignedInt);
        CleanUnsignedIntExpression* uint_expr =
            static_cast<CleanUnsignedIntExpression*>(clean_expr.get());
        EXPECT_EQ(uint_expr->value, (uint64_t) 1);
    }
}

// Variable
TEST_F(ExpressionCleanerTest, cleanVariableTest) {
    std::string source = "count";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseExpression();
    std::unique_ptr<CleanExpression> clean_expr =
        ExpressionCleaner(clean_scope).clean(expr.get());

    EXPECT_EQ(clean_expr->type, CleanExpressionType::Variable);
    CleanVariableExpression* var_expr =
        static_cast<CleanVariableExpression*>(clean_expr.get());
    EXPECT_EQ(var_expr->var_name, "count");
}

// Group
TEST_F(ExpressionCleanerTest, cleanGroupTest) {
    std::string source = "(count)";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseExpression();
    std::unique_ptr<CleanExpression> clean_expr =
        ExpressionCleaner(clean_scope).clean(expr.get());

    EXPECT_EQ(clean_expr->type, CleanExpressionType::Group);
    CleanGroupExpression* group_expr =
        static_cast<CleanGroupExpression*>(clean_expr.get());
    EXPECT_EQ(group_expr->expression->type, CleanExpressionType::Variable);
    CleanVariableExpression* var_expr =
        static_cast<CleanVariableExpression*>(group_expr->expression.get());
    EXPECT_EQ(var_expr->var_name, "count");
}

// Assignment
TEST_F(ExpressionCleanerTest, cleanAssignmentTest) {
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("count: int = 0");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        var_def = parser.parseDefinition();
        scope->addDefinition("count", var_def);
    }

    // Assignment doesn't introduce a definition
    {
        std::string source = "count = 1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        ExpressionChecker(scope).check(expr.get());
        std::unique_ptr<CleanExpression> clean_expr =
            ExpressionCleaner(clean_scope).clean(expr.get());
        
        EXPECT_EQ(clean_expr->type, CleanExpressionType::Assignment);
        CleanAssignmentExpression* assign_expr =
            static_cast<CleanAssignmentExpression*>(clean_expr.get());
        EXPECT_EQ(assign_expr->rvalue->type, CleanExpressionType::SignedInt);
        CleanSignedIntExpression* int_expr =
            static_cast<CleanSignedIntExpression*>(assign_expr->rvalue.get());
        EXPECT_EQ(int_expr->value, (int64_t) 1);
    }

    // Assignment introduces a definition
    {
        std::string source = "new_count = 1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        ExpressionChecker(scope).check(expr.get());
        std::unique_ptr<CleanExpression> clean_expr =
            ExpressionCleaner(clean_scope).clean(expr.get());
        
        EXPECT_EQ(clean_expr->type, CleanExpressionType::Assignment);
        CleanAssignmentExpression* assign_expr =
            static_cast<CleanAssignmentExpression*>(clean_expr.get());
        EXPECT_EQ(assign_expr->rvalue->type, CleanExpressionType::Variable);
        CleanVariableExpression* var_expr =
            static_cast<CleanVariableExpression*>(assign_expr->rvalue.get());
        EXPECT_EQ(var_expr->var_name, "new_count");

        EXPECT_EQ(clean_scope->hasSymbol<CleanVariableDefinition>("new_count"), true);
        std::unique_ptr<CleanVariableDefinition>& clean_var =
            clean_scope->getSymbol<CleanVariableDefinition>("new_count");
        EXPECT_EQ(clean_var->name, "new_count");
        EXPECT_EQ(clean_var->initializer->type, CleanExpressionType::SignedInt);
        CleanSignedIntExpression* int_expr =
            static_cast<CleanSignedIntExpression*>(clean_var->initializer.get());
        EXPECT_EQ(int_expr->value, (int64_t) 1);
    }
}
