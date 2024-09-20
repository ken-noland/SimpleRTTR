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
    registration().type<Color>()
        .meta("description", "specific photon wavelengths of the visible spectrum")
        .value(RED, "RED")
            .meta("description", "it's RED")
        .value(GREEN, "GREEN")
            .meta("description", "it's GREEN")
        .value(BLUE, "BLUE")
            .meta("description", "it's BLUE")
        .value(ALPHA, "ALPHA")
            .meta("description", "it's ALPHA");
}

TEST(RTTREnum, TestBasicEnum)
{
    Type enumType = types().get_type<Color>();

    EXPECT_EQ(enumType.name(), "Color");
    EXPECT_TRUE(enumType.has_flag(TypeFlag::IsEnum));

    auto exists = [enumType](const std::string name) 
        { 
            return std::find_if(enumType.values().begin(), enumType.values().end(), [name](const Value& value) 
                { 
                    return value.name() == name; 
                }) != enumType.values().end(); 
        };

    EXPECT_EQ(exists("RED"), true);
    EXPECT_EQ(exists("GREEN"), true);
    EXPECT_EQ(exists("BLUE"), true);
    EXPECT_EQ(exists("ALPHA"), true);

    auto value = [enumType](const std::string name) 
        { 
        return std::find_if(enumType.values().begin(), enumType.values().end(), [name](const Value& value) 
            { 
                return value.name() == name; 
            })->value().get_as<Color>(); 
        };

    EXPECT_EQ(value("RED"), Color::RED);
    EXPECT_EQ(value("GREEN"), Color::GREEN);
    EXPECT_EQ(value("BLUE"), Color::BLUE);
    EXPECT_EQ(value("ALPHA"), Color::ALPHA);
}

TEST(RTTREnum, TestEnumValueDoesNotExist)
{
    Type enumType = types().get_type<Color>();

    EXPECT_EQ(enumType.name(), "Color");

    EXPECT_EQ(std::find_if(enumType.values().begin(), enumType.values().end(), [](const Value& value) { return value.name() == "X"; }), enumType.values().end());
}

TEST(RTTREnum, TestEnumConvertedToInt)
{
    Type enumType = types().get_type<Color>();

    EXPECT_EQ(enumType.name(), "Color");

    auto exists = [enumType](const std::string name) 
        { 
            return std::find_if(enumType.values().begin(), enumType.values().end(), [name](const Value& value) 
                { 
                    return value.name() == name; 
                }) != enumType.values().end(); 
        };

    EXPECT_EQ(exists("RED"), true);
    EXPECT_EQ(exists("GREEN"), true);
    EXPECT_EQ(exists("BLUE"), true);
    EXPECT_EQ(exists("ALPHA"), true);

    auto value = [enumType](const std::string name) 
        { 
        return std::find_if(enumType.values().begin(), enumType.values().end(), [name](const Value& value) 
            { 
                return value.name() == name; 
            })->value().get_as<int>(); 
        };

    EXPECT_EQ(value("RED"), Color::RED);
    EXPECT_EQ(value("GREEN"), Color::GREEN);
    EXPECT_EQ(value("BLUE"), Color::BLUE);
    EXPECT_EQ(value("ALPHA"), Color::ALPHA);
}