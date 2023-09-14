#include <gtest/gtest.h>
#include <SimpleRTTR.h>

using namespace SimpleRTTR;

class SimpleRTTRTestProperties1
{
public:
    char            charMember;
    short           shortMember;
    int             intMember;
    void*           voidpointerMember;
};

int someFunc(int x, int y) 
{
    return 0;
}

SIMPLERTTR
{
    Registration().Type<SimpleRTTRTestProperties1>()
        .Property(&SimpleRTTRTestProperties1::charMember, "charMember")
            .Meta("description", "a simple 1 byte type")
            .Meta("numbers", {1,2,3,4,5})
        .Property(&SimpleRTTRTestProperties1::shortMember, "shortMember")
            .Meta("description", "a simple 2 byte type")
            .Meta("callback", someFunc)
        .Property(&SimpleRTTRTestProperties1::intMember, "intMember")
            .Meta("description", "a simple 4 byte type")
        .Property(&SimpleRTTRTestProperties1::voidpointerMember, "voidpointerMember");
}

TEST(RTTRProperties, TestPropertiesList)
{
    Type type = Types().GetType<SimpleRTTRTestProperties1>();
    ASSERT_NE(type, Type::InvalidType());

    const Type::PropertyList& properties = type.Properties();
    ASSERT_EQ(properties.size(), 4);

    const Property& prop1 = properties[0];
    EXPECT_EQ(prop1.Name(), "charMember");
    EXPECT_EQ(prop1.Type(), Types().GetType<char>());
    EXPECT_EQ(prop1.Offset(), 0);

    const Property& prop2 = properties[1];
    EXPECT_EQ(prop2.Name(), "shortMember");
    EXPECT_EQ(prop2.Type(), Types().GetType<short>());
    EXPECT_EQ(prop2.Offset(), 2);

    const Property& prop3 = properties[2];
    EXPECT_EQ(prop3.Name(), "intMember");
    EXPECT_EQ(prop3.Type(), Types().GetType<int>());
    EXPECT_EQ(prop3.Offset(), 4);

    const Property& prop4 = properties[3];
    EXPECT_EQ(prop4.Name(), "voidpointerMember");
    EXPECT_EQ(prop4.Type(), Types().GetType<void*>());
    EXPECT_EQ(prop4.Offset(), 8);

}

TEST(RTTRProperties, TestPropertiesMeta)
{
    Type type = Types().GetType<SimpleRTTRTestProperties1>();
    ASSERT_NE(type, Type::InvalidType());

    const Type::PropertyList& properties = type.Properties();
    ASSERT_EQ(properties.size(), 4);

    const Property& prop1 = properties[0];
    {
        const Property::MetaList meta = prop1.Meta();
        ASSERT_EQ(meta.size(), 2);

        {
            EXPECT_EQ(meta[0].Key().Type(), Types().GetType<const char*>());
            EXPECT_EQ(meta[0].Key().ToString(), "description");
            EXPECT_EQ(meta[0].Key().GetAs<const char*>(), std::string("description"));
            EXPECT_EQ(meta[0].Value().Type(), Types().GetType<const char*>());
            EXPECT_EQ(meta[0].Value().ToString(), "a simple 1 byte type");
            EXPECT_EQ(meta[0].Value().GetAs<const char*>(), std::string("a simple 1 byte type"));
        }

        {
            EXPECT_EQ(meta[1].Key().Type(), Types().GetType<const char*>());
            EXPECT_EQ(meta[1].Key().ToString(), "numbers");
            EXPECT_EQ(meta[1].Key().GetAs<const char*>(), std::string("numbers"));
            EXPECT_EQ(meta[1].Value().Type(), Types().GetType<std::vector<int>>());
            std::vector<int> values = meta[1].Value().GetAs<std::vector<int>>();
            ASSERT_EQ(values.size(), 5);
            EXPECT_EQ(values[0], 1);
            EXPECT_EQ(values[1], 2);
            EXPECT_EQ(values[2], 3);
            EXPECT_EQ(values[3], 4);
            EXPECT_EQ(values[4], 5);
        }

    }

    const Property& prop2 = properties[1];
    const Property::MetaList meta2 = prop2.Meta();
}