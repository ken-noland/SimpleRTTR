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
    EXPECT_THROW(Registration().Type<SimpleRTTRTestMethod1>().Method(&SimpleRTTRTestMethod1::AddXPlusY, "AddXPlusY", { "X" }), std::runtime_error);
}

TEST(RTTRMethod, TestNoArgumentsThrowsException)
{
    EXPECT_THROW(Registration().Type<SimpleRTTRTestMethod1>().Method(&SimpleRTTRTestMethod1::AddXPlusY, "AddXPlusY"), std::runtime_error);
}

TEST(RTTRMethod, TestNoArguments)
{
    Registration().Type<SimpleRTTRTestMethod1>().Method(&SimpleRTTRTestMethod1::AddTwoPlusTwo, "AddTwoPlusTwo");

    Type type = Types().GetType<SimpleRTTRTestMethod2>();
    
}

TEST(RTTRMethod, TestArgumentNames)
{
    Registration().Type<SimpleRTTRTestMethod2>().Method(&SimpleRTTRTestMethod2::AddXPlusY, "AddXPlusY", { "X", "Y" });

    Type type = Types().GetType<SimpleRTTRTestMethod2>();

    MethodContainer::ConstIterator found = std::find_if(type.Methods().begin(), type.Methods().end(), 
        [](const Method& method) { return (method.Name() == "AddXPlusY") && (method.Parameters().size() == 2); });

    EXPECT_NE(found, type.Methods().end());

    const Method& method = *found;
    EXPECT_EQ(method.Parameters().size(), 2);

    Parameter param1 = method.Parameters()[0];
    EXPECT_EQ(param1.Name(), "X");
    EXPECT_EQ(param1.Type(), Types().GetType<int>());

    Parameter param2 = method.Parameters()[1];
    EXPECT_EQ(param2.Name(), "Y");
    EXPECT_EQ(param2.Type(), Types().GetType<int>());
}