#include "order.h"

using namespace Test;

SIMPLERTTR
{
    SimpleRTTR::Registration().Type<Order>()
        .Meta("description", "A simple order class")
        .Constructor<const std::string&, const std::string&, int>({"CustomerName", "ProductName", "Quantity"})

        .Property(&Order::CustomerName, "CustomerName")
        .Property(&Order::ProductName, "ProductName")
        .Property(&Order::Quantity, "Quantity");
}
