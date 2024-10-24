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
    std::optional<Type> testType1 = types().get_type<TestHash::SimpleTestHash1>();
    EXPECT_TRUE(testType1);

    std::optional<Type> testType2 = types().get_type<TestHash::SimpleTestHash2>();
    EXPECT_TRUE(testType2);

    EXPECT_NE(testType1.value().hash(), 0);
    EXPECT_NE(testType2.value().hash(), 0);
    EXPECT_NE(testType1.value().hash(), testType2.value().hash());
}