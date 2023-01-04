#pragma once

#if defined (__GNUC__) || defined (__GNUG__)
#include <cstddef>
#include <cstdlib>
#elif defined(_MSC_VER)
#include <xmemory>
#endif //_MSC_VER

class CustomAllocatorStats
{
public:
    static size_t TotalAllocations;
    static size_t TotalDeallocations;

    static size_t TotalAllocationSize;
    static size_t TotalDeallocationSize;

    static size_t TotalAllocatedSize;
};

template<typename ClassType>
class CustomAllocator : public CustomAllocatorStats
{
public:
    using value_type = ClassType;

    inline CustomAllocator() noexcept {}
    inline CustomAllocator(const CustomAllocator&) noexcept = default;
    template <class _Other>
    inline CustomAllocator(const CustomAllocator<_Other>&) noexcept {}
    inline ~CustomAllocator() = default;
    inline CustomAllocator& operator=(const CustomAllocator&) = default;

    inline void deallocate(ClassType* const pointer, const std::size_t count) {
        std::size_t size = count * sizeof(ClassType);
        TotalDeallocations++;
        TotalDeallocationSize += size;
        TotalAllocatedSize -= size;
        free(pointer);
    }

    inline ClassType* allocate(const std::size_t count) {
        std::size_t size = count * sizeof(ClassType);
        TotalAllocations++;
        TotalAllocationSize += size;
        TotalAllocatedSize += size;
        return static_cast<ClassType*>(malloc(size));
    }
};
