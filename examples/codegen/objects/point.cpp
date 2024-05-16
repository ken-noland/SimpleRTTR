#include "point.h"

using namespace Test;

SIMPLERTTR
{
    SimpleRTTR::Registration().Type<Point>()
        .Meta("description", "A simple point class")
        .Constructor<int, int>({"x", "y"})

        .Property(&Point::_x, "x")
            .Meta("description", "The x coordinate of the point")
            .Meta("getter", &Point::GetX)
            .Meta("setter", &Point::SetX)
        .Property(&Point::_y, "y")
            .Meta("description", "The y coordinate of the point")
            .Meta("getter", &Point::GetY)
            .Meta("setter", &Point::SetY);
}