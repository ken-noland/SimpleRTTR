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

SIMPLERTTR
{
    Registration().Type<SimpleRTTRTestCreateInstanceSimple>()
        .Property(&SimpleRTTRTestCreateInstanceSimple::property1, "property1")
        .Property(&SimpleRTTRTestCreateInstanceSimple::property2, "property2")
        .Property(&SimpleRTTRTestCreateInstanceSimple::property3, "property3");
}

TEST(RTTRMeta, TestTypeCreateInstance)
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

