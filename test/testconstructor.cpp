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
    registration().type<SimpleRTTRTestConstructor1>()
        .constructor()
        .property(&SimpleRTTRTestConstructor1::_x, "x")
        .property(&SimpleRTTRTestConstructor1::_y, "y");

    registration().type<SimpleRTTRTestConstructor2>()
        .constructor<int, int>({ "x", "y" })
        .property(&SimpleRTTRTestConstructor2::_x, "x")
        .property(&SimpleRTTRTestConstructor2::_y, "y");
}

TEST(RTTRConstructor, TestNoArguments)
{
    Type type = types().get_type<SimpleRTTRTestConstructor1>();

    const ConstructorContainer& constructors = type.constructors();
    ASSERT_EQ(constructors.size(), 1);

    Method constructor = constructors[0];
    ASSERT_EQ(constructor.parameters().size(), 0);
}


TEST(RTTRConstructor, Test2Arguments)
{
    Type type = types().get_type<SimpleRTTRTestConstructor2>();

    const ConstructorContainer& constructors = type.constructors();
    ASSERT_EQ(constructors.size(), 1);

    Method constructor = constructors[0];
    ASSERT_EQ(constructor.parameters().size(), 2);
}