#include <gtest/gtest.h>

#include "common/token_type.h"


class TokenTypeTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
};

TEST_F(TokenTypeTest, tokenTypeToStringTest)
{
    EXPECT_EQ(tokenTypeToString(PROTO_NOT_EQUAL),    std::string("NOT_EQUAL"));
    EXPECT_EQ(tokenTypeToString(PROTO_FLOAT),        std::string("FLOAT"));
    EXPECT_EQ(tokenTypeToString(PROTO_CONTINUE),     std::string("CONTINUE"));
    EXPECT_EQ(tokenTypeToString(PROTO_ERROR),        std::string("ERROR"));
}
