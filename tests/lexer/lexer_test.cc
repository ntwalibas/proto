#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

#include "common/token_type.h"
#include "common/token.h"
#include "lexer/lexer.h"


class LexerTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
};

TEST_F(LexerTest, lexBitwiseNot)
{
    std::string source = "~";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_BITWISE_NOT);
    EXPECT_EQ(token.getLexeme(), source);
}

TEST_F(LexerTest, lexMinus)
{
    std::string source = "-";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_MINUS);
    EXPECT_EQ(token.getLexeme(), source);
}

TEST_F(LexerTest, lexReturnType)
{
    std::string source = "->";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_RETURN_TYPE);
    EXPECT_EQ(token.getLexeme(), source);
}

TEST_F(LexerTest, lexInteger)
{
    std::string source = "12";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_INT);
    EXPECT_EQ(token.getLexeme(), source);
}

TEST_F(LexerTest, lexFloat)
{
    std::string source = "12.5";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_FLOAT);
    EXPECT_EQ(token.getLexeme(), source);
}

TEST_F(LexerTest, lexString)
{
    std::string source = "\"Hello World!\"";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_STRING);
    EXPECT_EQ(token.getLexeme(), std::string("Hello World!"));
}

TEST_F(LexerTest, lexKeyword)
{
    std::string source = "function";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_FUNCTION);
    EXPECT_EQ(token.getLexeme(), source);
}

TEST_F(LexerTest, lexIdentifier)
{
    std::string source = "sum";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_IDENTIFIER);
    EXPECT_EQ(token.getLexeme(), source);
}

TEST_F(LexerTest, lexDeclaration)
{
    std::string source = "sum: function(a: const int, b: const int) -> int";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    
    std::vector<Token> tokens;
    while (1) {
        Token current_token = lexer.lex();
        if (current_token.type != PROTO_EOF)
            tokens.push_back(current_token);
        else
            break;
    }

    EXPECT_EQ(tokens[0].type, PROTO_IDENTIFIER);
    EXPECT_EQ(tokens[0].getLexeme(), std::string("sum"));

    EXPECT_EQ(tokens[1].type, PROTO_COLON);
    EXPECT_EQ(tokens[1].getLexeme(), std::string(":"));

    EXPECT_EQ(tokens[2].type, PROTO_FUNCTION);
    EXPECT_EQ(tokens[2].getLexeme(), std::string("function"));

    EXPECT_EQ(tokens[3].type, PROTO_LEFT_PAREN);
    EXPECT_EQ(tokens[3].getLexeme(), std::string("("));

    EXPECT_EQ(tokens[4].type, PROTO_IDENTIFIER);
    EXPECT_EQ(tokens[4].getLexeme(), std::string("a"));

    EXPECT_EQ(tokens[5].type, PROTO_COLON);
    EXPECT_EQ(tokens[5].getLexeme(), std::string(":"));

    EXPECT_EQ(tokens[6].type, PROTO_CONST);
    EXPECT_EQ(tokens[6].getLexeme(), std::string("const"));

    EXPECT_EQ(tokens[7].type, PROTO_IDENTIFIER);
    EXPECT_EQ(tokens[7].getLexeme(), std::string("int"));

    EXPECT_EQ(tokens[8].type, PROTO_COMMA);
    EXPECT_EQ(tokens[8].getLexeme(), std::string(","));

    EXPECT_EQ(tokens[9].type, PROTO_IDENTIFIER);
    EXPECT_EQ(tokens[9].getLexeme(), std::string("b"));

    EXPECT_EQ(tokens[10].type, PROTO_COLON);
    EXPECT_EQ(tokens[10].getLexeme(), std::string(":"));

    EXPECT_EQ(tokens[11].type, PROTO_CONST);
    EXPECT_EQ(tokens[11].getLexeme(), std::string("const"));

    EXPECT_EQ(tokens[12].type, PROTO_IDENTIFIER);
    EXPECT_EQ(tokens[12].getLexeme(), std::string("int"));

    EXPECT_EQ(tokens[13].type, PROTO_RIGHT_PAREN);
    EXPECT_EQ(tokens[13].getLexeme(), std::string(")"));

    EXPECT_EQ(tokens[14].type, PROTO_RETURN_TYPE);
    EXPECT_EQ(tokens[14].getLexeme(), std::string("->"));

    EXPECT_EQ(tokens[15].type, PROTO_IDENTIFIER);
    EXPECT_EQ(tokens[15].getLexeme(), std::string("int"));
}

TEST_F(LexerTest, lexIANDtest)
{
    std::string source = "&=";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_IAND);
}

TEST_F(LexerTest, lexIRSHIFTTest)
{
    std::string source = ">>=";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_IRSHIFT);
}

TEST_F(LexerTest, lexIDIVTest)
{
    std::string source = "/=";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_IDIV);
}

TEST_F(LexerTest, lexIMULTest)
{
    std::string source = "*=";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_IMUL);
}

TEST_F(LexerTest, lexIPOWTest)
{
    std::string source = "**=";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Token token = lexer.lex();
    EXPECT_EQ(token.type, PROTO_IPOW);
}
