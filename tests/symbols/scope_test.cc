#include <gtest/gtest.h>
#include <stdexcept>
#include <cstddef>
#include <memory>

#include "parsetree/definitions/definition.h"
#include "symbols/scope.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class ScopeTest: public ::testing::Test
{
    protected:
        void SetUp() override {
            parent_scope = std::make_shared<Scope>(nullptr);
            child_scope = std::make_shared<Scope>(parent_scope);

            {
                std::string source = "count: bool = true";
                std::string source_path = "main.pro";
                Lexer lexer(std::make_shared<std::string>(source), source_path);
                Parser parser(lexer);
                var_bool = parser.parseDefinition();
                parent_scope->addDefinition("var_bool", var_bool);
            }

            {
                std::string source = "count: uint = 0";
                std::string source_path = "main.pro";
                Lexer lexer(std::make_shared<std::string>(source), source_path);
                Parser parser(lexer);
                var_uint = parser.parseDefinition();
                child_scope->addDefinition("var_uint", var_uint);
            }
        }

        void TearDown() override {
        }

        std::shared_ptr<Scope> parent_scope;
        std::shared_ptr<Scope> child_scope;

        std::unique_ptr<Definition> var_bool = nullptr;
        std::unique_ptr<Definition> var_uint = nullptr;
};

TEST_F(ScopeTest, getDefinitionTest)
{
    // We can retrieve definitions in the parent scope
    EXPECT_EQ(parent_scope->getDefinition("var_bool")->getType(), DefinitionType::Variable);

    // We can't retrieve definitions in child scope given parent sope
    EXPECT_THROW(parent_scope->getDefinition("var_uint", true), std::out_of_range);

    // We can retrieve definitions in the child scope
    EXPECT_EQ(child_scope->getDefinition("var_uint")->getType(), DefinitionType::Variable);

    // If the definition doesn't exist in the child but in the parent scope,
    // we can still retrieve it
    EXPECT_EQ(child_scope->getDefinition("var_bool", true)->getType(), DefinitionType::Variable);

    // If the definition doesn't exist in either child or parent, we can't retrieve it
    EXPECT_THROW(child_scope->getDefinition("nil", true), std::out_of_range);
}

TEST_F(ScopeTest, hasDefinitionTest)
{
    // We can find definitions in parent scope
    EXPECT_EQ(parent_scope->hasDefinition("var_bool"), true);

    // We can't find definitions in child scope given parent sope
    EXPECT_EQ(parent_scope->hasDefinition("var_uint", true), false);

    // We can find definitions in the child scope
    EXPECT_EQ(child_scope->hasDefinition("var_uint"), true);

    // We can find definitions in parent scope from child scope
    EXPECT_EQ(child_scope->hasDefinition("var_bool", true), true);

    // We can't find definitions that were never added
    EXPECT_EQ(child_scope->hasDefinition("nil", true), false);
}
