#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <exception>
#include <cstddef>
#include <utility>
#include <cstddef>
#include <memory>

#include "checker/ast/definitions/variable.h"
#include "ast/definitions/definition.h"
#include "ast/definitions/variable.h"
#include "checker/checker_error.h"
#include "symbols/scope.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class VariableDefinitionCheckerTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
        std::shared_ptr<Scope> scope = std::make_shared<Scope>(nullptr);
};

TEST_F(VariableDefinitionCheckerTest, checkTest)
{
    // Correct variable definition
    {
        std::string source = "count: int = 0";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        VariableDefinition* var_def = static_cast<VariableDefinition*>(def.get());

        VariableDefinitionChecker checker(var_def, scope);
        EXPECT_NO_THROW(checker.check());
    }
    
    // Type mismatch between variable type and initializer
    {
        std::string source = "count: uint = True";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        VariableDefinition* var_def = static_cast<VariableDefinition*>(def.get());

        VariableDefinitionChecker checker(var_def, scope);
        EXPECT_THROW({
            try {
                checker.check();
            } catch (CheckerError const& e) {
                EXPECT_STREQ(e.getPrimaryMessage(), "mismatched types");
                EXPECT_THAT(e.getSecondaryMessage(), ::testing::HasSubstr("[uint]"));
                EXPECT_THAT(e.getSecondaryMessage(), ::testing::HasSubstr("[bool]"));
                
                throw;
            }
        }, CheckerError);
    }
}
