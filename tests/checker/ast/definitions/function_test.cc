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
            std::make_shared<std::string>("sum1: function(a: uint, b:uint) -> uint{}");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        FunctionDefinition* fun_def = static_cast<FunctionDefinition*>(def.get());

        FunctionDefinitionChecker checker(fun_def, scope);
        EXPECT_NO_THROW(checker.check(def));
    }

    // We have an invalid type in the parameters
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("sum2: function(a: uint32, b:uint) -> uint{}");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        FunctionDefinition* fun_def = static_cast<FunctionDefinition*>(def.release());

        FunctionDefinitionChecker checker(fun_def, scope);
        EXPECT_THROW(checker.check(def), CheckerError);
    }

    // We have an invalid return type
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("sum3: function(a: uint, b:uint) -> int32{}");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        FunctionDefinition* fun_def = static_cast<FunctionDefinition*>(def.get());

        FunctionDefinitionChecker checker(fun_def, scope);
        EXPECT_THROW(checker.check(def), CheckerError);
    }

    // Header is valid, and body is valid
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("sum4: function(a: uint, b:uint) -> uint{ a + b\n }");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        FunctionDefinition* fun_def = static_cast<FunctionDefinition*>(def.get());

        FunctionDefinitionChecker checker(fun_def, scope);
        EXPECT_NO_THROW(checker.check(def));
    }

    // Header is valid, and body is invalid
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("sum5: function(a: uint, b:uint) -> uint{ inner: function()->void{}\n }");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        FunctionDefinition* fun_def = static_cast<FunctionDefinition*>(def.get());

        FunctionDefinitionChecker checker(fun_def, scope);
        EXPECT_THROW(checker.check(def), CheckerError);
    }
}