#include <gtest/gtest.h>
#include <memory>

#include "checker/ast/expressions/expression.h"
#include "ast/definitions/definition.h"
#include "checker/checker_error.h"
#include "symbols/scope.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class ExpressionCheckerTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
        std::shared_ptr<Scope> scope = std::make_shared<Scope>(nullptr);
        std::unique_ptr<Definition> var_def = nullptr;
};

TEST_F(ExpressionCheckerTest, checkCastTest)
{
    // Valid cast expression
    {
        std::string source = "0:int";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<TypeDeclaration>& type_decl =
            ExpressionChecker(expr.get(), scope).checkCast();

        EXPECT_EQ(type_decl->getTypeName(), "int");
    }

    // Invalid cast expression
    {
        std::string source = "0.5:bool";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(expr.get(), scope).checkCast(), CheckerError);
    }
}

TEST_F(ExpressionCheckerTest, checkTernaryIfTest)
{
    // Valid ternary if
    {
        std::string source = "1 > 0 ? True <> False";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<TypeDeclaration>& type_decl =
            ExpressionChecker(expr.get(), scope).checkTernaryIf();

        EXPECT_EQ(type_decl->getTypeName(), "bool");
    }

    // Invalid ternary if: condition is not boolean
    {
        std::string source = "1 ? True <> False";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(expr.get(), scope).checkTernaryIf(), CheckerError);
    }

    // Invalid ternary if: mismatched lvalue and rvalue
    {
        std::string source = "1 > 0 ? 1.0 <> 1:int";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(expr.get(), scope).checkTernaryIf(), CheckerError);
    }
}

TEST_F(ExpressionCheckerTest, checkAssignmentTest)
{
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("count: uint = 0");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        var_def = parser.parseDefinition();
        scope->addDefinition("count", var_def);
    }

    // Valid simple assignment with existing variable
    {
        std::string source = "count = 1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<TypeDeclaration>& expr_type_decl =
            ExpressionChecker(expr.get(), scope).check();

        EXPECT_EQ(expr_type_decl->getTypeName(), "uint");
    }

    // Valid simple assignment with non-existent variable
    {
        std::string source = "new_count = 1:int";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<TypeDeclaration>& expr_type_decl =
            ExpressionChecker(expr.get(), scope).check();

        EXPECT_EQ(expr_type_decl->getTypeName(), "int");
    }

    // Valid in-place assignment
    {
        std::string source = "count += 1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<TypeDeclaration>& expr_type_decl =
            ExpressionChecker(expr.get(), scope).check();

        EXPECT_EQ(expr_type_decl->getTypeName(), "uint");
    }

    // Invalid simple assignment: non-existent variable on the RHS
    {
        std::string source = "count = i + 1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(expr.get(), scope).check(), CheckerError);
    }

    // Invalid simple assignment: incompatible types
    {
        std::string source = "count = True";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(expr.get(), scope).check(), CheckerError);
    }

    // Invalid in-place assignment: non-existent variable on the LHS
    {
        std::string source = "i += 1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(expr.get(), scope).check(), CheckerError);
    }

    // Invalid in-place assignment: non-existent variable on the RHS
    {
        std::string source = "count += i";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(expr.get(), scope).check(), CheckerError);
    }

    // Invalid in-place assignment: incompatible types
    {
        std::string source = "count += True";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(expr.get(), scope).check(), CheckerError);
    }
}
