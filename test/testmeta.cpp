#include <gtest/gtest.h>
#include <simplerttr.h>

#include <array>

class SimpleRTTRTestMeta
{
public:
    char property1;
    char property2;
    char property3;
};

using namespace SimpleRTTR;

SIMPLERTTR
{
    Registration().Type<SimpleRTTRTestMeta>()
        .Meta("This is a key", "and this is the value")
        .Meta(42, "here is an int as a key")
        .Meta("here is an int as a value", 32)
        .Meta("this should resolve to std::vector<int>", { 1, 2, 3, 4 })

        .Property(&SimpleRTTRTestMeta::property1, "property1")
            .Meta("This is a key", "and this is the value")
            .Meta(42, "here is an int as a key")
            .Meta("here is an int as a value", 32)
            .Meta("this should resolve to std::vector<int>", { 1, 2, 3, 4 })
        .Property(&SimpleRTTRTestMeta::property2, "property2")
        .Property(&SimpleRTTRTestMeta::property3, "property3");
}

TEST(RTTRMeta, TestTypeHasMeta)
{
    Type type = Types().GetType<SimpleRTTRTestMeta>();
    ASSERT_GE(type.Meta().size(), 4);
}

TEST(RTTRMeta, TestMetaStringKeyStringValue)
{
    Type type = Types().GetType<SimpleRTTRTestMeta>();

    std::string stringKey;
    std::string stringValue;

    const Meta& meta = type.Meta()[0];
    EXPECT_EQ(meta.Key().Type().Name(), "char const *");
    EXPECT_EQ(meta.Key().Type(), Types().GetType<char const *>());
    ASSERT_NO_THROW(stringKey = meta.Key().GetAs<const char*>());

    EXPECT_EQ(meta.Value().Type().Name(), "char const *");
    EXPECT_EQ(meta.Value().Type(), Types().GetType<char const*>());
    ASSERT_NO_THROW(stringValue = meta.Value().GetAs<const char*>());

    EXPECT_EQ(stringKey, "This is a key");
    EXPECT_EQ(stringValue, "and this is the value");
}

TEST(RTTRMeta, TestMetaIntKeyStringValue)
{
    Type type = Types().GetType<SimpleRTTRTestMeta>();

    int intKey;
    std::string stringValue;

    const Meta& meta = type.Meta()[1];

    EXPECT_EQ(meta.Key().Type().Name(), "int");
    EXPECT_EQ(meta.Key().Type(), Types().GetType<int>());
    ASSERT_NO_THROW(intKey = meta.Key().GetAs<int>());

    EXPECT_EQ(meta.Value().Type().Name(), "char const *");
    EXPECT_EQ(meta.Value().Type(), Types().GetType<char const*>());
    ASSERT_NO_THROW(stringValue = meta.Value().GetAs<const char*>());

    EXPECT_EQ(intKey, 42);
    EXPECT_EQ(stringValue, "here is an int as a key");
}

TEST(RTTRMeta, TestMetaStringKeyIntValue)
{
    Type type = Types().GetType<SimpleRTTRTestMeta>();

    std::string stringKey;
    int intValue;

    const Meta& meta = type.Meta()[2];

    EXPECT_EQ(meta.Key().Type().Name(), "char const *");
    EXPECT_EQ(meta.Key().Type(), Types().GetType<char const*>());
    ASSERT_NO_THROW(stringKey = meta.Key().GetAs<const char*>());

    EXPECT_EQ(meta.Value().Type().Name(), "int");
    EXPECT_EQ(meta.Value().Type(), Types().GetType<int>());
    ASSERT_NO_THROW(intValue = meta.Value().GetAs<int>());

    EXPECT_EQ(stringKey, "here is an int as a value");
    EXPECT_EQ(intValue, 32);
}

TEST(RTTRMeta, TestMetaStringKeyListValue)
{
    Type type = Types().GetType<SimpleRTTRTestMeta>();

    std::string stringKey;

    const Meta& meta = type.Meta()[3];
    const std::vector<int>& vectorValue = meta.Value().GetAs<const std::vector<int>&>();

    EXPECT_EQ(meta.Key().Type().Name(), "char const *");
    EXPECT_EQ(meta.Key().Type(), Types().GetType<char const*>());
    ASSERT_NO_THROW(stringKey = meta.Key().GetAs<const char*>());

    EXPECT_EQ(stringKey, "this should resolve to std::vector<int>");

    EXPECT_EQ(meta.Value().Type(), Types().GetType<std::vector<int>>());
    ASSERT_EQ(vectorValue.size(), 4);

    EXPECT_EQ(vectorValue[0], 1);
    EXPECT_EQ(vectorValue[1], 2);
    EXPECT_EQ(vectorValue[2], 3);
    EXPECT_EQ(vectorValue[3], 4);
}

TEST(RTTRMeta, TestMetaOnFundamentalTypes)
{
    {
        //verify that we haven't touched the meta on the "char" type
        Type type = Types().GetType<char>();
        EXPECT_EQ(type.Meta().size(), 0);
    }

    //register our own metadata on the char type
    Registration().Type<char>()
        .Meta("type", "fundamental");

    Type type = Types().GetType<char>();
    EXPECT_EQ(type.Meta().size(), 1);
}