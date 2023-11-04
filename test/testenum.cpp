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

    EXPECT_EQ(enumType.HasValue("RED"), true);
    EXPECT_EQ(enumType.HasValue("GREEN"), true);
    EXPECT_EQ(enumType.HasValue("BLUE"), true);
    EXPECT_EQ(enumType.HasValue("ALPHA"), true);

    EXPECT_EQ(enumType.Value("RED").GetAs<Color>(), Color::RED);
    EXPECT_EQ(enumType.Value("GREEN").GetAs<Color>(), Color::GREEN);
    EXPECT_EQ(enumType.Value("BLUE").GetAs<Color>(), Color::BLUE);
    EXPECT_EQ(enumType.Value("ALPHA").GetAs<Color>(), Color::ALPHA);
}

TEST(RTTREnum, TestEnumValueDoesNotExist)
{
    Type enumType = Types().GetType<Color>();

    EXPECT_EQ(enumType.Name(), "Color");

    EXPECT_EQ(enumType.HasValue("X"), false);

    EXPECT_THROW(enumType.Value("X").GetAs<Color>(), std::bad_any_cast);
}
