#include <gtest/gtest.h>
#include <simplerttr.h>

using namespace SimpleRTTR;

class SimpleRTTRTestConstructor1
{
    SIMPLE_RTTR_ALLOW_ACCESS
public:
    SimpleRTTRTestConstructor1() {}
    ~SimpleRTTRTestConstructor1() {}

    void SetX(int x) { _x = x; }
    int GetX() { return _x; }

    void SetY(int y) { _y = y; }
    int GetY() { return _y; }

protected:
    int _x;
    int _y;
};

class SimpleRTTRTestConstructor2
{
    SIMPLE_RTTR_ALLOW_ACCESS
public:
    SimpleRTTRTestConstructor2(int x, int y) : _x(x), _y(y) {}

    void SetX(int x) { _x = x; }
    int GetX() { return _x; }

    void SetY(int y) { _y = y; }
    int GetY() { return _y; }

protected:
    int _x;
    int _y;
};

SIMPLERTTR
{
    Registration().Type<SimpleRTTRTestConstructor1>()
        .Constructor()
        .Property(&SimpleRTTRTestConstructor1::_x, "x")
        .Property(&SimpleRTTRTestConstructor1::_y, "y");

    Registration().Type<SimpleRTTRTestConstructor2>()
        .Constructor<int, int>({ "x", "y" })
        .Property(&SimpleRTTRTestConstructor2::_x, "x")
        .Property(&SimpleRTTRTestConstructor2::_y, "y");
}

TEST(RTTRConstructor, TestNoArguments)
{
    Type type = Types().GetType<SimpleRTTRTestConstructor1>();
}


TEST(RTTRConstructor, Test2Arguments)
{
    Type type = Types().GetType<SimpleRTTRTestConstructor2>();
}