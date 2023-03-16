#include <gtest/gtest.h>
#include <cstdint>
#include <memory>
#include <string>

#include "checker/parsetree/statements/statement.h"
#include "cleaner/parsetree/statements/statement.h"
#include "interpreter/ast/statements/statement.h"
#include "cleaner/ast/expressions/expression.h"
#include "cleaner/ast/statements/statement.h"
#include "interpreter/interpreter.h"
#include "cleaner/symbols/scope.h"
#include "cleaner/cleaner.h"
#include "checker/checker.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class StatementInterpreterTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
        std::shared_ptr<CleanScope> clean_scope = std::make_shared<CleanScope>(nullptr);
};

TEST_F(StatementInterpreterTest, interpretReturnTest) {
    std::string source = "return 1";

    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Statement> stmt = parser.parseStatement();
    std::unique_ptr<CleanStatement> clean_stmt =
        StatementCleaner().clean(stmt.get(), clean_scope);
    std::unique_ptr<CleanStatement> i_clean_stmt =
        StatementInterpreter().interpret(
            clean_stmt.get(), clean_scope.get()
        );
    
    EXPECT_EQ(i_clean_stmt->type, CleanStatementType::Expression);
    CleanExpression* i_clean_expr =
        static_cast<CleanExpression*>(i_clean_stmt.get());
    EXPECT_EQ(i_clean_expr->type, CleanExpressionType::SignedInt);
    CleanSignedIntExpression* int_expr =
        static_cast<CleanSignedIntExpression*>(i_clean_expr);
    EXPECT_EQ(int_expr->value, (int64_t) 1);
}
