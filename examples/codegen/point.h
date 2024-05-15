#pragma once

#include <simplerttr.h>
namespace Test
{
    class Point
    {
        SIMPLE_RTTR_ALLOW_ACCESS
    public:
        Point(int x, int y) : _x(x), _y(y) {}

    protected:
        int _x;
        int _y;
    };
}
