#include <gtest/gtest.h>
#include <cstddef>
#include <utility>
#include <memory>
#include <string>

#include "ast/declarations/declaration.h"
#include "ast/expressions/literal.h"
#include "inference/inference.h"
#include "common/token_type.h"
#include "common/token.h"


class InferenceTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
};

TEST_F(InferenceTest, inferTest) {
    std::shared_ptr<std::string> source = nullptr;

    // Test inference for literals
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
        std::unique_ptr<Expression> expr(std::move(lit_expr));

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference::infer(expr);
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getToken().getLexeme(), "bool");
    }
}
