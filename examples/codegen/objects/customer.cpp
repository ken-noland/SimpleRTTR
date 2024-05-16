#include "customer.h"

using namespace Test;

SIMPLERTTR
{
    SimpleRTTR::Registration().Type<Customer>()
        .Meta("description", "A simple customer class")
        //.Constructor({"name", "address"})  //TODO: add constructor support

        .Property(&Customer::_name, "name")
        .Property(&Customer::_address, "address");
}