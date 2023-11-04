#pragma once

namespace SimpleRTTR
{
    class Value
    {
    public:
        inline Value(stdrttr::string name, class Variant value);

        inline const stdrttr::string& Name() const;
        inline const Variant& Variant() const;

    protected:
        stdrttr::string _Name;
        class Variant _Value;
    };
}