#pragma once

namespace SimpleRTTR
{
    class Value
    {
    public:
        inline Value(Variant value);

    protected:
        Variant _Value;
    };

    //TODO
    template<typename T>
    class TValue : public Value
    {
    public:
        TValue(T value);

    };
}