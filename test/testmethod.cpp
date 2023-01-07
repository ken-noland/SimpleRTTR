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
