#include <gtest/gtest.h>
#include <simplerttr.h>

using namespace SimpleRTTR;

/***

There's no default way in C++ to override the construction of a class and to set default 
values for the properties of that class after the default constructor has been called.
In cases where you want to create an object that has values set in the meta data, you
can use a factory class to create the object and set the default values after the object
has been created.

This example shows how to use the meta data to set default values for the properties of
a class after the object has been created.

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
    //note: you can use a template here to create any type of object
    FactoryClass1* Create()
    {
        FactoryClass1* instance = new FactoryClass1();

        // read into the type properties and find any default values specified in the meta data
        Type type = types().get_type<FactoryClass1>();
        for (const Property& prop : type.properties())
        {
            if (prop.meta().has("Default"))
            {
                //get the default value from the meta data and set the property on the class instance
                Variant var = prop.meta().get("Default").value();
                prop.set(instance, var);
            }
        }

        return instance;
    }
};


SIMPLERTTR
{
    registration().type<FactoryClass1>()
        .property(&FactoryClass1::SomeValue, "SomeValue")
            .meta("Default", 5)
        .property(&FactoryClass1::SomeOtherValue, "SomeOtherValue")
            .meta("Default", 2)
        .property(&FactoryClass1::SomeDescription, "SomeDescription")
            .meta("Default", "Some Text");
}

TEST(RTTRType, TestTypeCreation)
{
    GTEST_SKIP_("The ability to copy variants from one value type(const char*) to another (std::string) is not supported at this time, so skipping this test.");

    Type type = types().get_type<FactoryClass1>();

    EXPECT_EQ(type.name(), "FactoryClass1");
    EXPECT_EQ(type.size(), sizeof(FactoryClass1));

    EXPECT_EQ(type.properties().size(), 3);

    Factory factory;

    FactoryClass1 *instance = nullptr;
    instance = factory.Create();

    EXPECT_EQ(instance->SomeValue, 5);
    EXPECT_EQ(instance->SomeOtherValue, 2);
    EXPECT_EQ(instance->SomeDescription, "Some Text");
}