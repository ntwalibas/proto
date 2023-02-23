#include <gtest/gtest.h>
#include <exception>
#include <cstddef>
#include <utility>
#include <memory>
#include <string>

#include "ast/declarations/declaration.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/literal.h"
#include "inference/inference.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class InferenceTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
    
    std::string source_path = "main.pro";
};

TEST_F(InferenceTest, inferTest) {
}

TEST_F(InferenceTest, inferLiteralTypeTest) {
    std::shared_ptr<std::string> source = nullptr;

    // Booleans literals
    {
        std::string source = "True";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr).inferLiteralType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "bool");
    }

    // Integer literals
    {
        std::string source = "0";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr).inferLiteralType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint64");
    }

    // Float literals
    {
        std::string source = "0.5";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr).inferLiteralType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "float64");
    }

    // String literals
    {
        std::string source = "\"Hello World!\"";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr).inferLiteralType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "string");
    }
}

TEST_F(InferenceTest, inferArrayTypeTest) {
    std::shared_ptr<std::string> source = nullptr;

    // Well-formed array
    {
        std::string source = "[0, 1, 2, 3]";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr).inferArrayType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Array);
        ArrayTypeDeclaration& type_decl =
            static_cast<ArrayTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "[4]uint64");
    }

    // Empty array
    {
        std::string source = "[]";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(
            Inference(expr).inferArrayType(),
            std::length_error
        );
    }

    // Non-uniform array
    {
        std::string source = "[0, True]";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(
            Inference(expr).inferArrayType(),
            std::invalid_argument
        );
    }

    // Array within array
    {
        std::string source = "[0, [True]]";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(
            Inference(expr).inferArrayType(),
            std::domain_error
        );
    }
}
