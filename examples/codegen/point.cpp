//Make sure that preprocessor directives are handled correctly. If a file is included between 
// a #ifdef and #endif block, the include directive must be processed correctly.
#define ALWAYS_TRUE true
#if ALWAYS_TRUE
#include "point.h"
#endif

using namespace Test;

SIMPLERTTR
{
    SimpleRTTR::Registration().Type<Point>()
        .Meta("description", "A simple point class")
        //.Constructor({"x", "y"})  //TODO: add constructor support

        .Property(&Point::_x, "x")
        .Property(&Point::_y, "y");
}