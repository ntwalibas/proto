#include <gtest/gtest.h>
#include <cstdbool>
#include <memory>

#include "ast/declarations/type.h"
#include "common/token_type.h"
#include "utils/inference.h"
#include "common/token.h"


TEST(createSimpleTypeDeclaration, createSimpleTypeDeclarationTest)
{
    std::unique_ptr<SimpleTypeDeclaration> type_decl = createSimpleTypeDeclaration(
        true,
        "bool"
    );
    EXPECT_EQ(type_decl->isConst(), true);
    EXPECT_EQ(type_decl->getToken().type, PROTO_IDENTIFIER);
    EXPECT_EQ(type_decl->getToken().getLexeme(), "bool");
    EXPECT_EQ(type_decl->getToken().source_path, "__builtin__");
}

TEST(createArrayTypeDeclaration, createArrayTypeDeclarationTest)
{
    std::unique_ptr<ArrayTypeDeclaration> type_decl = createArrayTypeDeclaration(
        false,
        1,
        "int32"
    );
    EXPECT_EQ(type_decl->isConst(), false);
    EXPECT_EQ(type_decl->getToken().type, PROTO_LEFT_BRACKET);
    EXPECT_EQ(type_decl->getSize(), 1);
    EXPECT_EQ(type_decl->getToken().getLexeme(), "[");
    EXPECT_EQ(type_decl->getToken().source_path, "__builtin__");
}
