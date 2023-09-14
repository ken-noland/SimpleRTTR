#include <gtest/gtest.h>
#include <simplerttr.h>

using namespace SimpleRTTR;

class SimpleRTTRTestPreprocessor
{
public:
    char property1;
    char property2;
    char property3;
};

SIMPLERTTR
{
    Registration().Type<SimpleRTTRTestPreprocessor>()
        .Property(&SimpleRTTRTestPreprocessor::property1, "property1")
        .Property(&SimpleRTTRTestPreprocessor::property2, "property2")
        .Property(&SimpleRTTRTestPreprocessor::property3, "property3");
}

TEST(RTTRPreprocessor, TestPreprocessorWorks)
{
    Type type = Types().GetType<SimpleRTTRTestPreprocessor>();
    Type::MetaList::const_iterator cppFileIter = std::find_if(type.Meta().begin(), type.Meta().end(), [&](const Meta& meta) {
        printf(meta.Key().Type().Name().c_str()); printf("\n");
        return false;
        });

    //EXPECT_NE(cppFileIter, type.Meta().end());
}