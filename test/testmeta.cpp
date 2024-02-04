#include <gtest/gtest.h>
#include <simplerttr.h>

#include <array>

class SimpleRTTRTestMeta
{
public:
    char property1;
    char property2;
    char property3;

    void someFunc(int p1, int p2) { };
};

using namespace SimpleRTTR;

SIMPLERTTR
{
    Registration().Type<SimpleRTTRTestMeta>()
        .Meta("This is a key", "and this is the value")
        .Meta(42, "here is an int as a key")
        .Meta("here is an int as a value", 32)
        .Meta("this should resolve to std::vector<int>", { 1, 2, 3, 4 })
        .Meta("and here's a function pointer", &SimpleRTTRTestMeta::someFunc)

        .Property(&SimpleRTTRTestMeta::property1, "property1")
            .Meta("This is a key", "and this is the value")
            .Meta(42, "here is an int as a key")
            .Meta("here is an int as a value", 32)
            .Meta("this should resolve to std::vector<int>", { 1, 2, 3, 4 })
        .Property(&SimpleRTTRTestMeta::property2, "property2")
        .Property(&SimpleRTTRTestMeta::property3, "property3")

        .Method(&SimpleRTTRTestMeta::someFunc, "someFunc", { "p1", "p2" })
            .Meta("description", "this function does something");
}

TEST(RTTRMeta, TestTypeHasMeta)
{
    Type type = Types().GetType<SimpleRTTRTestMeta>();
    ASSERT_GE(type.Meta().Size(), 4);
}

TEST(RTTRMeta, TestInvalidMeta)
{
    Type type = Types().GetType<SimpleRTTRTestMeta>();
    const Meta& meta = type.Meta().Get("This value doesn't exist");
    ASSERT_EQ(meta, Meta::InvalidMeta());
}


TEST(RTTRMeta, TestMetaStringKeyStringValue)
{
    Type type = Types().GetType<SimpleRTTRTestMeta>();

    std::string stringKey;
    std::string stringValue;

    const Meta& meta = type.Meta().Get("This is a key");
    ASSERT_NE(meta, Meta::InvalidMeta());

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

    const Meta& meta = type.Meta().Get(42);
    ASSERT_NE(meta, Meta::InvalidMeta());

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

    const Meta& meta = type.Meta().Get("here is an int as a value");
    ASSERT_NE(meta, Meta::InvalidMeta());

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

    const Meta& meta = type.Meta().Get("this should resolve to std::vector<int>");
    ASSERT_NE(meta, Meta::InvalidMeta());

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

TEST(RTTRMeta, TestMetaStringKeyFuncPointerValue)
{
    Type type = Types().GetType<SimpleRTTRTestMeta>();
    const Meta& meta = type.Meta().Get("and here's a function pointer");
    ASSERT_NE(meta, Meta::InvalidMeta());

    EXPECT_EQ(meta.Value(), &SimpleRTTRTestMeta::someFunc);
}


TEST(RTTRMeta, TestMetaOnFundamentalTypes)
{
    {
        //verify that we haven't touched the meta on the "char" type
        Type type = Types().GetType<char>();
        EXPECT_EQ(type.Meta().Size(), 0);
    }

    //register our own metadata on the char type
    Registration().Type<char>()
        .Meta("type", "fundamental");

    Type type = Types().GetType<char>();
    EXPECT_EQ(type.Meta().Size(), 1);
}

TEST(RTTRMeta, TestMetaOnClassMethod)
{
    //check that the method has the correct metadata
    Type type = Types().GetType<SimpleRTTRTestMeta>();
    const Method& method = type.Methods().Get("someFunc");
    ASSERT_NE(method, Method::InvalidMethod());
    const Meta& meta = method.Meta().Get("description");
    ASSERT_NE(meta, Meta::InvalidMeta());
    EXPECT_STREQ(meta.Value().GetAs<const char*>(), "this function does something");
}