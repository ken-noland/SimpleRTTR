#include <gtest/gtest.h>
#include <SimpleRTTR.h>

using namespace SimpleRTTR;

TEST(RTTRTypeHelper, TestBasic)
{
    TypeHelper<int> typeHelper;
    EXPECT_EQ(typeHelper.Name(), "int");
}

TEST(RTTRTypeHelper, TestSTDVector)
{
    TypeHelper<std::vector<int>> typeHelper;
    EXPECT_EQ(typeHelper.Name(), "vector");
    EXPECT_EQ(typeHelper.TemplateParams().size(), 2);
}
