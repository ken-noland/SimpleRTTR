#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>

//need to include the allocator before the SimpleRTTR header file
//#include "testallocator.h"
//#define SIMPLERTTR_CUSTOM_ALLOCATOR CustomAllocator

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

    EXPECT_EQ(Types().HasType<SimpleRTTRTestType1>(), false);
    Registration().Type<SimpleRTTRTestType1>();
    EXPECT_EQ(Types().HasType<SimpleRTTRTestType1>(), true);
}


TEST(RTTRType, TestGetType)
{
    Type invalidType = Types().GetType<SimpleRTTRTestType>();
    EXPECT_EQ(invalidType, Type::InvalidType());

    Registration().Type<SimpleRTTRTestType>();

    Type validType = Types().GetType<SimpleRTTRTestType>();
    EXPECT_EQ(validType.Name(), "SimpleRTTRTestType");
    EXPECT_EQ(validType.IsEnum(), false);
}


TEST(RTTRType, TestNamespaceType)
{
    Registration().Type<SimpleRTTRTest::SimpleRTTRTestNamespaceType>();

    Type validType = Types().GetType<SimpleRTTRTest::SimpleRTTRTestNamespaceType>();
    EXPECT_EQ(validType.Name(), "SimpleRTTRTestNamespaceType");
    ASSERT_EQ(validType.Namespaces().size(), 1);
    EXPECT_EQ(validType.Namespaces()[0], "SimpleRTTRTest");
}

TEST(RTTRType, TestSTLVector)
{
    Type validType = Types().GetOrCreateType<std::vector<int>>();
    EXPECT_EQ(validType.Name(), "vector");
    ASSERT_EQ(validType.TemplateParams().size(), 2);

    Type param1 = validType.TemplateParams()[0].Type();
    Type param2 = validType.TemplateParams()[1].Type();

    EXPECT_EQ(param1.Name(), "int");
    EXPECT_EQ(param2.Name(), "allocator");

    ASSERT_EQ(param2.TemplateParams().size(), 1);
    Type allocParam = param2.TemplateParams()[0].Type();
    EXPECT_EQ(allocParam.Name(), "int");
}


TEST(RTTRType, TestTemplateTypeComaredToTypeInfo)
{
    Type vectorType1 = Types().GetOrCreateType<std::vector<int>>();

    Type vectorType2 = Types().GetType(typeid(std::vector<int>));
    Type invalidType = Types().GetType<int>();

    EXPECT_EQ(vectorType1, vectorType2);
    EXPECT_NE(vectorType1, invalidType);
}

TEST(RTTRType, TestTemplateWithinNamespace)
{
    Registration().Type<SimpleRTTRTest::SimpleRTTRTestNamespaceWithTemplate<int>>();

    Type validType = Types().GetType<SimpleRTTRTest::SimpleRTTRTestNamespaceWithTemplate<int>>();
    EXPECT_EQ(validType.Name(), "SimpleRTTRTestNamespaceWithTemplate");
    ASSERT_EQ(validType.Namespaces().size(), 1);
    EXPECT_EQ(validType.Namespaces()[0], "SimpleRTTRTest");
}

TEST(RTTRType, TestBasicTypes)
{
    EXPECT_EQ(Types().GetType<void>().Name(), "void");
    EXPECT_EQ(Types().GetType<void>().Size(), 0);

    EXPECT_EQ(Types().GetType<char>().Name(), "char");
    EXPECT_EQ(Types().GetType<char>().Size(), sizeof(char));

    EXPECT_EQ(Types().GetType<short>().Name(), "short");
    EXPECT_EQ(Types().GetType<short>().Size(), sizeof(short));

    EXPECT_EQ(Types().GetType<int>().Name(), "int");
    EXPECT_EQ(Types().GetType<int>().Size(), sizeof(int));

    EXPECT_EQ(Types().GetType<long>().Name(), "long");
    EXPECT_EQ(Types().GetType<long>().Size(), sizeof(long));

    EXPECT_EQ(Types().GetType<float>().Name(), "float");
    EXPECT_EQ(Types().GetType<float>().Size(), sizeof(float));

    EXPECT_EQ(Types().GetType<double>().Name(), "double");
    EXPECT_EQ(Types().GetType<double>().Size(), sizeof(double));
}

TEST(RTTRType, TestExistingTemplateTypeRegistration)
{
    Registration().Type<std::vector<int>>();
    Type validType = Types().GetType<std::vector<int>>();
    EXPECT_EQ(validType.Name(), "vector");

    const Type& existingType = Types().GetType<std::vector<int>>();
    Registration().Type(existingType)
        .Meta("test", "test");

    //check that the metadata was added to the existing type
    const Type& existingType2 = Types().GetType<std::vector<int>>();
    const MetaContainer& metadata = existingType2.Meta();
    EXPECT_EQ(metadata.Size(), 1);
    EXPECT_EQ(metadata[0].Key(), "test");
    EXPECT_EQ(metadata[0].Value().ToString(), "test");
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
    Registration().Type<SimpleRTTREnum1>();
    Registration().Type<SimpleRTTREnum2>();

    Type enumType1 = Types().GetType<SimpleRTTREnum1>();
    EXPECT_EQ(enumType1.IsEnum(), true);

    Type enumType2 = Types().GetType<SimpleRTTREnum2>();
    EXPECT_EQ(enumType2.IsEnum(), true);
}