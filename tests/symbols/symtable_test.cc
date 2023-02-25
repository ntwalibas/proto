#include <gtest/gtest.h>
#include <exception>
#include <cstddef>
#include <memory>
#include <string>

#include "ast/definitions/definition.h"
#include "symbols/symtable.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class SymtableTest: public ::testing::Test
{
    protected:
        void SetUp() override {
            std::string source = "count: bool = True";
            std::string source_path = "main.pro";
            Lexer lexer(std::make_shared<std::string>(source), source_path);
            Parser parser(lexer);
            var = parser.parseDefinition();
        }

        void TearDown() override {
            symtable.clear();
        }

        Symtable symtable;
        std::unique_ptr<Definition> var = nullptr;
        std::unique_ptr<Definition> nil = nullptr;
};

TEST_F(SymtableTest, addGetDefinitionsTest)
{
    symtable.addDefinition("var", var);
    symtable.addDefinition("nil", nil);

    EXPECT_EQ(symtable.getDefinitions().size(), 2);
    EXPECT_EQ(symtable.addDefinition("var", var), false);
}

TEST_F(SymtableTest, getDefinitionTest)
{
    symtable.addDefinition("var", var);
    symtable.addDefinition("nil", nil);

    EXPECT_NE(symtable.getDefinition("var"), nullptr);
    EXPECT_EQ(symtable.getDefinition("var")->getType(), DefinitionType::Variable);
    EXPECT_EQ(symtable.getDefinition("nil"), nullptr);

    EXPECT_THROW(symtable.getDefinition("fun"), std::out_of_range);
}

TEST_F(SymtableTest, hasDefinitionTest)
{
    symtable.addDefinition("var", var);

    EXPECT_EQ(symtable.hasDefinition("var"), true);
    EXPECT_EQ(symtable.hasDefinition("nil"), false);
}

TEST(BuiltinTypesSymtable, isBuiltinTypeTest)
{
    EXPECT_EQ(BuiltinTypesSymtable::isBuiltinType("bool"), true);
    EXPECT_EQ(BuiltinTypesSymtable::isBuiltinType("int"), false);
}
