#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "common/token_type.h"
#include "common/token.h"


class TokenTest: public ::testing::Test
{
    protected:
        void SetUp() override {
            static std::string _source = "var";
            source = std::make_shared<std::string>(_source);
            source_path = "main.pro";
        }

        void TearDown() override {
        }

        std::shared_ptr<std::string> source;
        std::string source_path;
};

TEST_F(TokenTest, getLexemeTest)
{
    Token token(
        PROTO_IDENTIFIER,
        source,
        source_path,
        source->begin(),
        3,
        1,
        1
    );

    std::string lexeme = token.getLexeme();
    EXPECT_EQ(lexeme, *source);
}

TEST_F(TokenTest, getLineTest)
{
    Token token(
        PROTO_IDENTIFIER,
        source,
        source_path,
        source->begin(),
        3,
        1,
        1
    );

    TokenLine tok_line = token.getLine();
    EXPECT_EQ(tok_line.line, *source);
}
