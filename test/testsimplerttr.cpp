#include <gtest/gtest.h>
#include <simplerttr.h>

class SimpleRTTRTestClass1
{
public:
    int AddTwoPlusTwo() { return 2 + 2; }
    int AddTwoPlusX(int x) { return 2 + x; }
    int AddXPlusY(int x, int y) { return x + y; }

    char            charMember;
    short           shortMember;
    int             intMember;
    long            longMember;
    unsigned char   ucharMember;
    unsigned short  ushortMember;
    unsigned int    uintMember;
    unsigned long   ulongMember;
    void*           voidpointerMember;
    float           floatMember;
    double          doubleMember;
};

class SimpleRTTRTestClass2
{
public:
    SimpleRTTRTestClass1 member;
};

using namespace SimpleRTTR;

SIMPLERTTR
{
    registration().type<SimpleRTTRTestClass1>()
        .property(&SimpleRTTRTestClass1::charMember, "charMember")
        .property(&SimpleRTTRTestClass1::shortMember, "shortMember")
        .property(&SimpleRTTRTestClass1::intMember, "intMember")
        .property(&SimpleRTTRTestClass1::longMember, "longMember")
        .property(&SimpleRTTRTestClass1::ucharMember, "ucharMember")
        .property(&SimpleRTTRTestClass1::ushortMember, "ushortMember")
        .property(&SimpleRTTRTestClass1::uintMember, "uintMember")
        .property(&SimpleRTTRTestClass1::ulongMember, "ulongMember")
        .property(&SimpleRTTRTestClass1::voidpointerMember, "voidpointerMember")
        .property(&SimpleRTTRTestClass1::floatMember, "floatMember")
        .property(&SimpleRTTRTestClass1::doubleMember, "doubleMember")

        .method(&SimpleRTTRTestClass1::AddTwoPlusTwo, "AddTwoPlusTwo")
        .method(&SimpleRTTRTestClass1::AddTwoPlusX, "AddTwoPlusX", {"X"})
        .method(&SimpleRTTRTestClass1::AddXPlusY, "AddXPlusY", {"X", "Y"});

    registration().type<SimpleRTTRTestClass2>()
        .property(&SimpleRTTRTestClass2::member, "member");
}

TEST(RTTRRegistration, TestBasicRegistration) 
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();
    EXPECT_EQ(test.name(), "SimpleRTTRTestClass1");
}
TEST(RTTRRegistration, TestBasicProperties)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();
    EXPECT_EQ(test.properties().size(), 11);
}

TEST(RTTRRegistration, TestCharMember)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    Property charMember = test.properties()[0];
    EXPECT_EQ(charMember.name(), "charMember");
    EXPECT_EQ(charMember.offset(), offsetof(SimpleRTTRTestClass1, charMember));
    EXPECT_EQ(charMember.type().name(), "char");
    EXPECT_EQ(charMember.type().size(), sizeof(char));
}
TEST(RTTRRegistration, TestShortMember)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    Property shortMember = test.properties()[1];
    EXPECT_EQ(shortMember.name(), "shortMember");
    EXPECT_EQ(shortMember.offset(), offsetof(SimpleRTTRTestClass1, shortMember));
    EXPECT_EQ(shortMember.type().name(), "short");
    EXPECT_EQ(shortMember.type().size(), sizeof(short));
}

TEST(RTTRRegistration, TestIntMember)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    Property intMember = test.properties()[2];
    EXPECT_EQ(intMember.name(), "intMember");
    EXPECT_EQ(intMember.offset(), offsetof(SimpleRTTRTestClass1, intMember));
    EXPECT_EQ(intMember.type().name(), "int");
    EXPECT_EQ(intMember.type().size(), sizeof(int));
}

TEST(RTTRRegistration, TestLongMember)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    Property longMember = test.properties()[3];
    EXPECT_EQ(longMember.name(), "longMember");
    EXPECT_EQ(longMember.offset(), offsetof(SimpleRTTRTestClass1, longMember));
    EXPECT_EQ(longMember.type().name(), "long");
    EXPECT_EQ(longMember.type().size(), sizeof(long));
}

TEST(RTTRRegistration, TestUCharMember)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    Property ucharMember = test.properties()[4];
    EXPECT_EQ(ucharMember.name(), "ucharMember");
    EXPECT_EQ(ucharMember.offset(), offsetof(SimpleRTTRTestClass1, ucharMember));
    EXPECT_EQ(ucharMember.type().name(), "unsigned char");
    EXPECT_EQ(ucharMember.type().size(), sizeof(unsigned char));
}

TEST(RTTRRegistration, TestUShortMember)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    Property ushortMember = test.properties()[5];
    EXPECT_EQ(ushortMember.name(), "ushortMember");
    EXPECT_EQ(ushortMember.offset(), offsetof(SimpleRTTRTestClass1, ushortMember));
    EXPECT_EQ(ushortMember.type().name(), "unsigned short");
    EXPECT_EQ(ushortMember.type().size(), sizeof(unsigned short));
}

TEST(RTTRRegistration, TestUIntMember)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    Property uintMember = test.properties()[6];
    EXPECT_EQ(uintMember.name(), "uintMember");
    EXPECT_EQ(uintMember.offset(), offsetof(SimpleRTTRTestClass1, uintMember));
    EXPECT_EQ(uintMember.type().name(), "unsigned int");
    EXPECT_EQ(uintMember.type().size(), sizeof(unsigned int));
}

TEST(RTTRRegistration, TestULongMember)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    Property ulongMember = test.properties()[7];
    EXPECT_EQ(ulongMember.name(), "ulongMember");
    EXPECT_EQ(ulongMember.offset(), offsetof(SimpleRTTRTestClass1, ulongMember));
    EXPECT_EQ(ulongMember.type().name(), "unsigned long");
    EXPECT_EQ(ulongMember.type().size(), sizeof(unsigned long));
}

TEST(RTTRRegistration, TestVoidPointerMember)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    Property voidpointerMember = test.properties()[8];
    EXPECT_EQ(voidpointerMember.name(), "voidpointerMember");
    EXPECT_EQ(voidpointerMember.offset(), offsetof(SimpleRTTRTestClass1, voidpointerMember));
    EXPECT_EQ(voidpointerMember.type().name(), "void*");
    EXPECT_EQ(voidpointerMember.type().size(), sizeof(void*));
}

TEST(RTTRRegistration, TestFloatMember)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    Property floatMember = test.properties()[9];
    EXPECT_EQ(floatMember.name(), "floatMember");
    EXPECT_EQ(floatMember.offset(), offsetof(SimpleRTTRTestClass1, floatMember));
    EXPECT_EQ(floatMember.type().name(), "float");
    EXPECT_EQ(floatMember.type().size(), sizeof(float));
}

TEST(RTTRRegistration, TestDoubleMember)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    Property doubleMember = test.properties()[10];
    EXPECT_EQ(doubleMember.name(), "doubleMember");
    EXPECT_EQ(doubleMember.offset(), offsetof(SimpleRTTRTestClass1, doubleMember));
    EXPECT_EQ(doubleMember.type().name(), "double");
    EXPECT_EQ(doubleMember.type().size(), sizeof(double));
}

TEST(RTTRRegistration, TestBasicMethods)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();
    EXPECT_EQ(test.methods().size(), 3);
}

TEST(RTTRRegistration, TestAddTwoPlusTwoMethod)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    const Method& method = test.methods()[0];
    EXPECT_EQ(method.name(), "AddTwoPlusTwo");
    EXPECT_EQ(method.return_type(), types().get_type<int>());
    EXPECT_EQ(method.parameters().size(), 0);
}

TEST(RTTRRegistration, TestAddTwoPlusXMethod)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    const Method& method = test.methods()[1];
    EXPECT_EQ(method.name(), "AddTwoPlusX");
    EXPECT_EQ(method.return_type(), types().get_type<int>());
    ASSERT_EQ(method.parameters().size(), 1);

    const Parameter& param1 = method.parameters()[0];
    EXPECT_EQ(param1.type(), types().get_type<int>());
}

TEST(RTTRRegistration, TestAddXPlusYMethod)
{
    Type test = types().get_type<SimpleRTTRTestClass1>().value();

    const Method& method = test.methods()[2];
    EXPECT_EQ(method.name(), "AddXPlusY");
    EXPECT_EQ(method.return_type(), types().get_type<int>());
    ASSERT_EQ(method.parameters().size(), 2);

    const Parameter& param1 = method.parameters()[0];
    EXPECT_EQ(param1.type(), types().get_type<int>());

    const Parameter& param2 = method.parameters()[1];
    EXPECT_EQ(param1.type(), types().get_type<int>());
}
