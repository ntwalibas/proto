#include <gtest/gtest.h>
#include <memory>
#include <string>

#include <iostream>

#include "ast/definitions/definition.h"
#include "ast/definitions/variable.h"
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
            EXPECT_STREQ(e.getPrimaryMessage(), "a program must have at least one definition");
            throw;
        }
    }, ParserError);
}

TEST_F(ParserTest, parseDefinitionTest)
{
    // The definition doesn't start with an identifier that names it
    std::string noIdentifierSource = "function";
    Lexer noIdentifierLexer(std::make_shared<std::string>(noIdentifierSource), source_path);
    Parser noIdentifierparser(noIdentifierLexer);

    EXPECT_THROW({
        try {
            noIdentifierparser.parseDefinition();
        } catch(ParserError& e) {
            EXPECT_STREQ(e.getSecondaryMessage(), "expected an identifier");
            throw;
        }
    }, ParserError);

    // Missing colon after definition name
    std::string noColonSource = "name";
    Lexer noColonLexer(std::make_shared<std::string>(noColonSource), source_path);
    Parser noColonparser(noColonLexer);

    EXPECT_THROW({
        try {
            noColonparser.parseDefinition();
        } catch(ParserError& e) {
            EXPECT_STREQ(e.getSecondaryMessage(), "expected a colon after this definition name");
            throw;
        }
    }, ParserError);

    // Otherwise, we have a valid definition
    std::string Source = "name: string";
    Lexer Lexer(std::make_shared<std::string>(Source), source_path);
    Parser parser(Lexer);
    std::unique_ptr<Definition> def = parser.parseDefinition();
    EXPECT_EQ(def -> getType(), DefinitionType::Variable);
}

TEST_F(ParserTest, parseVariableDefinitionTest)
{
    std::string source = "name: string";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Definition> def = parser.parseDefinition();
    EXPECT_EQ(def->getType(), DefinitionType::Variable);
    VariableDefinition& var_def = dynamic_cast<VariableDefinition&>(*def);
    EXPECT_EQ(var_def.getToken().getLexeme(), std::string("name"));
}
