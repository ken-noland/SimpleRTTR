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
    Registration().Type<TestHash::SimpleTestHash1>()
        .Property(&TestHash::SimpleTestHash1::property1, "property1")
        .Property(&TestHash::SimpleTestHash1::property2, "property2")
        .Property(&TestHash::SimpleTestHash1::property3, "property3")
    ;


    Registration().Type<TestHash::SimpleTestHash2>()
        .Property(&TestHash::SimpleTestHash2::property1, "property1")
        .Property(&TestHash::SimpleTestHash2::property2, "property2")
        .Property(&TestHash::SimpleTestHash2::property3, "property3")
        ;
}

TEST(RTTRHash, TestHashEquality)
{
    Type testType1 = Types().GetType<TestHash::SimpleTestHash1>();
    EXPECT_NE(testType1, Type::InvalidType());

    Type testType2 = Types().GetType<TestHash::SimpleTestHash2>();
    EXPECT_NE(testType2, Type::InvalidType());

    EXPECT_NE(testType1.Hash(), 0);
    EXPECT_NE(testType2.Hash(), 0);
    EXPECT_NE(testType1.Hash(), testType2.Hash());
}