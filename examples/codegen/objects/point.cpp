#include "point.h"

using namespace Test;

SIMPLERTTR
{
    SimpleRTTR::registration().type<Point>()
        .meta("description", "A simple point class")
        .constructor()
        .constructor<int, int>({"X", "Y"})

        .property(&Point::X, "X")
            .meta("description", "The x coordinate of the point")
        .property(&Point::Y, "Y")
            .meta("description", "The y coordinate of the point");
}