#include <gtest/gtest.h>
#include <simplerttr.h>

using namespace SimpleRTTR;

class SimpleRTTRTestCreateInstanceSimple
{
public:
    SimpleRTTRTestCreateInstanceSimple()
    {
        constructorCalled = true;
    }

    ~SimpleRTTRTestCreateInstanceSimple()
    {
        destructorCalled = true;
    }

    char property1;
    char property2;
    char property3;

    static bool constructorCalled;
    static bool destructorCalled;
};

bool SimpleRTTRTestCreateInstanceSimple::constructorCalled = false;
bool SimpleRTTRTestCreateInstanceSimple::destructorCalled = false;

class Point
{
public:
    Point(float x, float y) : X(x), Y(y)
    {
        constructorCalled = true;
    }

    ~Point()
    {
        destructorCalled = true;
    }

    float X;
    float Y;

    static bool constructorCalled;
    static bool destructorCalled;
};

bool Point::constructorCalled = false;
bool Point::destructorCalled = false;


SIMPLERTTR
{
    registration().type<SimpleRTTRTestCreateInstanceSimple>()
        .property(&SimpleRTTRTestCreateInstanceSimple::property1, "property1")
        .property(&SimpleRTTRTestCreateInstanceSimple::property2, "property2")
        .property(&SimpleRTTRTestCreateInstanceSimple::property3, "property3");
    registration().type<Point>()
        .property(&Point::X, "X")
        .property(&Point::Y, "Y");
}

TEST(RTTRTypeInstance, TestTypeCreateInstance)
{
    EXPECT_EQ(SimpleRTTRTestCreateInstanceSimple::constructorCalled, false);
    EXPECT_EQ(SimpleRTTRTestCreateInstanceSimple::destructorCalled, false);

    Type type = types().get_type<SimpleRTTRTestCreateInstanceSimple>();
    SimpleRTTRTestCreateInstanceSimple* instance = type.create_instance<SimpleRTTRTestCreateInstanceSimple>();

    ASSERT_NE(instance, nullptr);
    EXPECT_EQ(SimpleRTTRTestCreateInstanceSimple::constructorCalled, true);

    type.destroy_instance(instance);
    EXPECT_EQ(SimpleRTTRTestCreateInstanceSimple::destructorCalled, true);
}

TEST(RTTRTypeInstance, TestConstructorWithParameters)
{
    EXPECT_EQ(Point::constructorCalled, false);
    EXPECT_EQ(Point::destructorCalled, false);

    Type type = types().get_type<Point>();
    Point* instance = type.create_instance<Point>(1.f, 2.f);

    ASSERT_NE(instance, nullptr);
    EXPECT_EQ(Point::constructorCalled, true);

    type.destroy_instance(instance);
    EXPECT_EQ(Point::destructorCalled, true);
}

TEST(RTTRTypeInstance, TestCreateUnknownType)
{
    //class TestCreateUnknownTypeClass
    //{
    //public:
    //    int x;
    //    int y;
    //};

    //registration().type<TestCreateUnknownTypeClass>()
    //    .property(&TestCreateUnknownTypeClass::x, "x")
    //    .property(&TestCreateUnknownTypeClass::y, "y");

    //const Type& type = types().get_type<TestCreateUnknownTypeClass>();
    //void* instance = type.CreateInstance();

    //ASSERT_NE(instance, nullptr);
    //type.DestroyInstance(instance);
}
