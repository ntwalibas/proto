#include <gtest/gtest.h>
#include <exception>
#include <cstddef>
#include <memory>

#include "ast/definitions/definition.h"
#include "checker/scope.h"


class ScopeTest: public ::testing::Test
{
    protected:
        void SetUp() override {
            parent_scope = std::make_shared<Scope>(nullptr);
            child_scope = std::make_shared<Scope>(parent_scope);

            parent_scope->addDefinition("var", var);
            child_scope->addDefinition("fun", fun);
        }

        void TearDown() override {
        }

        std::shared_ptr<Scope> parent_scope;
        std::shared_ptr<Scope> child_scope;
        std::unique_ptr<Definition> var = std::make_unique<Definition>(DefinitionType::Variable);
        std::unique_ptr<Definition> fun = std::make_unique<Definition>(DefinitionType::Function);
};

TEST_F(ScopeTest, getDefinitionTest)
{
    // We can retrieve definitions in the parent scope
    EXPECT_EQ(parent_scope->getDefinition("var")->getType(), DefinitionType::Variable);

    // We can't retrieve definitions in child scope given parent sope
    EXPECT_THROW(parent_scope->getDefinition("fun", true), std::out_of_range);

    // We can retrieve definitions in the child scope
    EXPECT_EQ(child_scope->getDefinition("fun")->getType(), DefinitionType::Function);

    // If the definition doesn't exist in the child but in the parent scope,
    // we can still retrieve it
    EXPECT_EQ(child_scope->getDefinition("var", true)->getType(), DefinitionType::Variable);

    // If the definition doesn't exist in either child or parent, we can't retrieve it
    EXPECT_THROW(child_scope->getDefinition("nil", true), std::out_of_range);
}

TEST_F(ScopeTest, hasDefinitionTest)
{
    // We can find definitions in parent scope
    EXPECT_EQ(parent_scope->hasDefinition("var"), true);

    // We can't find definitions in child scope given parent sope
    EXPECT_EQ(parent_scope->hasDefinition("fun", true), false);

    // We can find definitions in the child scope
    EXPECT_EQ(child_scope->hasDefinition("fun"), true);

    // We can find definitions in parent scope from child scope
    EXPECT_EQ(child_scope->hasDefinition("var", true), true);

    // We can't find definitions that were never added
    EXPECT_EQ(child_scope->hasDefinition("nil", true), false);
}
