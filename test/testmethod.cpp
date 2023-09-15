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



TEST(RTTRMethod, TestIncorrectArgumentsThrowsException)
{
    EXPECT_THROW(Registration().Type<SimpleRTTRTestMethod1>().Method(&SimpleRTTRTestMethod1::AddXPlusY, "AddXPlusY", { "X" }), std::runtime_error);
}

TEST(RTTRMethod, TestNoArgumentsThrowsException)
{
    EXPECT_THROW(Registration().Type<SimpleRTTRTestMethod1>().Method(&SimpleRTTRTestMethod1::AddXPlusY, "AddXPlusY"), std::runtime_error);
}

TEST(RTTRMethod, TestArgumentNames)
{
    Registration().Type<SimpleRTTRTestMethod1>().Method(&SimpleRTTRTestMethod1::AddXPlusY, "AddXPlusY", { "X", "Y" });

    Type type = Types().GetType<SimpleRTTRTestMethod1>();

    EXPECT_EQ(type.Methods().size(), 1);

    Method method = type.Methods()[0];
    EXPECT_EQ(method.Parameters().size(), 2);

    Parameter param1 = method.Parameters()[0];
    EXPECT_EQ(param1.Name(), "X");
    EXPECT_EQ(param1.Type(), Types().GetType<int>());

    Parameter param2 = method.Parameters()[1];
    EXPECT_EQ(param2.Name(), "Y");
    EXPECT_EQ(param2.Type(), Types().GetType<int>());
}