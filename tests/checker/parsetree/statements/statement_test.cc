#include <gtest/gtest.h>
#include <utility>
#include <memory>

#include "checker/parsetree/statements/statement.h"
#include "parsetree/definitions/definition.h"
#include "checker/checker_error.h"
#include "utils/inference.h"
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
        std::shared_ptr<Scope> parent_scope = std::make_shared<Scope>(nullptr);
        std::shared_ptr<Scope> scope = std::make_shared<Scope>(parent_scope);
        std::unique_ptr<TypeDeclaration> ret_type_decl = nullptr;
};

TEST_F(StatementCheckerTest, checkBlockTest)
{
    // Valid block statement
    {
        std::string source = "{count: int = 0:int\n count += 1:int\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ));
    }

    // Invalid block statement
    {
        std::string source = "{inner: function()->void{}\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ), CheckerError);
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
        
        EXPECT_NO_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ));
    }

    // Invalid condition in if statement
    {
        std::string source = "if (1) {counter = 0\n}\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ), CheckerError);
    }

    // Invalid condition in elif branch
    {
        std::string source = "if (True) {} elif (1) {counter = 1\n}\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ), CheckerError);
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
        
        EXPECT_NO_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ));
    }

    // Valid for loop only with initial clause
    {
        std::string source = "for (count = 0;;) {count += 1\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ));
    }

    // Valid for loop with initial clause and termination clause
    {
        std::string source = "for (count = 0:int; count < 10:int;) {count += 1:int\n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ));
    }

    // Valid for loop with initial clause, termination clause and increment clause
    {
        std::string source = "for (count = 0; count < 10; count += 1) {}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ));
    }

    // Invalid init clause
    {
        std::string source = "for (count += 0;;) {}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ), CheckerError);
    }

    // Invalid term clause
    {
        std::string source = "for (count = 0; count = 10;) {}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ), CheckerError);
    }
}

TEST_F(StatementCheckerTest, checkWhileTest)
{
    // Valid while loop
    {
        std::string source = "while (True) {}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ));
    }

    // Invalid while loop
    {
        std::string source = "while (1) {}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ), CheckerError);
    }
}

TEST_F(StatementCheckerTest, checkBreakTest)
{
    // Valid break statement
    {
        std::string source = "while (True) { break \n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ));
    }

    // Invalid continue
    {
        std::string source = "break\n";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ), CheckerError);
    }
}

TEST_F(StatementCheckerTest, checkContinueTest)
{
    // Valid continue statement
    {
        std::string source = "while (True) { continue \n}";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_NO_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ));
    }

    // Invalid continue
    {
        std::string source = "continue\n";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();
        
        EXPECT_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ), CheckerError);
    }
}

TEST_F(StatementCheckerTest, checkReturnTest)
{
    // Valid return statement with nothing to return
    {
        std::string source = "return\n";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();

        ret_type_decl = createSimpleTypeDeclaration(false, "void");
        
        EXPECT_NO_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ));

        delete ret_type_decl.release();
    }

    // Valid return statement with int to return
    {
        std::string source = "return 0:int\n";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();

        ret_type_decl = createSimpleTypeDeclaration(false, "int");
        
        EXPECT_NO_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ));

        delete ret_type_decl.release();
    }

    // Invalid return that returns nothing while the function returns int
    {
        std::string source = "return\n";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Statement> stmt = parser.parseStatement();

        ret_type_decl = createSimpleTypeDeclaration(false, "int");
        
        EXPECT_THROW(StatementChecker(ret_type_decl).check(
            static_cast<Statement*>(stmt.get()),
            scope
        ), CheckerError);

        delete ret_type_decl.release();
    }
}
