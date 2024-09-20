#include "customer.h"

using namespace Test;

SIMPLERTTR
{
    SimpleRTTR::registration().type<Customer>()
        .meta("description", "A simple customer class")
        //.Constructor({"name", "address"})  //TODO: add constructor support

        .property(&Customer::_name, "name")
        .property(&Customer::_address, "address");
}