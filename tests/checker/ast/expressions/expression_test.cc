#include <gtest/gtest.h>
#include <memory>

#include "checker/ast/expressions/expression.h"
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
};

TEST_F(ExpressionCheckerTest, checkLiteralTest)
{
    std::string source = "0";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseExpression();

    EXPECT_NO_THROW(ExpressionChecker(expr, scope).checkLiteral());
}

TEST_F(ExpressionCheckerTest, checkCastTest)
{
    // Valid cast expression
    {
        std::string source = "0:int";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        std::unique_ptr<TypeDeclaration>& type_decl =
            ExpressionChecker(expr, scope).checkCast();

        EXPECT_EQ(type_decl->getTypeName(), "int");
    }

    // Invalid cast expression
    {
        std::string source = "0.5:bool";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(ExpressionChecker(expr, scope).checkCast(), CheckerError);
    }
}
