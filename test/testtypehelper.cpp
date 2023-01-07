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
    EXPECT_EQ(typeHelper.Name(), "int");
}

TEST(RTTRTypeHelper, TestSTDVector)
{
    TypeHelper<std::vector<int>> typeHelper;
    EXPECT_EQ(typeHelper.Name(), "vector");
    EXPECT_EQ(typeHelper.TemplateParams().size(), 2);
}


TEST(RTTRTypeHelper, TestSTDMap)
{
//    using high_resolution_clock = std::chrono::high_resolution_clock;
//    using time_point = std::chrono::high_resolution_clock::time_point;
//    using duration = std::chrono::duration<std::int64_t>;

//    high_resolution_clock clock;
//    time_point start = clock.now();
//    std::size_t beforeAllocs = CustomAllocatorStats::TotalAllocations;
//    std::size_t beforeDeallocs = CustomAllocatorStats::TotalDeallocations;

    TypeHelper<std::unordered_map<std::string, int>> typeHelper;
//    time_point end = clock.now();
//    std::size_t afterAllocs = CustomAllocatorStats::TotalAllocations;
//    std::size_t afterDeallocs = CustomAllocatorStats::TotalDeallocations;

//    std::chrono::nanoseconds totalTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
//    printf("Allocations   : %zu\n", afterAllocs - beforeAllocs);
//    printf("Deallocations : %zu\n", afterDeallocs - beforeDeallocs);
//    printf("Time          : %zu\n", totalTime.count());

    EXPECT_EQ(typeHelper.Name(), "unordered_map");
    EXPECT_EQ(typeHelper.TemplateParams().size(), 5);
}
