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
    registration().type<SimpleRTTRTestProperties1>()
        .property(&SimpleRTTRTestProperties1::charMember, "charMember")
            .meta("description", "a simple 1 byte type")
            .meta("numbers", {1,2,3,4,5})
        .property(&SimpleRTTRTestProperties1::shortMember, "shortMember")
            .meta("description", "a simple 2 byte type")
            .meta("callback", someFunc)
        .property(&SimpleRTTRTestProperties1::intMember, "intMember")
            .meta("description", "a simple 4 byte type")
        .property(&SimpleRTTRTestProperties1::voidpointerMember, "voidpointerMember")
        .property(&SimpleRTTRTestProperties1::protectedMember, "protectedMember")
        .property(&SimpleRTTRTestProperties1::privateMember, "privateMember")
        .property(&SimpleRTTRTestProperties1::complexProperties, "complexProperties")
        .property(&SimpleRTTRTestProperties1::complexPropertiesPtr, "complexPropertiesPtr");
    ;
}

TEST(RTTRProperties, TestPropertiesList)
{
    Type type = types().get_type<SimpleRTTRTestProperties1>().value();

    const PropertyContainer& properties = type.properties();
    ASSERT_EQ(properties.size(), 8);

    const Property& prop1 = properties[0];
    EXPECT_EQ(prop1.name(), "charMember");
    EXPECT_EQ(prop1.type(), types().get_type<char>());
    EXPECT_EQ(prop1.offset(), 0);

    const Property& prop2 = properties[1];
    EXPECT_EQ(prop2.name(), "shortMember");
    EXPECT_EQ(prop2.type(), types().get_type<short>());
    EXPECT_EQ(prop2.offset(), 2);

    const Property& prop3 = properties[2];
    EXPECT_EQ(prop3.name(), "intMember");
    EXPECT_EQ(prop3.type(), types().get_type<int>());
    EXPECT_EQ(prop3.offset(), 4);

    const Property& prop4 = properties[3];
    EXPECT_EQ(prop4.name(), "voidpointerMember");
    EXPECT_EQ(prop4.type(), types().get_type<void*>());
    EXPECT_EQ(prop4.offset(), 8);

}


TEST(RTTRProperties, TestPropertiesGet)
{
    Type type = types().get_type<SimpleRTTRTestProperties1>().value();

    SimpleRTTRTestProperties1 instance;
    instance.charMember = 5;
    instance.shortMember = 10;
    instance.intMember = 15;
    instance.voidpointerMember = (void*)0x1234;

    const PropertyContainer& properties = type.properties();
    ASSERT_EQ(properties.size(), 8);

    const Property& prop1 = properties[0];
    EXPECT_EQ(prop1.get(&instance).get_as<char>(), 5);

    const Property& prop2 = properties[1];
    EXPECT_EQ(prop2.get(&instance).get_as<short>(), 10);

    const Property& prop3 = properties[2];
    EXPECT_EQ(prop3.get(&instance).get_as<int>(), 15);

    const Property& prop4 = properties[3];
    EXPECT_EQ(prop4.get(&instance).get_as<void*>(), (void*)0x1234);
}

TEST(RTTRProperties, TestPropertiesSet)
{
    Type type = types().get_type<SimpleRTTRTestProperties1>().value();

    SimpleRTTRTestProperties1 instance;
    instance.charMember = 0;
    instance.shortMember = 0;
    instance.intMember = 0;
    instance.voidpointerMember = nullptr;

    const PropertyContainer& properties = type.properties();
    ASSERT_EQ(properties.size(), 8);

    const Property& prop1 = properties[0];
    prop1.set(&instance, (char)5);
    EXPECT_EQ(instance.charMember, 5);

    const Property& prop2 = properties[1];
    prop2.set(&instance, (short)10);
    EXPECT_EQ(instance.shortMember, 10);

    const Property& prop3 = properties[2];
    prop3.set(&instance, (int)15);
    EXPECT_EQ(instance.intMember, 15);

    const Property& prop4 = properties[3];
    prop4.set(&instance, (void*)0x1234);
    EXPECT_EQ(instance.voidpointerMember, (void*)0x1234);
}



TEST(RTTRProperties, TestPropertiesMeta)
{
    Type type = types().get_type<SimpleRTTRTestProperties1>().value();

    const PropertyContainer& properties = type.properties();
    ASSERT_EQ(properties.size(), 8);

    ASSERT_TRUE(properties.has("charMember"));
    const Property& prop1 = properties.get("charMember");
    {
        const MetaContainer meta = prop1.meta();
        ASSERT_GE(meta.size(), 2);

        {
            EXPECT_TRUE(meta.has("description"));
            const Meta& metaValue = meta.get("description");

            EXPECT_EQ(metaValue.key().type(), types().get_type<const char*>());
            EXPECT_EQ(metaValue.value().type(), types().get_type<const char*>());

            EXPECT_STRCASEEQ(metaValue.key().get_as<const char*>(), "description");
            EXPECT_STRCASEEQ(metaValue.value().get_as<const char*>(), "a simple 1 byte type");
        }

        {
            EXPECT_TRUE(meta.has("description"));
            Meta metaValue = meta.get("description");
        }

    }

    const Property& prop2 = properties[1];
    const MetaContainer& meta2 = prop2.meta();
}

TEST(RTTRProperties, TestProtected)
{

}
