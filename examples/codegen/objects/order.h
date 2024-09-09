#pragma once

#include <simplerttr.h>

namespace Test
{
    class Order
    {
    public:
        Order(const std::string& customerName, const std::string& productName, int quantity) : CustomerName(customerName), ProductName(productName), Quantity(quantity) {}

        std::string CustomerName;
        std::string ProductName;
        int Quantity;
    };
}