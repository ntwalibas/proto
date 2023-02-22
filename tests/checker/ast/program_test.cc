#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <exception>
#include <cstddef>
#include <memory>

#include "checker/checker_error.h"
#include "checker/ast/program.h"
#include "parser/parser.h"
#include "ast/program.h"
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
        std::string source = "count: uint64 = 0";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        Program prog = parser.parseProgram();
        ProgramChecker checker(prog);
        EXPECT_NO_THROW(checker.check());
        EXPECT_EQ(prog.getScope()->getDefinitions().size(), 1);
        EXPECT_EQ(prog.getScope()->hasDefinition("count"), true);
    }

    // Incorrect program
    {
        std::string source = "count: type = 0";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        Program prog = parser.parseProgram();
        ProgramChecker checker(prog);
        EXPECT_THROW({
            try {
                checker.check();
            } catch (CheckerError const& e) {
                EXPECT_STREQ(e.getSecondaryMessage(), "type [type] does not exist");
                
                throw;
            }
        }, CheckerError);
    }
}
