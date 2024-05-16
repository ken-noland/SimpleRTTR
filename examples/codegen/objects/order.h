#pragma once

#include <simplerttr.h>

namespace Test
{
    class Order
    {
        SIMPLE_RTTR_ALLOW_ACCESS
    public:
        Order(const std::string& customerName, const std::string& productName, int quantity) : _customerName(customerName), _productName(productName), _quantity(quantity) {}

    protected:
        std::string _customerName;
        std::string _productName;
        int _quantity;
    };
}