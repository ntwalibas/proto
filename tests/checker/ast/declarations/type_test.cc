#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "checker/ast/declarations/type.h"
#include "checker/checker_error.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class TypeDeclarationCheckerTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
};

TEST_F(TypeDeclarationCheckerTest, checkTest)
{
    // Bad simple type declaration
    {
        std::string source = "int32";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<TypeDeclaration> type_decl = parser.parseTypeDeclaration();
        TypeDeclarationChecker checker(type_decl);

        EXPECT_THROW({
            try {
                checker.check();
            } catch (CheckerError& e) {
                EXPECT_STREQ(e.getSecondaryMessage(), "type [int32] does not exist");
                throw;
            }
        }, CheckerError);
    }

    // Good simple type declaration
    {
        std::string source = "int";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<TypeDeclaration> type_decl = parser.parseTypeDeclaration();
        TypeDeclarationChecker checker(type_decl);
        EXPECT_NO_THROW(checker.check());
    }
}
