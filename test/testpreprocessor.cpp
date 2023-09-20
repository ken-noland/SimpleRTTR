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
    const Meta& file = type.Meta().Get("source_filename");
    ASSERT_NE(file, Meta::InvalidMeta());

    const char* filename = file.Value().GetAs<const char*>();
    EXPECT_STREQ(filename, __FILE__);
}

TEST(RTTRPreprocessor, TestPreprocessorJustTheRegisteredType)
{
    //test that the type registered is the ONLY type registered with the metadata
    int count = 0;
    for (Type type : Types())
    {
        const Meta& meta = type.Meta().Get("source_filename");
        if(meta != Meta::InvalidMeta())
        {
            const char* strFilename = meta.Value().GetAs<const char*>();
            //std::cout << "Type " << type.Name() << " declared in " << strFilename << std::endl;
            if (std::strcmp(strFilename, __FILE__) == 0)
            {
                count++;
            }
        }
    }

    EXPECT_EQ(count, 1);
}