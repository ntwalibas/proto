#include <gtest/gtest.h>
#include <cstddef>
#include <memory>
#include <string>

#include "checker/ast/definitions/function.h"
#include "ast/definitions/function.h"
#include "checker/checker_error.h"
#include "symbols/scope.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class FunctionCheckerTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
    
    std::string source_path = "main.pro";
    std::shared_ptr<Scope> scope = std::make_shared<Scope>(nullptr);
};

TEST_F(FunctionCheckerTest, checkTest)
{
    // All parameters and return types are valid
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("sum: function(a: uint32, b:uint32) -> uint32{}");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        std::unique_ptr<FunctionDefinition> fun_def(
            static_cast<FunctionDefinition*>(def.release())
        );

        FunctionDefinitionChecker checker(fun_def.get(), scope);
        EXPECT_NO_THROW(checker.check());
    }

    // We have an invalid type in the parameters
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("sum: function(a: uint, b:uint32) -> uint32{}");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        std::unique_ptr<FunctionDefinition> fun_def(
            static_cast<FunctionDefinition*>(def.release())
        );

        FunctionDefinitionChecker checker(fun_def.get(), scope);
        EXPECT_THROW(checker.check(), CheckerError);
    }

    // We have an invalid return type
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("sum: function(a: uint32, b:uint32) -> int{}");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        std::unique_ptr<FunctionDefinition> fun_def(
            static_cast<FunctionDefinition*>(def.release())
        );

        FunctionDefinitionChecker checker(fun_def.get(), scope);
        EXPECT_THROW(checker.check(), CheckerError);
    }
}