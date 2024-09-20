#pragma once

namespace SimpleRTTR
{
    class Value
    {
    public:
        inline Value(std::string name, class Variant value);

        inline const std::string& name() const;
        inline const Variant& value() const;

        inline const MetaContainer& meta() const;

        inline std::size_t hash() const;

        inline bool operator==(const Value& other) const;

    protected:
        friend MetaContainer& _InternalGetMetadata(class Value& value);

        std::string _Name;
        class Variant _Value;
        MetaContainer _Meta;
    };

    class ValueContainer : public DefaultContainer<Value>
    {
    };
}