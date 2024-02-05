#pragma once

namespace SimpleRTTR
{
    class Value
    {
    public:
        inline Value(stdrttr::string name, class Variant value);

        inline const stdrttr::string& Name() const;
        inline const Variant& Variant() const;

        inline const MetaContainer& Meta() const;

        inline bool operator==(const Value& other) const;

    protected:
        friend MetaContainer& _InternalGetMetadata(class Value& value);

        stdrttr::string _Name;
        class Variant _Value;
        MetaContainer _Meta;
    };

    class ValueContainer : public DefaultContainer<Value>
    {
    };
}