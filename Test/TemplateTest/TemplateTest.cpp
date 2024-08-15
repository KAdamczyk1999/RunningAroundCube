#include "example_file.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace testing;

struct TemplateTest : public Test {
    void SetUp() {}
};

TEST_F(TemplateTest, CheckNumber) { EXPECT_EQ(functionToTest(), 42); }
