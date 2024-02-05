#include <gtest/gtest.h>
#include <simplerttr.h>

using namespace SimpleRTTR;

namespace TestIterator
{
    class SimpleRTTRTestIterator
    {
    public:
        enum class Enum
        {
            Value1,
            Value2,
            Value3
        };

        char property1;
        char property2;
        char property3;
        Enum property4;

        void someFunc1(int p1, int p2) { };
        void someFunc2(int p1, int p2) { };
        void someFunc3(int p1, int p2) { };
    };
}

using namespace SimpleRTTR;

SIMPLERTTR
{
    Registration().Type<TestIterator::SimpleRTTRTestIterator>()
        .Meta("This is a key", "and this is the value")
        .Meta(42, "here is an int as a key")
        .Meta("here is an int as a value", 32)
        .Meta("this should resolve to std::vector<int>", { 1, 2, 3, 4 })
        .Meta("and here's a function pointer", &TestIterator::SimpleRTTRTestIterator::someFunc1)

        .Property(&TestIterator::SimpleRTTRTestIterator::property1, "property1")
            .Meta("This is a key", "and this is the value")
            .Meta(42, "here is an int as a key")
            .Meta("here is an int as a value", 32)
            .Meta("this should resolve to std::vector<int>", { 1, 2, 3, 4 })
        .Property(&TestIterator::SimpleRTTRTestIterator::property2, "property2")
        .Property(&TestIterator::SimpleRTTRTestIterator::property3, "property3")
        .Property(&TestIterator::SimpleRTTRTestIterator::property4, "property4")
        .Method(&TestIterator::SimpleRTTRTestIterator::someFunc1, "someFunc1", {"p1", "p2"})
            .Meta("description", "this function does something")
        .Method(&TestIterator::SimpleRTTRTestIterator::someFunc2, "someFunc2", {"p1", "p2"})
            .Meta("description", "this function does nothing")
        .Method(&TestIterator::SimpleRTTRTestIterator::someFunc3, "someFunc3", {"p1", "p2"})
            .Meta("description", "this function does everything")
        ;

    Registration().Type<TestIterator::SimpleRTTRTestIterator::Enum>()
        .Value(TestIterator::SimpleRTTRTestIterator::Enum::Value1, "Value1")
        .Value(TestIterator::SimpleRTTRTestIterator::Enum::Value2, "Value2")
        .Value(TestIterator::SimpleRTTRTestIterator::Enum::Value3, "Value3");
}

TEST(RTTRIterator, TestAllTypes)
{
    // iterate over all types, and just check that there are more than 2 types
    int count = 0;
    for (Type type : Types())
    {
        count++;
    }

    EXPECT_GT(count, 2);
}

TEST(RTTRIterator, TestTypeMeta)
{
    Type testIteratorType = Types().GetType<TestIterator::SimpleRTTRTestIterator>();
    EXPECT_NE(testIteratorType, Type::InvalidType());

    int count = 0;
    for (const Meta& meta : testIteratorType.Meta())
    {
        count++;
    }

    //we have 5, but because this is a type and it is registered above, SimpleRTTR adds a few meta tags to it to signify where it was declared and
    //  a few more meta data stuff we can find out about the type during compile time.
    EXPECT_GT(count, 5);
}

TEST(RTTRIterator, TestTypeProperties)
{
    Type testIteratorType = Types().GetType<TestIterator::SimpleRTTRTestIterator>();
    EXPECT_NE(testIteratorType, Type::InvalidType());

    int count = 0;
    for(const Property& property : testIteratorType.Properties())
    {
        count++;
    }

    EXPECT_EQ(count, 4);
}

TEST(RTTRIterator, TestPropertiesMeta)
{
    Type testIteratorType = Types().GetType<TestIterator::SimpleRTTRTestIterator>();
    EXPECT_NE(testIteratorType, Type::InvalidType());

    Property prop = testIteratorType.Properties().Get("property1");
    ASSERT_NE(prop, Property::InvalidProperty());

    int count = 0;
    for (const Meta& meta : prop.Meta())
    {
        count++;
    }

    EXPECT_EQ(count, 4);
}

TEST(RTTRIterator, TestTypeMethods)
{
    Type testIteratorType = Types().GetType<TestIterator::SimpleRTTRTestIterator>();
    EXPECT_NE(testIteratorType, Type::InvalidType());

    int count = 0;
    for (const Method& method : testIteratorType.Methods())
    {
        count++;
    }

    EXPECT_EQ(count, 3);
}

TEST(RTTRIterator, TestMethodsMeta)
{
    const Type& testIteratorType = Types().GetType<TestIterator::SimpleRTTRTestIterator>();
    EXPECT_NE(testIteratorType, Type::InvalidType());

    const Method& method = testIteratorType.Methods().Get("someFunc1");
    ASSERT_NE(method, Method::InvalidMethod());

    int count = 0;
    for (const Meta& meta : method.Meta())
    {
        count++;
    }

    EXPECT_GT(count, 0);
}

TEST(RTTRIterator, TestMethodsParameters)
{
    const Type& testIteratorType = Types().GetType<TestIterator::SimpleRTTRTestIterator>();
    EXPECT_NE(testIteratorType, Type::InvalidType());

    const Method& method = testIteratorType.Methods().Get("someFunc1");
    ASSERT_NE(method, Method::InvalidMethod());

    int count = 0;
    for (const Parameter& param : method.Parameters())
    {
        count++;
    }

    EXPECT_EQ(count, 2);
}