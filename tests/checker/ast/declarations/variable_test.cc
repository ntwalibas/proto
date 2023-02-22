#include <gtest/gtest.h>


class VariableDeclarationCheckerTest: public ::testing::Test
{
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }
};

// We do not test the variable declaration checker because
// all it does is invoke the type declaration checker.
