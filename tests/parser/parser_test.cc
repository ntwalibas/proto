#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "ast/definitions/definition.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/variable.h"
#include "ast/definitions/variable.h"
#include "ast/expressions/literal.h"
#include "ast/declarations/type.h"
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


// Program
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


// Definitions
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
    std::string Source = "name: string = \"John Doe\"";
    Lexer Lexer(std::make_shared<std::string>(Source), source_path);
    Parser parser(Lexer);
    std::unique_ptr<Definition> def = parser.parseDefinition();
    EXPECT_EQ(def->getType(), DefinitionType::Variable);
}

TEST_F(ParserTest, parseVariableDefinitionTest)
{
    std::string source = "name: string = \"John Doe\"";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Definition> def = parser.parseDefinition();

    // We have the correct variable name
    EXPECT_EQ(def->getType(), DefinitionType::Variable);
    VariableDefinition& var_def = dynamic_cast<VariableDefinition&>(*def);
    EXPECT_EQ(var_def.getToken().getLexeme(), "name");

    // We have the correct type
    SimpleTypeDeclaration& var_type = var_def.getTypeDeclaration();
    EXPECT_EQ(var_type.getToken().getLexeme(), "string");

    // We have the correct initializer
    std::unique_ptr<Expression>& var_init = var_def.getInitializer();
    EXPECT_EQ(var_init->getType(), ExpressionType::Literal);
    LiteralExpression& lit_expr = dynamic_cast<LiteralExpression&>(*var_init);
    EXPECT_EQ(lit_expr.getToken().getLexeme(), "John Doe");
    EXPECT_EQ(lit_expr.getLiteralType(), LiteralType::String);
}


// Declarations
TEST_F(ParserTest, parseSimpleTypeDeclarationTest)
{
    std::string source = "const string";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    SimpleTypeDeclaration decl = parser.parseSimpleTypeDeclaration();

    EXPECT_EQ(decl.isConst(), true);
    EXPECT_EQ(decl.getToken().getLexeme(), "string");
}


// Expressions
TEST_F(ParserTest, parsePrimaryExpressionTest)
{
    std::string source = "name";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parsePrimaryExpression();

    // We have the correct expression type
    EXPECT_EQ(expr->getType(), ExpressionType::Variable);
    VariableExpression& var_expr = dynamic_cast<VariableExpression&>(*expr);

    // We have the correct variable expression name
    EXPECT_EQ(var_expr.getToken().getLexeme(), "name");
}

TEST_F(ParserTest, parseVariableExpressionTest)
{
    std::string source = "name";
    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    std::unique_ptr<VariableExpression> var_expr = parser.parseVariableExpression();

    EXPECT_EQ(var_expr->getToken().getLexeme(), "name");
}

TEST_F(ParserTest, parseLiteralExpressionTest)
{
    // Integer literal
    std::string intSource = "0";
    Lexer intLexer(std::make_shared<std::string>(intSource), source_path);
    Parser intParser(intLexer);
    std::unique_ptr<LiteralExpression> int_lit = intParser.parseLiteralExpression();
    EXPECT_EQ(int_lit->getToken().getLexeme(), "0");
    EXPECT_EQ(int_lit->getLiteralType(), LiteralType::Integer);

    // Float literal
    std::string floatSource = "0.5";
    Lexer floatLexer(std::make_shared<std::string>(floatSource), source_path);
    Parser floatParser(floatLexer);
    std::unique_ptr<LiteralExpression> float_lit = floatParser.parseLiteralExpression();
    EXPECT_EQ(float_lit->getToken().getLexeme(), "0.5");
    EXPECT_EQ(float_lit->getLiteralType(), LiteralType::Float);

    // String literal
    std::string stringSource = "\"string\"";
    Lexer stringLexer(std::make_shared<std::string>(stringSource), source_path);
    Parser stringParser(stringLexer);
    std::unique_ptr<LiteralExpression> string_lit = stringParser.parseLiteralExpression();
    EXPECT_EQ(string_lit->getToken().getLexeme(), "string");
    EXPECT_EQ(string_lit->getLiteralType(), LiteralType::String);
}
