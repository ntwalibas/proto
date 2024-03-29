#include <gtest/gtest.h>
#include <memory>

#include "checker/parsetree/expressions/expression.h"
#include "parsetree/definitions/definition.h"
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
        std::string source = "0:uint";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<TypeDeclaration>& type_decl =
            ExpressionChecker(scope).checkCast(expr.get());

        EXPECT_EQ(type_decl->getTypeName(), "uint");
    }

    // Invalid cast expression
    {
        std::string source = "0.5:bool";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(scope).checkCast(expr.get()), CheckerError);
    }
}

TEST_F(ExpressionCheckerTest, checkTernaryIfTest)
{
    // Valid ternary if
    {
        std::string source = "1 > 0 ? true <> false";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<TypeDeclaration>& type_decl =
            ExpressionChecker(scope).checkTernaryIf(expr.get());

        EXPECT_EQ(type_decl->getTypeName(), "bool");
    }

    // Invalid ternary if: condition is not boolean
    {
        std::string source = "1 ? true <> false";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(scope).checkTernaryIf(expr.get()), CheckerError);
    }

    // Invalid ternary if: mismatched lvalue and rvalue
    {
        std::string source = "1 > 0 ? 1.0 <> 1:int";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(scope).checkTernaryIf(expr.get()), CheckerError);
    }
}

TEST_F(ExpressionCheckerTest, checkAssignmentTest)
{
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("count: int = 0");
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
            ExpressionChecker(scope).check(expr.get());

        EXPECT_EQ(expr_type_decl->getTypeName(), "int");
    }

    // Valid simple assignment with non-existent variable
    {
        std::string source = "new_count = 1:uint";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<TypeDeclaration>& expr_type_decl =
            ExpressionChecker(scope).check(expr.get());

        EXPECT_EQ(expr_type_decl->getTypeName(), "uint");
    }

    // Valid in-place assignment
    {
        std::string source = "count += 1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<TypeDeclaration>& expr_type_decl =
            ExpressionChecker(scope).check(expr.get());

        EXPECT_EQ(expr_type_decl->getTypeName(), "int");
    }

    // Invalid simple assignment: non-existent variable on the RHS
    {
        std::string source = "count = i + 1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(scope).check(expr.get()), CheckerError);
    }

    // Invalid simple assignment: incompatible types
    {
        std::string source = "count = true";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(scope).check(expr.get()), CheckerError);
    }

    // Invalid in-place assignment: non-existent variable on the LHS
    {
        std::string source = "i += 1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(scope).check(expr.get()), CheckerError);
    }

    // Invalid in-place assignment: non-existent variable on the RHS
    {
        std::string source = "count += i";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(scope).check(expr.get()), CheckerError);
    }

    // Invalid in-place assignment: incompatible types
    {
        std::string source = "count += true";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(scope).check(expr.get()), CheckerError);
    }
}
