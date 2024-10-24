#include <gtest/gtest.h>
#include <simplerttr.h>

using namespace SimpleRTTR;

namespace TestIterator
{
    template <typename A, typename B=int, typename C=int>
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

        void someFunc1(int p1, A p2) { };
        void someFunc2(int p1, B p2) { };
        void someFunc3(int p1, C p2) { };
    };
}

using namespace SimpleRTTR;

SIMPLERTTR
{
    registration().type<TestIterator::SimpleRTTRTestIterator<int>>()
        .meta("This is a key", "and this is the value")
        .meta(42, "here is an int as a key")
        .meta("here is an int as a value", 32)
        .meta("this should resolve to std::vector<int>", { 1, 2, 3, 4 })
        .meta("and here's a function pointer", &TestIterator::SimpleRTTRTestIterator<int>::someFunc1)

        .property(&TestIterator::SimpleRTTRTestIterator<int>::property1, "property1")
            .meta("This is a key", "and this is the value")
            .meta(42, "here is an int as a key")
            .meta("here is an int as a value", 32)
            .meta("this should resolve to std::vector<int>", { 1, 2, 3, 4 })
        .property(&TestIterator::SimpleRTTRTestIterator<int>::property2, "property2")
        .property(&TestIterator::SimpleRTTRTestIterator<int>::property3, "property3")
        .property(&TestIterator::SimpleRTTRTestIterator<int>::property4, "property4")
        .method(&TestIterator::SimpleRTTRTestIterator<int>::someFunc1, "someFunc1", {"p1", "p2"})
            .meta("description", "this function does something")
        .method(&TestIterator::SimpleRTTRTestIterator<int>::someFunc2, "someFunc2", {"p1", "p2"})
            .meta("description", "this function does nothing")
        .method(&TestIterator::SimpleRTTRTestIterator<int>::someFunc3, "someFunc3", {"p1", "p2"})
            .meta("description", "this function does everything")
        ;

    registration().type<TestIterator::SimpleRTTRTestIterator<int>::Enum>()
        .value(TestIterator::SimpleRTTRTestIterator<int>::Enum::Value1, "Value1")
        .value(TestIterator::SimpleRTTRTestIterator<int>::Enum::Value2, "Value2")
        .value(TestIterator::SimpleRTTRTestIterator<int>::Enum::Value3, "Value3");
}

TEST(RTTRIterator, TestAllTypes)
{
    // iterate over all types, and just check that there are more than 2 types
    int count = 0;
    for (Type type : types())
    {
        count++;
    }

    EXPECT_GT(count, 2);
}

TEST(RTTRIterator, TestTypeMeta)
{
    std::optional<Type> testIteratorType = types().get_type<TestIterator::SimpleRTTRTestIterator<int>>();
    EXPECT_TRUE(testIteratorType);

    int count = 0;
    for (const Meta& meta : testIteratorType.value().meta())
    {
        count++;
    }

    //we have 5, but because this is a type and it is registered above, SimpleRTTR adds a few meta tags to it to signify where it was declared and
    //  a few more meta data stuff we can find out about the type during compile time.
    EXPECT_GT(count, 5);
}

TEST(RTTRIterator, TestTypeProperties)
{
    std::optional<Type> testIteratorType = types().get_type<TestIterator::SimpleRTTRTestIterator<int>>();
    EXPECT_TRUE(testIteratorType);

    int count = 0;
    for(const Property& property : testIteratorType.value().properties())
    {
        count++;
    }

    EXPECT_EQ(count, 4);
}

TEST(RTTRIterator, TestPropertiesMeta)
{
    std::optional<Type> testIteratorType = types().get_type<TestIterator::SimpleRTTRTestIterator<int>>();
    EXPECT_TRUE(testIteratorType);

    std::optional<Property> prop = testIteratorType.value().properties().get("property1");
    EXPECT_TRUE(prop);

    int count = 0;
    for (const Meta& meta : prop.value().meta())
    {
        count++;
    }

    EXPECT_EQ(count, 4);
}

TEST(RTTRIterator, TestTypeMethods)
{
    std::optional<Type> testIteratorType = types().get_type<TestIterator::SimpleRTTRTestIterator<int>>();
    EXPECT_TRUE(testIteratorType);

    int count = 0;
    for (const Method& method : testIteratorType.value().methods())
    {
        count++;
    }

    EXPECT_EQ(count, 3);
}

TEST(RTTRIterator, TestMethodsMeta)
{
    std::optional<Type> testIteratorType = types().get_type<TestIterator::SimpleRTTRTestIterator<int>>();
    EXPECT_TRUE(testIteratorType);

    std::optional<std::reference_wrapper<const Method>> method = testIteratorType.value().methods().get("someFunc1");
    EXPECT_TRUE(method);

    int count = 0;
    for (const Meta& meta : method.value().get().meta())
    {
        count++;
    }

    EXPECT_GT(count, 0);
}

TEST(RTTRIterator, TestMethodsParameters)
{
    std::optional<Type> testIteratorType = types().get_type<TestIterator::SimpleRTTRTestIterator<int>>();
    EXPECT_TRUE(testIteratorType);

    std::optional<std::reference_wrapper<const Method>> method = testIteratorType.value().methods().get("someFunc1");
    EXPECT_TRUE(method);

    int count = 0;
    for (const Parameter& param : method.value().get().parameters())
    {
        count++;
    }

    EXPECT_EQ(count, 2);
}

TEST(RTTRIterator, TestNamespaces)
{
    std::optional<Type> testIteratorType = types().get_type<TestIterator::SimpleRTTRTestIterator<int>>();
    EXPECT_TRUE(testIteratorType);

    int count = 0;
    for (const std::string& namesp : testIteratorType.value().namespaces())
    {
        count++;
        EXPECT_EQ(namesp, "TestIterator");
    }
    EXPECT_EQ(count, 1);
}

TEST(RTTRIterator, TestTemplateParameters)
{
    std::optional<Type> testIteratorType = types().get_type<TestIterator::SimpleRTTRTestIterator<int>>();
    EXPECT_TRUE(testIteratorType);

    int count = 0;
    for(const TypeReference& typeRef : testIteratorType.value().template_params())
    {
        count++;
    }
    EXPECT_EQ(count, 3);
}