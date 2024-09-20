#include "order.h"

using namespace Test;

SIMPLERTTR
{
    SimpleRTTR::registration().type<Order>()
        .meta("description", "A simple order class")
        .constructor<const std::string&, const std::string&, int>({"CustomerName", "ProductName", "Quantity"})

        .property(&Order::CustomerName, "CustomerName")
        .property(&Order::ProductName, "ProductName")
        .property(&Order::Quantity, "Quantity");
}
