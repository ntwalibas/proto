#include <gtest/gtest.h>
#include <stdexcept>
#include <cstddef>
#include <utility>
#include <memory>
#include <string>

#include "ast/declarations/declaration.h"
#include "ast/definitions/definition.h"
#include "ast/expressions/expression.h"
#include "ast/declarations/variable.h"
#include "inference/inference_error.h"
#include "ast/expressions/literal.h"
#include "inference/inference.h"
#include "symbols/scope.h"
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
    std::shared_ptr<Scope> scope = std::make_shared<Scope>(nullptr);
    std::unique_ptr<Definition> var_def = nullptr;
    std::unique_ptr<VariableDeclaration> var_decl = nullptr;
    std::unique_ptr<Definition> fun_def = nullptr;
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
            Inference(expr.get(), scope).inferLiteralType();
        
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
            Inference(expr.get(), scope).inferLiteralType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }

    // Float literals
    {
        std::string source = "0.5";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferLiteralType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "float");
    }

    // String literals
    {
        std::string source = "\"Hello World!\"";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferLiteralType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "string");
    }
}

TEST_F(InferenceTest, inferCastTypeTest) {
    std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1:int");

    Lexer lexer(source, source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseExpression();

    std::unique_ptr<TypeDeclaration>& expr_type =
        Inference(expr.get(), scope).inferCastType();
    
    EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
    SimpleTypeDeclaration& type_decl =
        static_cast<SimpleTypeDeclaration&>(*expr_type);
    EXPECT_EQ(type_decl.getTypeName(), "int");
}

TEST_F(InferenceTest, inferVariableTypeTest) {
    // Variable definition
    {
        std::string source = "count: uint = 0";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        var_def.reset(nullptr);
        var_def = parser.parseDefinition();
        scope->addDefinition("count", var_def);
    }

    // Variable declaration
    {
        std::string source = "other: bool";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        var_decl = parser.parseVariableDeclaration();
        scope->addVariableDeclaration("other", var_decl);
    }

    // Variable definition is in scope
    {
        std::string source = "count";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferVariableType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }

    // Variable definition is not in scope
    {
        std::string source = "name";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(Inference(expr.get(), scope).inferVariableType(), InferenceError);
    }

    // Variable declaration is in scope
    {
        std::string source = "other";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferVariableType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "bool");
    }
}

TEST_F(InferenceTest, inferGroupTypeTest) {
    std::shared_ptr<std::string> source =
        std::make_shared<std::string>("(True)");

    Lexer lexer(source, source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseExpression();

    std::unique_ptr<TypeDeclaration>& expr_type =
        Inference(expr.get(), scope).inferGroupType();
    
    EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
    SimpleTypeDeclaration& type_decl =
        static_cast<SimpleTypeDeclaration&>(*expr_type);
    EXPECT_EQ(type_decl.getTypeName(), "bool");
}

TEST_F(InferenceTest, inferCallTypeTest) {
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("sum: function(a:uint, b: uint) -> uint{}");
        Lexer lexer(source, source_path);
        Parser parser(lexer);
        fun_def = parser.parseDefinition();
        scope->addDefinition("sum(uint,uint)", fun_def);
    }

    // Function was found
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("sum(2, 2)");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferCallType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }

    // Function not in scope
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("mul(2, 2)");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(Inference(expr.get(), scope).inferCallType(), InferenceError);
    }
}

TEST_F(InferenceTest, inferUnaryTypeTest) {
    // Unary positive: unsigned int
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("+10");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferUnaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }

    // Unary positive: signed int
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("+-10");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferUnaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }

    // Unary positive: float
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("+0.5");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferUnaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "float");
    }

    // Unary negative: unsigned int
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-10");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferUnaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }

    // Unary negative: signed int
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("--10");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferUnaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }

    // Unary negative: float
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-0.5");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferUnaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "float");
    }

    // Bitwise not: unsigned int
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("~10");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferUnaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }

    // Bitwise not: signed int
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("~(-10)");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferUnaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }

    // Logical not: bool
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("!True");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferUnaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "bool");
    }

    // Wrong type applied to logical not
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("!0");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        EXPECT_THROW(Inference(expr.get(), scope).inferUnaryType(), InferenceError);
    }
}

TEST_F(InferenceTest, inferBinaryTypeTest) {
    // Addition
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 + 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }

    // Substraction
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 - 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }

    // Multiplication
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 * 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-1 * -2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1.0 * 2.0");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "float");
    }

    // Division
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 / 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-1 / -2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1.0 / 2.0");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "float");
    }

    // Remainder
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 % 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-1 % -2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1.0 % 2.0");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "float");
    }

    // Power
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 ** 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-1 ** -2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "float");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1.0 ** 2.0");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "float");
    }

    // Power
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 ** 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-1 ** -2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "float");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1.0 ** 2.0");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "float");
    }

    // Bitwise and
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 & 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-1 & -2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }

    // Bitwise or
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 | 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-1 | -2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }

    // Bitwise xor
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 ^ 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-1 ^ -2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }

    // Left shift
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 << 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-1 << 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }

    // Right shift
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 >> 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-1 >> 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "int");
    }

    // Logical and
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("True && False");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "bool");
    }

    // Logical Or
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("True || False");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "bool");
    }

    // Equal
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("True == True");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "bool");
    }

    // Not Equal
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("0 != 1");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "bool");
    }

    // Greater than
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("2.0 > 1.0");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "bool");
    }

    // Greater or equal
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("2.0 > 2.0");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "bool");
    }

    // Less than
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("1 < 2");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "bool");
    }

    // Less or equal
    {
        std::shared_ptr<std::string> source =
        std::make_shared<std::string>("-2 <= -1");

        Lexer lexer(source, source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferBinaryType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "bool");
    }
}

TEST_F(InferenceTest, inferTernaryIfTypeTest) {
    std::shared_ptr<std::string> source =
        std::make_shared<std::string>("b == True ? 1:int <> -1");

    Lexer lexer(source, source_path);
    Parser parser(lexer);
    std::unique_ptr<Expression> expr = parser.parseExpression();

    std::unique_ptr<TypeDeclaration>& expr_type =
        Inference(expr.get(), scope).inferTernaryIfType();
    
    EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
    SimpleTypeDeclaration& type_decl =
        static_cast<SimpleTypeDeclaration&>(*expr_type);
    EXPECT_EQ(type_decl.getTypeName(), "int");
}

TEST_F(InferenceTest, inferAssignmentTypeTest) {
    {
        std::string source = "new_count: uint = 0";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        var_def.reset(nullptr);
        var_def = parser.parseDefinition();
        scope->addDefinition("new_count", var_def);
    }

    // Simple assignment
    {
        std::string source = "new_count = 1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferAssignmentType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }

    // Correct in-place assignment
    {
        std::string source = "new_count += 1";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();

        std::unique_ptr<TypeDeclaration>& expr_type =
            Inference(expr.get(), scope).inferAssignmentType();
        
        EXPECT_EQ(expr_type->getTypeCategory(), TypeCategory::Simple);
        SimpleTypeDeclaration& type_decl =
            static_cast<SimpleTypeDeclaration&>(*expr_type);
        EXPECT_EQ(type_decl.getTypeName(), "uint");
    }

    // Incorrect in-place assignment
    {
        std::string source = "new_count += True";
        Lexer lexer(std::make_shared<std::string>(source), source_path);
        Parser parser(lexer);
        std::unique_ptr<Expression> expr = parser.parseExpression();
        EXPECT_THROW(Inference(expr.get(), scope).inferAssignmentType(), InferenceError);
    }
}