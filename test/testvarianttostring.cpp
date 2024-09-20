#include <gtest/gtest.h>
#include <simplerttr.h>

using namespace SimpleRTTR;

SIMPLERTTR
{
    registration();
}

TEST(RTTRVariantToString, TestCharToString)
{
    char c('a');
    Variant var(c);

    EXPECT_EQ(var.type(), types().get_type<char>());

    std::string strValue = var.to_string();

    EXPECT_EQ(strValue, "a");
}


TEST(RTTRVariantToString, TestCharPointerToString)
{
    const char* str("This is a test!");
    Variant var(str);

    EXPECT_EQ(var.type(), types().get_type<const char*>());

    std::string strValue = var.to_string();

    EXPECT_EQ(strValue, "This is a test!");
}

TEST(RTTRVariantToString, TestShortToString)
{
    short i(42);
    Variant var(i);

    EXPECT_EQ(var.type(), types().get_type<short>());

    std::string strValue = var.to_string();

    EXPECT_EQ(strValue, "42");
}

TEST(RTTRVariantToString, TestIntToString)
{
    int i(42);
    Variant var(i);

    EXPECT_EQ(var.type(), types().get_type<int>());

    std::string strValue = var.to_string();

    EXPECT_EQ(strValue, "42");
}

TEST(RTTRVariantToString, TestInitializerListListOfIntsToString)
{
    std::initializer_list<int> vec = { 1, 42, -12 };
    Variant var(vec);

    EXPECT_EQ(var.type(), types().get_or_create_type<std::initializer_list<int>>());

    std::string strValue = var.to_string();

    EXPECT_EQ(strValue, "{ 1, 42, -12 }");
}

TEST(RTTRVariantToString, TestVectorListOfIntsToString)
{
    std::vector<int> vec({ 1, 42, -12 });
    Variant var(vec);

    EXPECT_EQ(var.type(), types().get_type<std::vector<int>>());

    std::string strValue = var.to_string();

    EXPECT_EQ(strValue, "{ 1, 42, -12 }");
}
