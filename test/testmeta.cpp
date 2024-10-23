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
    registration().type<SimpleRTTRTestMeta>()
        .meta("This is a key", "and this is the value")
        .meta(42, "here is an int as a key")
        .meta("here is an int as a value", 32)
        .meta("this should resolve to std::vector<int>", { 1, 2, 3, 4 })
        .meta("and here's a function pointer", &SimpleRTTRTestMeta::someFunc)

        .property(&SimpleRTTRTestMeta::property1, "property1")
            .meta("This is a key", "and this is the value")
            .meta(42, "here is an int as a key")
            .meta("here is an int as a value", 32)
            .meta("this should resolve to std::vector<int>", { 1, 2, 3, 4 })
        .property(&SimpleRTTRTestMeta::property2, "property2")
        .property(&SimpleRTTRTestMeta::property3, "property3")

        .method(&SimpleRTTRTestMeta::someFunc, "someFunc", { "p1", "p2" })
            .meta("description", "this function does something");
}

TEST(RTTRMeta, TestTypeHasMeta)
{
    Type type = types().get_type<SimpleRTTRTestMeta>();
    ASSERT_GE(type.meta().size(), 4);
}

TEST(RTTRMeta, TestInvalidMeta)
{
    Type type = types().get_type<SimpleRTTRTestMeta>();
    const Meta& meta = type.meta().get("This value doesn't exist");
    ASSERT_EQ(meta, Meta::invalid_meta());
}


TEST(RTTRMeta, TestMetaStringKeyStringValue)
{
    Type type = types().get_type<SimpleRTTRTestMeta>();

    std::string stringKey;
    std::string stringValue;

    const Meta& meta = type.meta().get("This is a key");
    ASSERT_NE(meta, Meta::invalid_meta());

    EXPECT_EQ(meta.key().type().name(), "char const*");
    EXPECT_EQ(meta.key().type(), types().get_type<char const *>());
    ASSERT_NO_THROW(stringKey = meta.key().get_as<const char*>());

    EXPECT_EQ(meta.value().type().name(), "char const*");
    EXPECT_EQ(meta.value().type(), types().get_type<char const*>());
    ASSERT_NO_THROW(stringValue = meta.value().get_as<const char*>());

    EXPECT_EQ(stringKey, "This is a key");
    EXPECT_EQ(stringValue, "and this is the value");
}

TEST(RTTRMeta, TestMetaIntKeyStringValue)
{
    Type type = types().get_type<SimpleRTTRTestMeta>();

    int intKey;
    std::string stringValue;

    const Meta& meta = type.meta().get(42);
    ASSERT_NE(meta, Meta::invalid_meta());

    EXPECT_EQ(meta.key().type().name(), "int");
    EXPECT_EQ(meta.key().type(), types().get_type<int>());
    ASSERT_NO_THROW(intKey = meta.key().get_as<int>());

    EXPECT_EQ(meta.value().type().name(), "char const*");
    EXPECT_EQ(meta.value().type(), types().get_type<char const*>());
    ASSERT_NO_THROW(stringValue = meta.value().get_as<const char*>());

    EXPECT_EQ(intKey, 42);
    EXPECT_EQ(stringValue, "here is an int as a key");
}

TEST(RTTRMeta, TestMetaStringKeyIntValue)
{
    Type type = types().get_type<SimpleRTTRTestMeta>();

    std::string stringKey;
    int intValue;

    const Meta& meta = type.meta().get("here is an int as a value");
    ASSERT_NE(meta, Meta::invalid_meta());

    EXPECT_EQ(meta.key().type().name(), "char const*");
    EXPECT_EQ(meta.key().type(), types().get_type<char const*>());
    ASSERT_NO_THROW(stringKey = meta.key().get_as<const char*>());

    EXPECT_EQ(meta.value().type().name(), "int");
    EXPECT_EQ(meta.value().type(), types().get_type<int>());
    ASSERT_NO_THROW(intValue = meta.value().get_as<int>());

    EXPECT_EQ(stringKey, "here is an int as a value");
    EXPECT_EQ(intValue, 32);
}

TEST(RTTRMeta, TestMetaStringKeyListValue)
{
    Type type = types().get_type<SimpleRTTRTestMeta>();

    std::string stringKey;

    const Meta& meta = type.meta().get("this should resolve to std::vector<int>");
    ASSERT_NE(meta, Meta::invalid_meta());

    const std::vector<int>& vectorValue = meta.value().get_as<const std::vector<int>>();

    EXPECT_EQ(meta.key().type().name(), "char const*");
    EXPECT_EQ(meta.key().type(), types().get_type<char const*>());
    ASSERT_NO_THROW(stringKey = meta.key().get_as<const char*>());

    EXPECT_EQ(stringKey, "this should resolve to std::vector<int>");

    EXPECT_EQ(meta.value().type(), types().get_type<std::vector<int>>());
    ASSERT_EQ(vectorValue.size(), 4);

    EXPECT_EQ(vectorValue[0], 1);
    EXPECT_EQ(vectorValue[1], 2);
    EXPECT_EQ(vectorValue[2], 3);
    EXPECT_EQ(vectorValue[3], 4);
}

TEST(RTTRMeta, TestMetaStringKeyFuncPointerValue)
{
    Type type = types().get_type<SimpleRTTRTestMeta>();
    const Meta& meta = type.meta().get("and here's a function pointer");
    ASSERT_NE(meta, Meta::invalid_meta());

    EXPECT_EQ(meta.value(), &SimpleRTTRTestMeta::someFunc);
}


TEST(RTTRMeta, TestMetaOnFundamentalTypes)
{
    {
        //verify that we haven't touched the meta on the "char" type
        Type type = types().get_type<char>();
        EXPECT_EQ(type.meta().size(), 0);
    }

    //register our own metadata on the char type
    registration().type<char>()
        .meta("type", "fundamental");

    Type type = types().get_type<char>();
    EXPECT_EQ(type.meta().size(), 1);
}

TEST(RTTRMeta, TestMetaOnClassMethod)
{
    //check that the method has the correct metadata
    Type type = types().get_type<SimpleRTTRTestMeta>();
    const Method& method = type.methods().get("someFunc");
    ASSERT_NE(method, Method::invalid_method());
    const Meta& meta = method.meta().get("description");
    ASSERT_NE(meta, Meta::invalid_meta());
    EXPECT_STREQ(meta.value().get_as<const char*>(), "this function does something");
}
