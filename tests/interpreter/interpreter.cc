#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "interpreter/interpreter.h"
#include "cleaner/symbols/scope.h"
#include "cleaner/cleaner.h"
#include "checker/checker.h"
#include "parser/parser.h"
#include "lexer/lexer.h"


class InterpreterTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        std::string source_path = "main.pro";
};

TEST_F(InterpreterTest, interpretTest) {
    std::string source =
        "main: function() -> int {\n"
        "    return 12\n"
        "}\n";

    Lexer lexer(std::make_shared<std::string>(source), source_path);
    Parser parser(lexer);
    Program prog = parser.parseProgram();
    Checker(prog).check();
    Cleaner cleaner(prog);
    std::shared_ptr<CleanScope> scope = cleaner.clean();
    Interpreter interpreter(scope.get());
    EXPECT_EQ(interpreter.interpret(), 12);
}
