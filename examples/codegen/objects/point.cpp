#include "point.h"

using namespace Test;

SIMPLERTTR
{
    SimpleRTTR::Registration().Type<Point>()
        .Meta("description", "A simple point class")
        .Constructor()
        .Constructor<int, int>({"X", "Y"})

        .Property(&Point::X, "X")
            .Meta("description", "The x coordinate of the point")
        .Property(&Point::Y, "Y")
            .Meta("description", "The y coordinate of the point");
}