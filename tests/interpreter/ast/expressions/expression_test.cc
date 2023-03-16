#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "checker/parsetree/expressions/expression.h"
#include "cleaner/parsetree/expressions/expression.h"
#include "interpreter/ast/expressions/expression.h"
#include "cleaner/ast/expressions/expression.h"
#include "interpreter/interpreter.h"
#include "cleaner/symbols/scope.h"
#include "cleaner/cleaner.h"
#include "checker/checker.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class ExpressionInterpreterTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
        std::shared_ptr<CleanScope> clean_scope = std::make_shared<CleanScope>(nullptr);
};

TEST_F(ExpressionInterpreterTest, interpretLiteralTest) {
    // Boolean
    {
        std::string source = "true";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<CleanExpression> clean_expr =
            ExpressionCleaner(clean_scope).clean(expr.get());
        std::unique_ptr<CleanExpression> i_clean_expr =
            ExpressionInterpreter(clean_scope.get()).interpret(clean_expr.get());

        EXPECT_EQ(i_clean_expr->type, CleanExpressionType::Boolean);
        CleanBoolExpression* bool_expr =
            static_cast<CleanBoolExpression*>(i_clean_expr.get());
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
        std::unique_ptr<CleanExpression> i_clean_expr =
            ExpressionInterpreter(clean_scope.get()).interpret(clean_expr.get());

        EXPECT_EQ(i_clean_expr->type, CleanExpressionType::SignedInt);
        CleanSignedIntExpression* int_expr =
            static_cast<CleanSignedIntExpression*>(i_clean_expr.get());
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
        std::unique_ptr<CleanExpression> i_clean_expr =
            ExpressionInterpreter(clean_scope.get()).interpret(clean_expr.get());

        EXPECT_EQ(i_clean_expr->type, CleanExpressionType::SignedInt);
        CleanSignedIntExpression* int_expr =
            static_cast<CleanSignedIntExpression*>(i_clean_expr.get());
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
        std::unique_ptr<CleanExpression> i_clean_expr =
            ExpressionInterpreter(clean_scope.get()).interpret(clean_expr.get());

        EXPECT_EQ(i_clean_expr->type, CleanExpressionType::Float);
        CleanFloatExpression* float_expr =
            static_cast<CleanFloatExpression*>(i_clean_expr.get());
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
        std::unique_ptr<CleanExpression> i_clean_expr =
            ExpressionInterpreter(clean_scope.get()).interpret(clean_expr.get());

        EXPECT_EQ(i_clean_expr->type, CleanExpressionType::String);
        CleanStringExpression* string_expr =
            static_cast<CleanStringExpression*>(i_clean_expr.get());
        EXPECT_EQ(string_expr->value, "Hello World!");
    }
}
