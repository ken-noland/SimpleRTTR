#include <gtest/gtest.h>
#include <SimpleRTTR.h>

using namespace SimpleRTTR;


class SimpleRTTRTestComplexProperties1
{
public:
    int             intMember;
    std::string     stringMember;
};

class SimpleRTTRTestProperties1
{
    SIMPLE_RTTR_ALLOW_ACCESS;
public:
    char            charMember;
    short           shortMember;
    int             intMember;
    void*           voidpointerMember;

    SimpleRTTRTestComplexProperties1 complexProperties;
    //SimpleRTTRTestComplexProperties1& complexPropertiesRef; //references can not be passed to properties
    SimpleRTTRTestComplexProperties1* complexPropertiesPtr;

protected:
    int             protectedMember;

private:
    int             privateMember;
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
        .Property(&SimpleRTTRTestProperties1::voidpointerMember, "voidpointerMember")
        .Property(&SimpleRTTRTestProperties1::protectedMember, "protectedMember")
        .Property(&SimpleRTTRTestProperties1::privateMember, "privateMember")
        .Property(&SimpleRTTRTestProperties1::complexProperties, "complexProperties")
        .Property(&SimpleRTTRTestProperties1::complexPropertiesPtr, "complexPropertiesPtr");
    ;
}

TEST(RTTRProperties, TestPropertiesList)
{
    Type type = Types().GetType<SimpleRTTRTestProperties1>();
    ASSERT_NE(type, Type::InvalidType());

    const PropertyContainer& properties = type.Properties();
    ASSERT_EQ(properties.Size(), 8);

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


TEST(RTTRProperties, TestPropertiesGet)
{
    Type type = Types().GetType<SimpleRTTRTestProperties1>();
    ASSERT_NE(type, Type::InvalidType());

    SimpleRTTRTestProperties1 instance;
    instance.charMember = 5;
    instance.shortMember = 10;
    instance.intMember = 15;
    instance.voidpointerMember = (void*)0x1234;

    const PropertyContainer& properties = type.Properties();
    ASSERT_EQ(properties.Size(), 8);

    const Property& prop1 = properties[0];
    EXPECT_EQ(prop1.Get(&instance).GetAs<char>(), 5);

    const Property& prop2 = properties[1];
    EXPECT_EQ(prop2.Get(&instance).GetAs<short>(), 10);

    const Property& prop3 = properties[2];
    EXPECT_EQ(prop3.Get(&instance).GetAs<int>(), 15);

    const Property& prop4 = properties[3];
    EXPECT_EQ(prop4.Get(&instance).GetAs<void*>(), (void*)0x1234);
}

TEST(RTTRProperties, TestPropertiesSet)
{
    Type type = Types().GetType<SimpleRTTRTestProperties1>();
    ASSERT_NE(type, Type::InvalidType());

    SimpleRTTRTestProperties1 instance;
    instance.charMember = 0;
    instance.shortMember = 0;
    instance.intMember = 0;
    instance.voidpointerMember = nullptr;

    const PropertyContainer& properties = type.Properties();
    ASSERT_EQ(properties.Size(), 8);

    const Property& prop1 = properties[0];
    prop1.Set(&instance, (char)5);
    EXPECT_EQ(instance.charMember, 5);

    const Property& prop2 = properties[1];
    prop2.Set(&instance, (short)10);
    EXPECT_EQ(instance.shortMember, 10);

    const Property& prop3 = properties[2];
    prop3.Set(&instance, (int)15);
    EXPECT_EQ(instance.intMember, 15);

    const Property& prop4 = properties[3];
    prop4.Set(&instance, (void*)0x1234);
    EXPECT_EQ(instance.voidpointerMember, (void*)0x1234);
}



TEST(RTTRProperties, TestPropertiesMeta)
{
    Type type = Types().GetType<SimpleRTTRTestProperties1>();
    ASSERT_NE(type, Type::InvalidType());

    const PropertyContainer& properties = type.Properties();
    ASSERT_EQ(properties.Size(), 8);

    ASSERT_TRUE(properties.Has("charMember"));
    const Property& prop1 = properties.Get("charMember");
    {
        const MetaContainer meta = prop1.Meta();
        ASSERT_GE(meta.Size(), 2);

        {
            EXPECT_TRUE(meta.Has("description"));
            const Meta& metaValue = meta.Get("description");

            EXPECT_EQ(metaValue.Key().Type(), Types().GetType<const char*>());
            EXPECT_EQ(metaValue.Value().Type(), Types().GetType<const char*>());

            EXPECT_STRCASEEQ(metaValue.Key().GetAs<const char*>(), "description");
            EXPECT_STRCASEEQ(metaValue.Value().GetAs<const char*>(), "a simple 1 byte type");
        }

        {
            EXPECT_TRUE(meta.Has("description"));
            Meta metaValue = meta.Get("description");
        }

    }

    const Property& prop2 = properties[1];
    const MetaContainer& meta2 = prop2.Meta();
}

TEST(RTTRProperties, TestProtected)
{

}
