#include <gtest/gtest.h>
#include <simplerttr.h>

using namespace SimpleRTTR;


namespace TestHash
{
    class SimpleTestHash1
    {
    public:
        char property1;
        char property2;
        char property3;
    };

    class SimpleTestHash2
    {
    public:
        char property1;
        char property2;
        char property3;
    };
}

SIMPLERTTR
{
    registration().type<TestHash::SimpleTestHash1>()
        .property(&TestHash::SimpleTestHash1::property1, "property1")
        .property(&TestHash::SimpleTestHash1::property2, "property2")
        .property(&TestHash::SimpleTestHash1::property3, "property3")
    ;


    registration().type<TestHash::SimpleTestHash2>()
        .property(&TestHash::SimpleTestHash2::property1, "property1")
        .property(&TestHash::SimpleTestHash2::property2, "property2")
        .property(&TestHash::SimpleTestHash2::property3, "property3")
        ;
}

TEST(RTTRHash, TestHashEquality)
{
    Type testType1 = types().get_type<TestHash::SimpleTestHash1>();
    EXPECT_NE(testType1, Type::invalid_type());

    Type testType2 = types().get_type<TestHash::SimpleTestHash2>();
    EXPECT_NE(testType2, Type::invalid_type());

    EXPECT_NE(testType1.hash(), 0);
    EXPECT_NE(testType2.hash(), 0);
    EXPECT_NE(testType1.hash(), testType2.hash());
}