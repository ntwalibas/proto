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
