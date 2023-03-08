#include <gtest/gtest.h>
#include <memory>

#include "checker/ast/statements/statement.h"
#include "ast/definitions/definition.h"
#include "checker/checker_error.h"
#include "symbols/scope.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class StatementCheckerTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
        std::shared_ptr<Scope> scope = std::make_shared<Scope>(nullptr);
};

TEST_F(StatementCheckerTest, checkBlockTest)
{
    // Valid block statement
    {
        std::string source = "{count: int = 0:int\n count += 1:int\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(stmt.get(), scope).checkBlock());
    }

    // Invalid block statement
    {
        std::string source = "{inner: function()->void{}\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(stmt.get(), scope).checkBlock(), CheckerError);
    }
}

TEST_F(StatementCheckerTest, checkIfTest)
{
    // Valid if statement
    {
        std::string source = "if (True && True) {counter = 0\n} elif (True && False) {conter = 1\n} elif (False && False) {counter = 2\n}\n";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(stmt.get(), scope).checkIf());
    }

    // Invalid condition in if statement
    {
        std::string source = "if (1) {counter = 0\n}\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(stmt.get(), scope).checkIf(), CheckerError);
    }

    // Invalid condition in elif branch
    {
        std::string source = "if (True) {} elif (1) {counter = 1\n}\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(stmt.get(), scope).checkIf(), CheckerError);
    }
}

TEST_F(StatementCheckerTest, checkForTest)
{
    // Valid for loop without clauses
    {
        std::string source = "for (;;) {count = 0:int\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(stmt.get(), scope).checkFor());
    }

    // Valid for loop only with initial clause
    {
        std::string source = "for (count = 0;;) {count += 1\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(stmt.get(), scope).checkFor());
    }

    // Valid for loop with initial clause and termination clause
    {
        std::string source = "for (count = 0:int; count < 10:int;) {count += 1:int\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(stmt.get(), scope).checkFor());
    }

    // Valid for loop with initial clause, termination clause and increment clause
    {
        std::string source = "for (count = 0; count < 10; count += 1) {}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(stmt.get(), scope).checkFor());
    }

    // Invalid init clause
    {
        std::string source = "for (count += 0;;) {}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(stmt.get(), scope).checkFor(), CheckerError);
    }

    // Invalid term clause
    {
        std::string source = "for (count = 0; count = 10;) {}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(stmt.get(), scope).checkFor(), CheckerError);
    }
}
