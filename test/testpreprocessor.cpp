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
    registration().type<SimpleRTTRTestPreprocessor>()
        .property(&SimpleRTTRTestPreprocessor::property1, "property1")
        .property(&SimpleRTTRTestPreprocessor::property2, "property2")
        .property(&SimpleRTTRTestPreprocessor::property3, "property3");
}

TEST(RTTRPreprocessor, TestPreprocessorWorks)
{
    Type type = types().get_type<SimpleRTTRTestPreprocessor>().value();
    const Meta& file = type.meta().get("source_filename");
    ASSERT_NE(file, Meta::invalid_meta());

    const char* filename = file.value().get_as<const char*>();
    EXPECT_STREQ(filename, __FILE__);
}

TEST(RTTRPreprocessor, TestPreprocessorJustTheRegisteredType)
{
    //test that the type registered is the ONLY type registered with the metadata
    int count = 0;
    for (Type type : types())
    {
        const Meta& meta = type.meta().get("source_filename");
        if(meta != Meta::invalid_meta())
        {
            const char* strFilename = meta.value().get_as<const char*>();
            //std::cout << "Type " << type.name() << " declared in " << strFilename << std::endl;
            if (std::strcmp(strFilename, __FILE__) == 0)
            {
                count++;
            }
        }
    }

    EXPECT_EQ(count, 1);
}