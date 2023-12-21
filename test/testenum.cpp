#include <gtest/gtest.h>
#include <SimpleRTTR.h>

using namespace SimpleRTTR;

enum Color  //unscoped enum
{
    RED,
    GREEN,
    BLUE,
    ALPHA
};

enum class Direction : std::uint8_t //scoped enum
{
    NORTH,
    SOUTH,
    EAST,
    WEST
};


SIMPLERTTR
{
    Registration().Type<Color>()
        .Meta("description", "specific photon wavelengths of the visible spectrum")
        .Value(RED, "RED")
            .Meta("description", "it's RED")
        .Value(GREEN, "GREEN")
            .Meta("description", "it's GREEN")
        .Value(BLUE, "BLUE")
            .Meta("description", "it's BLUE")
        .Value(ALPHA, "ALPHA")
            .Meta("description", "it's ALPHA");
}

TEST(RTTREnum, TestBasicEnum)
{
    Type enumType = Types().GetType<Color>();

    EXPECT_EQ(enumType.Name(), "Color");

    auto exists = [enumType](const stdrttr::string name) 
        { 
            return std::find_if(enumType.Values().begin(), enumType.Values().end(), [name](const Value& value) 
                { 
                    return value.Name() == name; 
                }) != enumType.Values().end(); 
        };

    EXPECT_EQ(exists("RED"), true);
    EXPECT_EQ(exists("GREEN"), true);
    EXPECT_EQ(exists("BLUE"), true);
    EXPECT_EQ(exists("ALPHA"), true);

    auto value = [enumType](const stdrttr::string name) 
        { 
        return std::find_if(enumType.Values().begin(), enumType.Values().end(), [name](const Value& value) 
            { 
                return value.Name() == name; 
            })->Variant().GetAs<Color>(); 
        };

    EXPECT_EQ(value("RED"), Color::RED);
    EXPECT_EQ(value("GREEN"), Color::GREEN);
    EXPECT_EQ(value("BLUE"), Color::BLUE);
    EXPECT_EQ(value("ALPHA"), Color::ALPHA);
}

TEST(RTTREnum, TestEnumValueDoesNotExist)
{
    Type enumType = Types().GetType<Color>();

    EXPECT_EQ(enumType.Name(), "Color");

    EXPECT_EQ(std::find_if(enumType.Values().begin(), enumType.Values().end(), [](const Value& value) { return value.Name() == "X"; }), enumType.Values().end());
}
