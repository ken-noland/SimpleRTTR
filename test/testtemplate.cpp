#include <gtest/gtest.h>
#include <simplerttr.h>

using namespace SimpleRTTR;

template<typename T>
class SimpleRTTRTestTemplate
{
public:
    T property1;
};

template<typename T>
class SimpleRTTRTestTemplate2
{
public:
    SimpleRTTRTestTemplate<T> property1;
};



TEST(RTTRRegistration, TestBasicTemplate) 
{
    registration().type<SimpleRTTRTestTemplate<int>>()
        .property(&SimpleRTTRTestTemplate<int>::property1, "property1");

    Type test = types().get_type<SimpleRTTRTestTemplate<int>>().value();

    EXPECT_EQ(test.name(), "SimpleRTTRTestTemplate");
    EXPECT_EQ(test.properties().size(), 1);
    EXPECT_EQ(test.properties()[0].name(), "property1");
    EXPECT_EQ(test.template_params().size(), 1);
    EXPECT_EQ(test.template_params()[0].type(), types().get_type<int>().value());
}



TEST(RTTRRegistration, TestNestedTemplate)
{
    registration().type<SimpleRTTRTestTemplate2<int>>()
        .property(&SimpleRTTRTestTemplate2<int>::property1, "property1");

    Type test = types().get_type<SimpleRTTRTestTemplate2<int>>().value();

    EXPECT_EQ(test.name(), "SimpleRTTRTestTemplate2");
    EXPECT_EQ(test.properties().size(), 1);
    EXPECT_EQ(test.properties()[0].name(), "property1");
    EXPECT_EQ(test.template_params().size(), 1);
    EXPECT_EQ(test.template_params()[0].type(), types().get_type<int>().value());

    Type ref = test.properties()[0].type();
    EXPECT_EQ(ref.name(), "SimpleRTTRTestTemplate");
    EXPECT_EQ(ref.template_params().size(), 1);
}

template<int A, int B>
class SimpleRTTRTestTemplate3
{
public:
    int property1[A];
    int property2[B];
};

TEST(RTTRRegistration, TestValueTemplateParameter)
{
    registration().type<SimpleRTTRTestTemplate3<2, 4>>()
        .property(&SimpleRTTRTestTemplate3<2, 4>::property1, "property1")
        .property(&SimpleRTTRTestTemplate3<2, 4>::property2, "property2");

    Type test = types().get_type<SimpleRTTRTestTemplate3<2, 4>>().value();

    EXPECT_EQ(test.name(), "SimpleRTTRTestTemplate3");
    EXPECT_EQ(test.properties().size(), 2);
    EXPECT_EQ(test.properties()[0].name(), "property1");
    EXPECT_EQ(test.properties()[1].name(), "property2");
    EXPECT_EQ(test.template_params().size(), 2);

}


template<int A, typename T>
class SimpleRTTRTestTemplate4
{
public:
    T property1[A];
};

TEST(RTTRRegistration, TestMixedValueTypeTemplateParameter)
{
    registration().type<SimpleRTTRTestTemplate4<2, int>>()
        .property(&SimpleRTTRTestTemplate4<2, int>::property1, "property1");

    Type test = types().get_type<SimpleRTTRTestTemplate4<2, int>>().value();

    EXPECT_EQ(test.name(), "SimpleRTTRTestTemplate4");
    EXPECT_EQ(test.properties().size(), 1);
    EXPECT_EQ(test.properties()[0].name(), "property1");
    EXPECT_EQ(test.template_params().size(), 2);
}

template<int A, typename T, int B>
class SimpleRTTRTestTemplate5
{
public:
    SimpleRTTRTestTemplate5() : property2(B) {}

    T property1[A];
    int property2;
};

TEST(RTTRRegistration, TestMixedValueTypeValueTemplateParameter)
{
    registration().type<SimpleRTTRTestTemplate5<2, int, 42>>()
        .property(&SimpleRTTRTestTemplate5<2, int, 42>::property1, "property1")
        .property(&SimpleRTTRTestTemplate5<2, int, 42>::property2, "property2");

    Type test = types().get_type<SimpleRTTRTestTemplate5<2, int, 42>>().value();

    EXPECT_EQ(test.name(), "SimpleRTTRTestTemplate5");
    EXPECT_EQ(test.properties().size(), 2);
    EXPECT_EQ(test.properties()[0].name(), "property1");
    EXPECT_EQ(test.properties()[1].name(), "property2");
    EXPECT_EQ(test.template_params().size(), 3);
}
