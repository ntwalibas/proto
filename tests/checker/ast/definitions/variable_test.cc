#include <gtest/gtest.h>
#include <exception>
#include <cstddef>
#include <utility>
#include <memory>
#include <string>

#include "checker/ast/definitions/variable.h"
#include "ast/definitions/variable.h"
#include "checker/checker_error.h"
#include "symbols/scope.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class VariableCheckerTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
    
    std::string source_path = "main.pro";
    std::shared_ptr<Scope> scope = std::make_shared<Scope>(nullptr);
};

TEST_F(VariableCheckerTest, inferVariableTypeTest) {
    // Variable definition in due form
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("count: int = 0");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        std::unique_ptr<VariableDefinition> var_def(
            static_cast<VariableDefinition*>(def.release())
        );

        VariableDefinitionChecker checker(var_def.get(), scope);
        EXPECT_NO_THROW(checker.check());
        def = std::move(var_def);
        scope->addDefinition("count", def);
    }

    // Variable definition already in scope
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("count: uint = 0");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        std::unique_ptr<VariableDefinition> var_def(
            static_cast<VariableDefinition*>(def.release())
        );

        VariableDefinitionChecker checker(var_def.get(), scope);
        EXPECT_THROW(checker.check(), CheckerError);
    }

    // Variable definition has an incorrect type
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("count: int = 0");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Definition> def = parser.parseDefinition();
        std::unique_ptr<VariableDefinition> var_def(
            static_cast<VariableDefinition*>(def.release())
        );

        VariableDefinitionChecker checker(var_def.get(), scope);
        EXPECT_THROW(checker.check(), CheckerError);
    }

    // Variable definition already in scope
    {
        std::shared_ptr<std::string> source =
            std::make_shared<std::string>("other: bool");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<VariableDeclaration> decl =
            parser.parseVariableDeclaration();
        scope->addVariableDeclaration("other", decl);

        std::shared_ptr<std::string> defSource =
            std::make_shared<std::string>("other: bool = True");
        Lexer defLexer(defSource, source_path);
        Parser defParser(defLexer);
        std::unique_ptr<Definition> def = defParser.parseDefinition();
        std::unique_ptr<VariableDefinition> var_def(
            static_cast<VariableDefinition*>(def.release())
        );

        VariableDefinitionChecker checker(var_def.get(), scope);
        EXPECT_THROW(checker.check(), CheckerError);
    }
}
