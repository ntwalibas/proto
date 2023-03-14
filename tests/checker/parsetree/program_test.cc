#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdexcept>
#include <cstddef>
#include <memory>

#include "checker/checker_error.h"
#include "checker/parsetree/program.h"
#include "parser/parser.h"
#include "parsetree/program.h"
#include "lexer/lexer.h"


class ProgramCheckerTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
};

TEST_F(ProgramCheckerTest, checkTest)
{
    // Correct program
    {
        std::string source = "count: uint = 0\n main: function()->uint{return 0\n}\n";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        Program prog = parser.parseProgram();
        ProgramChecker checker(prog);
        EXPECT_NO_THROW(checker.check());
        EXPECT_EQ(prog.getScope()->hasDefinition("count"), true);
    }

    // Incorrect program
    {
        std::string source = "count: type = 0\n main: function()->uint{return 0\n}\n";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        Program prog = parser.parseProgram();
        ProgramChecker checker(prog);
        EXPECT_THROW({
            try {
                checker.check();
            } catch (CheckerError const& e) {
                EXPECT_STREQ(e.getSecondaryMessage(), "type `type` does not exist");
                
                throw;
            }
        }, CheckerError);
    }

    // Missing entry point
    {
        std::string source = "count: uint = 0\n";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        Program prog = parser.parseProgram();
        ProgramChecker checker(prog);
        EXPECT_THROW({
            try {
                checker.check();
            } catch (CheckerError const& e) {
                EXPECT_STREQ(
                    e.getPrimaryMessage(),
                    "missing entry point"
                );
                
                throw;
            }
        }, CheckerError);
    }

    // Invalid main function
    {
        std::string source = "main: function()->int{}\n";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        Program prog = parser.parseProgram();
        ProgramChecker checker(prog);
        EXPECT_THROW({
            try {
                checker.check();
            } catch (CheckerError const& e) {
                EXPECT_STREQ(
                    e.getPrimaryMessage(),
                    "invalid main function"
                );
                
                throw;
            }
        }, CheckerError);
    }
}
