#pragma once

#include <simplerttr.h>

namespace Test
{
    class Point
    {
        SIMPLE_RTTR_ALLOW_ACCESS
    public:
        Point(int x, int y) : _x(x), _y(y) {}

        int GetX() const { return _x; }
        void SetX(int x) { _x = x; }

        int GetY() const { return _y; }
        void SetY(int y) { _y = y; }

    protected:
        int _x;
        int _y;
    };
}
