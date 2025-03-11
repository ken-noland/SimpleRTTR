#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>

#include <simplerttr.h>

using namespace SimpleRTTR;



namespace SimpleRTTRTest
{
    class SimpleRTTRTestNamespaceType
    {
    public:
    };

    namespace Namespace2
    {
        class SimpleRTTRTestNamespaceType2
        {
        public:
        };
    }

    template<typename A>
    class SimpleRTTRTestNamespaceWithTemplate
    {
    public:
        class SimpleRTTRTestClassWithinTemplateAndNamespace
        {

        };

        template<typename B>
        class SimpleRTTRTestTemplateWithinTemplateAndNamespace
        {

        };
    };
}

class SimpleRTTRTestType
{
public:
};

SIMPLERTTR
{

}

TEST(RTTRType, TestHasType)
{
    class SimpleRTTRTestType1
    {
    public:
    };

    EXPECT_EQ(types().has_type<SimpleRTTRTestType1>(), false);
    registration().type<SimpleRTTRTestType1>();
    EXPECT_EQ(types().has_type<SimpleRTTRTestType1>(), true);
}


TEST(RTTRType, TestGetType)
{
    std::optional<Type> invalidType = types().get_type<SimpleRTTRTestType>();
    EXPECT_FALSE(invalidType);

    registration().type<SimpleRTTRTestType>();

    Type validType = types().get_type<SimpleRTTRTestType>().value();
    EXPECT_EQ(validType.name(), "SimpleRTTRTestType");
    EXPECT_EQ(validType.has_flag(TypeFlag::IsEnum), false);
}


TEST(RTTRType, TestNamespaceType)
{
    registration().type<SimpleRTTRTest::SimpleRTTRTestNamespaceType>();

    Type validType = types().get_type<SimpleRTTRTest::SimpleRTTRTestNamespaceType>().value();
    EXPECT_EQ(validType.name(), "SimpleRTTRTestNamespaceType");
    ASSERT_EQ(validType.namespaces().size(), 1);
    EXPECT_EQ(validType.namespaces()[0], "SimpleRTTRTest");
}

TEST(RTTRType, TestSTLVector)
{
    Type validType = types().get_or_create_type<std::vector<int>>();
    EXPECT_EQ(validType.name(), "vector");
    ASSERT_EQ(validType.template_params().size(), 2);

    Type param1 = validType.template_params()[0].type().type();
    Type param2 = validType.template_params()[1].type().type();

    EXPECT_EQ(param1.name(), "int");
    EXPECT_EQ(param2.name(), "allocator");

    ASSERT_EQ(param2.template_params().size(), 1);
    Type allocParam = param2.template_params()[0].type().type();
    EXPECT_EQ(allocParam.name(), "int");
}


TEST(RTTRType, TestTemplateTypeComaredToTypeInfo)
{
    Type vectorType1 = types().get_or_create_type<std::vector<int>>();

    Type vectorType2 = types().get_type(typeid(std::vector<int>)).value();
    Type invalidType = types().get_type<int>().value();

    EXPECT_EQ(vectorType1, vectorType2);
    EXPECT_NE(vectorType1, invalidType);
}

TEST(RTTRType, TestTemplateWithinNamespace)
{
    registration().type<SimpleRTTRTest::SimpleRTTRTestNamespaceWithTemplate<int>>();

    Type validType = types().get_type<SimpleRTTRTest::SimpleRTTRTestNamespaceWithTemplate<int>>().value();
    EXPECT_EQ(validType.name(), "SimpleRTTRTestNamespaceWithTemplate");
    ASSERT_EQ(validType.namespaces().size(), 1);
    EXPECT_EQ(validType.namespaces()[0], "SimpleRTTRTest");
}

TEST(RTTRType, TestBasicTypes)
{
    EXPECT_EQ(types().get_type<void>().value().name(), "void");
    EXPECT_EQ(types().get_type<void>().value().size(), 0);

    EXPECT_EQ(types().get_type<char>().value().name(), "char");
    EXPECT_EQ(types().get_type<char>().value().size(), sizeof(char));

    EXPECT_EQ(types().get_type<short>().value().name(), "short");
    EXPECT_EQ(types().get_type<short>().value().size(), sizeof(short));

    EXPECT_EQ(types().get_type<int>().value().name(), "int");
    EXPECT_EQ(types().get_type<int>().value().size(), sizeof(int));

    EXPECT_EQ(types().get_type<long>().value().name(), "long");
    EXPECT_EQ(types().get_type<long>().value().size(), sizeof(long));

    EXPECT_EQ(types().get_type<float>().value().name(), "float");
    EXPECT_EQ(types().get_type<float>().value().size(), sizeof(float));

    EXPECT_EQ(types().get_type<double>().value().name(), "double");
    EXPECT_EQ(types().get_type<double>().value().size(), sizeof(double));
}

TEST(RTTRType, TestExistingTemplateTypeRegistration)
{
    registration().type<std::vector<int>>();
    Type validType = types().get_type<std::vector<int>>().value();
    EXPECT_EQ(validType.name(), "vector");

    Type existingType = types().get_type<std::vector<int>>().value();
    registration().type(existingType)
        .meta("test_key", "test_value");

    //check that the metadata was added to the existing type
    Type existingType2 = types().get_type<std::vector<int>>().value();
    const MetaContainer& metadata = existingType2.meta();
    EXPECT_EQ(metadata.size(), 1);
    EXPECT_EQ(metadata[0].key(), "test_key");
    EXPECT_EQ(metadata[0].value(), "test_value");
}


enum class SimpleRTTREnum1
{
    Value1,
    Value2
};

enum SimpleRTTREnum2
{
    Value1,
    Value2
};

TEST(RTTRType, TestTypeIsEnum)
{
    registration().type<SimpleRTTREnum1>();
    registration().type<SimpleRTTREnum2>();

    Type enumType1 = types().get_type<SimpleRTTREnum1>().value();
    EXPECT_EQ(enumType1.has_flag(TypeFlag::IsEnum), true);

    Type enumType2 = types().get_type<SimpleRTTREnum2>().value();
    EXPECT_EQ(enumType2.has_flag(TypeFlag::IsEnum), true);
}