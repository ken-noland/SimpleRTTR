#include "order.h"

using namespace Test;

SIMPLERTTR
{
    SimpleRTTR::Registration().Type<Order>()
        .Meta("description", "A simple order class")
        //.Constructor({"customer", "product", "quantity"})  //TODO: add constructor support

        .Property(&Order::_customerName, "customer")
        .Property(&Order::_productName, "product")
        .Property(&Order::_quantity, "quantity");
}
