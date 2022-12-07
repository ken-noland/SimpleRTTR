#include <gtest/gtest.h>
#include <SimpleRTTR.h>

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
    Registration().Type<SimpleRTTRTestCreateInstanceSimple>()
        .Property(&SimpleRTTRTestCreateInstanceSimple::property1, "property1")
        .Property(&SimpleRTTRTestCreateInstanceSimple::property2, "property2")
        .Property(&SimpleRTTRTestCreateInstanceSimple::property3, "property3");
    Registration().Type<Point>()
        .Property(&Point::X, "X")
        .Property(&Point::Y, "Y");
}

TEST(RTTRTypeInstance, TestTypeCreateInstance)
{
    EXPECT_EQ(SimpleRTTRTestCreateInstanceSimple::constructorCalled, false);
    EXPECT_EQ(SimpleRTTRTestCreateInstanceSimple::destructorCalled, false);

    const Type& type = Types().GetType<SimpleRTTRTestCreateInstanceSimple>();
    SimpleRTTRTestCreateInstanceSimple* instance = type.CreateInstance<SimpleRTTRTestCreateInstanceSimple>();

    ASSERT_NE(instance, nullptr);
    EXPECT_EQ(SimpleRTTRTestCreateInstanceSimple::constructorCalled, true);

    type.DestroyInstance(instance);
    EXPECT_EQ(SimpleRTTRTestCreateInstanceSimple::destructorCalled, true);
}

TEST(RTTRTypeInstance, TestConstructorWithParameters)
{
    //EXPECT_EQ(Point::constructorCalled, false);
    //EXPECT_EQ(Point::destructorCalled, false);

    //const Type& type = Types().GetType<Point>();
    //Point* instance = type.CreateInstance<Point>(1.f, 2.f);

    //ASSERT_NE(instance, nullptr);
    //EXPECT_EQ(Point::constructorCalled, true);

    //type.DestroyInstance(instance);
    //EXPECT_EQ(Point::destructorCalled, true);
}

TEST(RTTRTypeInstance, TestCreateUnknownType)
{
    //class TestCreateUnknownTypeClass
    //{
    //public:
    //    int x;
    //    int y;
    //};

    //Registration().Type<TestCreateUnknownTypeClass>()
    //    .Property(&TestCreateUnknownTypeClass::x, "x")
    //    .Property(&TestCreateUnknownTypeClass::y, "y");

    //const Type& type = Types().GetType<TestCreateUnknownTypeClass>();
    //void* instance = type.CreateInstance();

    //ASSERT_NE(instance, nullptr);
    //type.DestroyInstance(instance);
}