#include <gtest/gtest.h>
#include <string>

#include "parser/parser.h"
#include "common/token.h"
#include "lexer/lexer.h"


class ParserTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
};

TEST_F(ParserTest, parseEmptyProgramTest)
{
    std::string source = "";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);

    EXPECT_THROW({
        try {
            parser.parse();
        } catch(ParserError& e) {
            EXPECT_EQ(e.getPrimaryMessage(), std::string("a program must have at least one definition"));
            throw;
        }
    }, ParserError);
}
