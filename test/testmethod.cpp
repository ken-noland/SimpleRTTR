#include <gtest/gtest.h>
#include <simplerttr.h>

using namespace SimpleRTTR;

class SimpleRTTRTestMethod1
{
public:
    int AddTwoPlusTwo() { return 2 + 2; }
    int AddTwoPlusX(int x) { return 2 + x; }
    int AddXPlusY(int x, int y) { return x + y; }
};

class SimpleRTTRTestMethod2
{
public:
    int AddTwoPlusTwo() { return 2 + 2; }
    int AddTwoPlusX(int x) { return 2 + x; }
    int AddXPlusY(int x, int y) { return x + y; }
};



TEST(RTTRMethod, TestIncorrectArgumentsThrowsException)
{
    EXPECT_THROW(registration().type<SimpleRTTRTestMethod1>().method(&SimpleRTTRTestMethod1::AddXPlusY, "AddXPlusY", { "X" }), std::runtime_error);
}

TEST(RTTRMethod, TestNoArgumentsThrowsException)
{
    EXPECT_THROW(registration().type<SimpleRTTRTestMethod1>().method(&SimpleRTTRTestMethod1::AddXPlusY, "AddXPlusY"), std::runtime_error);
}

TEST(RTTRMethod, TestNoArguments)
{
    registration().type<SimpleRTTRTestMethod1>().method(&SimpleRTTRTestMethod1::AddTwoPlusTwo, "AddTwoPlusTwo");

    Type type = types().get_type<SimpleRTTRTestMethod2>().value();
    
}

TEST(RTTRMethod, TestArgumentNames)
{
    registration().type<SimpleRTTRTestMethod2>().method(&SimpleRTTRTestMethod2::AddXPlusY, "AddXPlusY", { "X", "Y" });

    Type type = types().get_type<SimpleRTTRTestMethod2>().value();

    MethodContainer::ConstIterator found = std::find_if(type.methods().begin(), type.methods().end(), 
        [](const Method& method) { return (method.name() == "AddXPlusY") && (method.parameters().size() == 2); });

    EXPECT_NE(found, type.methods().end());

    const Method& method = *found;
    EXPECT_EQ(method.parameters().size(), 2);

    Parameter param1 = method.parameters()[0];
    EXPECT_EQ(param1.name(), "X");
    EXPECT_EQ(param1.type(), types().get_type<int>());

    Parameter param2 = method.parameters()[1];
    EXPECT_EQ(param2.name(), "Y");
    EXPECT_EQ(param2.type(), types().get_type<int>());
}