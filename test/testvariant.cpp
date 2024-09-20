#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>

#include <simplerttr.h>

using namespace SimpleRTTR;


TEST(RTTRVariant, TestBasicInt)
{
    Variant var(42);
    EXPECT_EQ(var.type(), types().get_type<int>());

    int value = var.get_as<int>();
    EXPECT_EQ(value, 42);
}

TEST(RTTRVariant, TestBasicChar)
{
    Variant var('a');
    EXPECT_EQ(var.type(), types().get_type<char>());

    char value = var.get_as<char>();
    EXPECT_EQ(value, 'a');
}

TEST(RTTRVariant, TestBasicShort)
{
    Variant var(short(42));
    EXPECT_EQ(var.type(), types().get_type<short>());

    short value = var.get_as<short>();
    EXPECT_EQ(value, 42);
}

TEST(RTTRVariant, TestBasicCharString)
{
    Variant var("This is a test!");
    EXPECT_EQ(var.type(), types().get_type<const char*>());

    const char* value = var.get_as<const char*>();
    EXPECT_STREQ(value, "This is a test!");

    std::string strValue = var.get_as<std::string>();
    EXPECT_EQ(strValue, "This is a test!");
}

TEST(RTTRVariant, TestBasicStdString)
{
    Variant var(std::string("This is a test!"));
    EXPECT_EQ(var.type(), types().get_type<std::string>());

    std::string value = var.get_as<std::string>();
    EXPECT_EQ(value, "This is a test!");

    const char* strValue = var.get_as<const char*>();
    EXPECT_STREQ(strValue, "This is a test!");
}

TEST(RTTRVariant, Test8BitConversion)
{
    char c = 42;
    Variant var(c);

    EXPECT_EQ(var.type(), types().get_type<char>());

    EXPECT_EQ(var.get_as<char>(), 42);
    EXPECT_EQ(var.get_as<short>(), 42);
    EXPECT_EQ(var.get_as<int>(), 42);
    EXPECT_EQ(var.get_as<long>(), 42);
    EXPECT_EQ(var.get_as<unsigned char>(), 42);
    EXPECT_EQ(var.get_as<unsigned short>(), 42);
    EXPECT_EQ(var.get_as<unsigned int>(), 42);
    EXPECT_EQ(var.get_as<unsigned long>(), 42);

    EXPECT_ANY_THROW(var.get_as<std::string>());
}


TEST(RTTRVariant, Test64BitConversion)
{
    unsigned long c = 42;
    Variant var(c);

    EXPECT_EQ(var.type(), types().get_type<unsigned long>());

    EXPECT_EQ(var.get_as<char>(), 42);
    EXPECT_EQ(var.get_as<short>(), 42);
    EXPECT_EQ(var.get_as<int>(), 42);
    EXPECT_EQ(var.get_as<long>(), 42);
    EXPECT_EQ(var.get_as<unsigned char>(), 42);
    EXPECT_EQ(var.get_as<unsigned short>(), 42);
    EXPECT_EQ(var.get_as<unsigned int>(), 42);
    EXPECT_EQ(var.get_as<unsigned long>(), 42);

    EXPECT_ANY_THROW(var.get_as<std::string>());
}

TEST(RTTRVariant, TestUint64BitConversion)
{
    uint64_t c = 42;
    Variant var(c);

    EXPECT_EQ(var.type(), types().get_type<uint64_t>());

    EXPECT_EQ(var.get_as<char>(), 42);
    EXPECT_EQ(var.get_as<short>(), 42);
    EXPECT_EQ(var.get_as<int>(), 42);
    EXPECT_EQ(var.get_as<long>(), 42);
    EXPECT_EQ(var.get_as<unsigned char>(), 42);
    EXPECT_EQ(var.get_as<unsigned short>(), 42);
    EXPECT_EQ(var.get_as<unsigned int>(), 42);
    EXPECT_EQ(var.get_as<unsigned long>(), 42);

    EXPECT_ANY_THROW(var.get_as<std::string>());
}

TEST(RTTRVariant, TestFloatConversion)
{
    float c = 42.0f;
    Variant var(c);

    EXPECT_EQ(var.type(), types().get_type<float>());

    EXPECT_EQ(var.get_as<float>(), 42.0f);
    EXPECT_EQ(var.get_as<double>(), 42.0);

    EXPECT_ANY_THROW(var.get_as<std::string>());
}

TEST(RTTRVariant, TestDoubleConversion)
{
    double c = 42.0;
    Variant var(c);

    EXPECT_EQ(var.type(), types().get_type<double>());

    EXPECT_EQ(var.get_as<float>(), 42.0f);
    EXPECT_EQ(var.get_as<double>(), 42.0);

    EXPECT_ANY_THROW(var.get_as<std::string>());
}

TEST(RTTRVariant, TestBasicEnum8bit)
{
    enum class Color : uint8_t
    {
        RED,
        GREEN,
        BLUE,
        ALPHA
    };

    Variant var(Color::RED);
    EXPECT_EQ(var.type(), types().get_type<Color>());

    Color value = var.get_as<Color>();
    EXPECT_EQ(value, Color::RED);

    int intValue = var.get_as<int>();
    EXPECT_EQ(intValue, (int)Color::RED);
}

TEST(RTTRVariant, TestBasicEnum16bit)
{
    enum class Color : uint16_t
    {
        RED,
        GREEN,
        BLUE,
        ALPHA
    };
    Variant var(Color::RED);
    EXPECT_EQ(var.type(), types().get_type<Color>());
    Color value = var.get_as<Color>();
    EXPECT_EQ(value, Color::RED);
    int intValue = var.get_as<int>();
    EXPECT_EQ(intValue, (int)Color::RED);
}

TEST(RTTRVariant, TestBasicEnum32bit)
{
    enum class Color : uint32_t
    {
        RED,
        GREEN,
        BLUE,
        ALPHA
    };
    Variant var(Color::RED);
    EXPECT_EQ(var.type(), types().get_type<Color>());
    Color value = var.get_as<Color>();
    EXPECT_EQ(value, Color::RED);
    int intValue = var.get_as<int>();
    EXPECT_EQ(intValue, (int)Color::RED);
}

TEST(RTTRVariant, TestBasicEnum64bit)
{
    enum class Color : uint64_t
    {
        RED,
        GREEN,
        BLUE,
        ALPHA
    };
    Variant var(Color::RED);
    EXPECT_EQ(var.type(), types().get_type<Color>());
    Color value = var.get_as<Color>();
    EXPECT_EQ(value, Color::RED);
    int intValue = var.get_as<int>();
    EXPECT_EQ(intValue, (int)Color::RED);
}