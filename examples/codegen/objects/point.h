#pragma once

#include <simplerttr.h>

namespace Test
{
    class Point
    {
    public:
        Point() : X(0), Y(0) {}
        Point(int x, int y) : X(x), Y(y) {}

        int X;
        int Y;
    };
}
