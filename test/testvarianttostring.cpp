#include <gtest/gtest.h>
#include <SimpleRTTR.h>

using namespace SimpleRTTR;

SIMPLERTTR
{
    Registration();
}

TEST(RTTRVariantToString, TestCharToString)
{
    char c('a');
    Variant var(c);

    EXPECT_EQ(var.Type(), Types().GetType<char>());

    std::string strValue = var.ToString();

    EXPECT_EQ(strValue, "a");
}


TEST(RTTRVariantToString, TestCharPointerToString)
{
    const char* str("This is a test!");
    Variant var(str);

    EXPECT_EQ(var.Type(), Types().GetType<const char*>());

    std::string strValue = var.ToString();

    EXPECT_EQ(strValue, "This is a test!");
}

TEST(RTTRVariantToString, TestShortToString)
{
    short i(42);
    Variant var(i);

    EXPECT_EQ(var.Type(), Types().GetType<short>());

    std::string strValue = var.ToString();

    EXPECT_EQ(strValue, "42");
}

TEST(RTTRVariantToString, TestIntToString)
{
    int i(42);
    Variant var(i);

    EXPECT_EQ(var.Type(), Types().GetType<int>());

    std::string strValue = var.ToString();

    EXPECT_EQ(strValue, "42");
}

TEST(RTTRVariantToString, TestInitializerListListOfIntsToString)
{
    std::initializer_list<int> vec = { 1, 42, -12 };
    Variant var(vec);

    EXPECT_EQ(var.Type(), Types().GetOrCreateType<std::initializer_list<int>>());

    std::string strValue = var.ToString();

    EXPECT_EQ(strValue, "{ 1, 42, -12 }");
}

TEST(RTTRVariantToString, TestVectorListOfIntsToString)
{
    std::vector<int> vec({ 1, 42, -12 });
    Variant var(vec);

    EXPECT_EQ(var.Type(), Types().GetType<std::vector<int>>());

    std::string strValue = var.ToString();

    EXPECT_EQ(strValue, "{ 1, 42, -12 }");
}
