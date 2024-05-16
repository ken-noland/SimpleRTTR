#pragma once

#include <simplerttr.h>
#include "order.h"

namespace Test
{
    class Customer
    {
        SIMPLE_RTTR_ALLOW_ACCESS
    public:
        Customer(const std::string& name, const std::string& address) : _name(name), _address(address) {}

    protected:
        std::string _name;
        std::string _address;

        std::vector<Order> _orders;
    };
}