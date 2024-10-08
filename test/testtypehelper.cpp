#include <gtest/gtest.h>

//need to include the allocator before the SimpleRTTR header file
//#include "testallocator.h"
//#define SIMPLERTTR_CUSTOM_ALLOCATOR CustomAllocator

#include <simplerttr.h>

#include <chrono>

using namespace SimpleRTTR;

TEST(RTTRTypeHelper, TestBasic)
{
    TypeHelper<int> typeHelper;
    EXPECT_EQ(typeHelper.name(), "int");
}

TEST(RTTRTypeHelper, TestSTDVector)
{
    TypeHelper<std::vector<int>> typeHelper;
    EXPECT_EQ(typeHelper.name(), "vector");
    EXPECT_EQ(typeHelper.template_params().size(), 2);
}


TEST(RTTRTypeHelper, TestSTDMap)
{
    TypeHelper<std::unordered_map<std::string, int>> typeHelper;

    EXPECT_EQ(typeHelper.name(), "unordered_map");
    EXPECT_EQ(typeHelper.template_params().size(), 5);
}
