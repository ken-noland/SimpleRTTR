#include <gtest/gtest.h>
#include <simplerttr.h>

using namespace SimpleRTTR;

/***



*/

class FactoryClass1
{
public:
    int SomeValue;
    int SomeOtherValue;
    std::string SomeDescription;
};

class Factory
{
public:
    FactoryClass1* Create()
    {
        FactoryClass1* instance = new FactoryClass1();

        // read into the type properties and find any default values specified in the meta data
        Type type = Types().GetType<FactoryClass1>();
        for (auto& prop : type.Properties())
        {
            if (prop.Meta().Has("Default"))
            {
                Variant var = prop.Meta().Get("Default");
//                prop.Set(instance, var);
            }
        }
    }
};


SIMPLERTTR
{
    Registration().Type<FactoryClass1>()
        .Property(&FactoryClass1::SomeValue, "SomeValue")
            .Meta("Default", 5)
        .Property(&FactoryClass1::SomeOtherValue, "SomeOtherValue")
            .Meta("Default", 2)
        .Property(&FactoryClass1::SomeDescription, "SomeDescription")
            .Meta("Default", "Some Text");
}

TEST(RTTRType, TestTypeCreation)
{
    Type type = Types().GetType<FactoryClass1>();

    EXPECT_EQ(type.Name(), "FactoryClass1");
    EXPECT_EQ(type.Size(), sizeof(FactoryClass1));

    EXPECT_EQ(type.Properties().Size(), 3);
}