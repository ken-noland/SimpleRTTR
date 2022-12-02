#include "testallocator.h"

size_t CustomAllocatorStats::TotalAllocations = 0;
size_t CustomAllocatorStats::TotalDeallocations = 0;

size_t CustomAllocatorStats::TotalAllocationSize = 0;
size_t CustomAllocatorStats::TotalDeallocationSize = 0;

size_t CustomAllocatorStats::TotalAllocatedSize = 0;