#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "common/token_type.h"
#include "common/token.h"


class TokenTest: public ::testing::Test
{
    protected:
        void SetUp() override {
            source = "var";
            source_path = "main.proto";
        }

        void TearDown() override {
        }

        std::string source;
        std::string source_path;
};

TEST_F(TokenTest, getLexemeTest)
{
    Token token(
        PROTO_IDENTIFIER,
        std::make_shared<std::string>(source),
        source_path,
        0,
        3,
        1,
        1
    );

    std::string lexeme = token.getLexeme();
    EXPECT_EQ(lexeme, source);
}

TEST_F(TokenTest, getLineTest)
{
    Token token(
        PROTO_IDENTIFIER,
        std::make_shared<std::string>(source),
        source_path,
        0,
        3,
        1,
        1
    );

    TokenLine tok_line = token.getLine();
    EXPECT_EQ(tok_line.line, source);
}
