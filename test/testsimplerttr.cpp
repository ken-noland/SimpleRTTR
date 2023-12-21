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
    Registration().Type<SimpleRTTRTestClass1>()
        .Property(&SimpleRTTRTestClass1::charMember, "charMember")
        .Property(&SimpleRTTRTestClass1::shortMember, "shortMember")
        .Property(&SimpleRTTRTestClass1::intMember, "intMember")
        .Property(&SimpleRTTRTestClass1::longMember, "longMember")
        .Property(&SimpleRTTRTestClass1::ucharMember, "ucharMember")
        .Property(&SimpleRTTRTestClass1::ushortMember, "ushortMember")
        .Property(&SimpleRTTRTestClass1::uintMember, "uintMember")
        .Property(&SimpleRTTRTestClass1::ulongMember, "ulongMember")
        .Property(&SimpleRTTRTestClass1::voidpointerMember, "voidpointerMember")
        .Property(&SimpleRTTRTestClass1::floatMember, "floatMember")
        .Property(&SimpleRTTRTestClass1::doubleMember, "doubleMember")

        .Method(&SimpleRTTRTestClass1::AddTwoPlusTwo, "AddTwoPlusTwo")
        .Method(&SimpleRTTRTestClass1::AddTwoPlusX, "AddTwoPlusX", {"X"})
        .Method(&SimpleRTTRTestClass1::AddXPlusY, "AddXPlusY", {"X", "Y"});

    Registration().Type<SimpleRTTRTestClass2>()
        .Property(&SimpleRTTRTestClass2::member, "member");
}

TEST(RTTRRegistration, TestBasicRegistration) 
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();
    EXPECT_EQ(test.Name(), "SimpleRTTRTestClass1");
}
TEST(RTTRRegistration, TestBasicProperties)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();
    EXPECT_EQ(test.Properties().Size(), 11);
}

TEST(RTTRRegistration, TestCharMember)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    Property charMember = test.Properties()[0];
    EXPECT_EQ(charMember.Name(), "charMember");
    EXPECT_EQ(charMember.Offset(), offsetof(SimpleRTTRTestClass1, charMember));
    EXPECT_EQ(charMember.Type().Name(), "char");
    EXPECT_EQ(charMember.Type().Size(), sizeof(char));
}
TEST(RTTRRegistration, TestShortMember)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    Property shortMember = test.Properties()[1];
    EXPECT_EQ(shortMember.Name(), "shortMember");
    EXPECT_EQ(shortMember.Offset(), offsetof(SimpleRTTRTestClass1, shortMember));
    EXPECT_EQ(shortMember.Type().Name(), "short");
    EXPECT_EQ(shortMember.Type().Size(), sizeof(short));
}

TEST(RTTRRegistration, TestIntMember)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    Property intMember = test.Properties()[2];
    EXPECT_EQ(intMember.Name(), "intMember");
    EXPECT_EQ(intMember.Offset(), offsetof(SimpleRTTRTestClass1, intMember));
    EXPECT_EQ(intMember.Type().Name(), "int");
    EXPECT_EQ(intMember.Type().Size(), sizeof(int));
}

TEST(RTTRRegistration, TestLongMember)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    Property longMember = test.Properties()[3];
    EXPECT_EQ(longMember.Name(), "longMember");
    EXPECT_EQ(longMember.Offset(), offsetof(SimpleRTTRTestClass1, longMember));
    EXPECT_EQ(longMember.Type().Name(), "long");
    EXPECT_EQ(longMember.Type().Size(), sizeof(long));
}

TEST(RTTRRegistration, TestUCharMember)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    Property ucharMember = test.Properties()[4];
    EXPECT_EQ(ucharMember.Name(), "ucharMember");
    EXPECT_EQ(ucharMember.Offset(), offsetof(SimpleRTTRTestClass1, ucharMember));
    EXPECT_EQ(ucharMember.Type().Name(), "unsigned char");
    EXPECT_EQ(ucharMember.Type().Size(), sizeof(unsigned char));
}

TEST(RTTRRegistration, TestUShortMember)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    Property ushortMember = test.Properties()[5];
    EXPECT_EQ(ushortMember.Name(), "ushortMember");
    EXPECT_EQ(ushortMember.Offset(), offsetof(SimpleRTTRTestClass1, ushortMember));
    EXPECT_EQ(ushortMember.Type().Name(), "unsigned short");
    EXPECT_EQ(ushortMember.Type().Size(), sizeof(unsigned short));
}

TEST(RTTRRegistration, TestUIntMember)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    Property uintMember = test.Properties()[6];
    EXPECT_EQ(uintMember.Name(), "uintMember");
    EXPECT_EQ(uintMember.Offset(), offsetof(SimpleRTTRTestClass1, uintMember));
    EXPECT_EQ(uintMember.Type().Name(), "unsigned int");
    EXPECT_EQ(uintMember.Type().Size(), sizeof(unsigned int));
}

TEST(RTTRRegistration, TestULongMember)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    Property ulongMember = test.Properties()[7];
    EXPECT_EQ(ulongMember.Name(), "ulongMember");
    EXPECT_EQ(ulongMember.Offset(), offsetof(SimpleRTTRTestClass1, ulongMember));
    EXPECT_EQ(ulongMember.Type().Name(), "unsigned long");
    EXPECT_EQ(ulongMember.Type().Size(), sizeof(unsigned long));
}

TEST(RTTRRegistration, TestVoidPointerMember)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    Property voidpointerMember = test.Properties()[8];
    EXPECT_EQ(voidpointerMember.Name(), "voidpointerMember");
    EXPECT_EQ(voidpointerMember.Offset(), offsetof(SimpleRTTRTestClass1, voidpointerMember));
    EXPECT_EQ(voidpointerMember.Type().Name(), "void *");
    EXPECT_EQ(voidpointerMember.Type().Size(), sizeof(void*));
}

TEST(RTTRRegistration, TestFloatMember)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    Property floatMember = test.Properties()[9];
    EXPECT_EQ(floatMember.Name(), "floatMember");
    EXPECT_EQ(floatMember.Offset(), offsetof(SimpleRTTRTestClass1, floatMember));
    EXPECT_EQ(floatMember.Type().Name(), "float");
    EXPECT_EQ(floatMember.Type().Size(), sizeof(float));
}

TEST(RTTRRegistration, TestDoubleMember)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    Property doubleMember = test.Properties()[10];
    EXPECT_EQ(doubleMember.Name(), "doubleMember");
    EXPECT_EQ(doubleMember.Offset(), offsetof(SimpleRTTRTestClass1, doubleMember));
    EXPECT_EQ(doubleMember.Type().Name(), "double");
    EXPECT_EQ(doubleMember.Type().Size(), sizeof(double));
}

TEST(RTTRRegistration, TestBasicMethods)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();
    EXPECT_EQ(test.Methods().size(), 3);
}

TEST(RTTRRegistration, TestAddTwoPlusTwoMethod)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    const Method& method = test.Methods()[0];
    EXPECT_EQ(method.Name(), "AddTwoPlusTwo");
    EXPECT_EQ(method.ReturnType(), Types().GetType<int>());
    EXPECT_EQ(method.Parameters().size(), 0);
}

TEST(RTTRRegistration, TestAddTwoPlusXMethod)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    const Method& method = test.Methods()[1];
    EXPECT_EQ(method.Name(), "AddTwoPlusX");
    EXPECT_EQ(method.ReturnType(), Types().GetType<int>());
    ASSERT_EQ(method.Parameters().size(), 1);

    const Parameter& param1 = method.Parameters()[0];
    EXPECT_EQ(param1.Type(), Types().GetType<int>());
}

TEST(RTTRRegistration, TestAddXPlusYMethod)
{
    Type test = Types().GetType<SimpleRTTRTestClass1>();

    const Method& method = test.Methods()[2];
    EXPECT_EQ(method.Name(), "AddXPlusY");
    EXPECT_EQ(method.ReturnType(), Types().GetType<int>());
    ASSERT_EQ(method.Parameters().size(), 2);

    const Parameter& param1 = method.Parameters()[0];
    EXPECT_EQ(param1.Type(), Types().GetType<int>());

    const Parameter& param2 = method.Parameters()[1];
    EXPECT_EQ(param1.Type(), Types().GetType<int>());
}
