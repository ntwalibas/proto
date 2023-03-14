#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>

#include "cleaner/symbols/scope.h"
#include "parsetree/program.h"
#include "cleaner/cleaner.h"
#include "checker/checker.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class CleanerTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
};

TEST_F(CleanerTest, cleanTest) {
    std::string source =
        "version: string = \"0.0.1\"\n"
        "sum: function(a: int, b: int) -> int {\n"
        "    return a + b\n"
        "}\n"
        "main: function() -> uint {\n"
        "    sum(-1, 1:int)\n"
        "    return 0\n"
        "}\n";

    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    Program prog = parser.parseProgram();
    Checker(prog).check();
    Cleaner cleaner(prog);
    std::shared_ptr<CleanScope> scope = cleaner.clean();
    EXPECT_EQ(cleaner.warnings.size(), 1);
    EXPECT_EQ(scope->fun_defs.symbols.size(), 2);
}
