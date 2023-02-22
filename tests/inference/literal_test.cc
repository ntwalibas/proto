#include <gtest/gtest.h>
#include <cstddef>
#include <utility>
#include <memory>
#include <string>

#include "ast/declarations/declaration.h"
#include "ast/expressions/literal.h"
#include "inference/literal.h"
#include "common/token_type.h"
#include "common/token.h"


class LiteralTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
};

TEST_F(LiteralTest, inferTest) {
    std::shared_ptr<std::string> source = nullptr;

    // Test inference for boolean literals
    {
        std::string expr_str = "True";
        source = std::make_shared<std::string>(expr_str);
        Token expr_tok(
            PROTO_IDENTIFIER,
            source,
            "__testing__",
            source->begin(),
            source->length(),
            0,
            0
        );

        std::unique_ptr<LiteralExpression> lit_expr =
            std::make_unique<LiteralExpression>(expr_tok, LiteralType::Boolean);
        
        LiteralInference expr_inference(lit_expr);
        expr_inference.infer();

        std::unique_ptr<TypeDeclaration>& expr_type =
            lit_expr->getTypeDeclaration();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            dynamic_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getToken().getLexeme(), "bool");
    }

    // Test inference for integer literals
    {
        std::string expr_str = "100";
        source = std::make_shared<std::string>(expr_str);
        Token expr_tok(
            PROTO_IDENTIFIER,
            source,
            "__testing__",
            source->begin(),
            source->length(),
            0,
            0
        );

        std::unique_ptr<LiteralExpression> lit_expr =
            std::make_unique<LiteralExpression>(expr_tok, LiteralType::Integer);
        
        LiteralInference expr_inference(lit_expr);
        expr_inference.infer();

        std::unique_ptr<TypeDeclaration>& expr_type =
            lit_expr->getTypeDeclaration();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            dynamic_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getToken().getLexeme(), "uint64");
    }

    // Test inference for float literals
    {
        std::string expr_str = "0.5";
        source = std::make_shared<std::string>(expr_str);
        Token expr_tok(
            PROTO_IDENTIFIER,
            source,
            "__testing__",
            source->begin(),
            source->length(),
            0,
            0
        );

        std::unique_ptr<LiteralExpression> lit_expr =
            std::make_unique<LiteralExpression>(expr_tok, LiteralType::Float);
        
        LiteralInference expr_inference(lit_expr);
        expr_inference.infer();

        std::unique_ptr<TypeDeclaration>& expr_type =
            lit_expr->getTypeDeclaration();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            dynamic_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getToken().getLexeme(), "float64");
    }

    // Test inference for string literals
    {
        std::string expr_str = "Hello World!";
        source = std::make_shared<std::string>(expr_str);
        Token expr_tok(
            PROTO_IDENTIFIER,
            source,
            "__testing__",
            source->begin(),
            source->length(),
            0,
            0
        );

        std::unique_ptr<LiteralExpression> lit_expr =
            std::make_unique<LiteralExpression>(expr_tok, LiteralType::String);
        
        LiteralInference expr_inference(lit_expr);

        std::unique_ptr<TypeDeclaration>& expr_type =
            expr_inference.infer();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            dynamic_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getToken().getLexeme(), "string");
    }
}
