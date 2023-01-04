#pragma once

namespace SimpleRTTR
{
    enum class PropertyFlags : uint16_t
    {
        IsConst = 0,
        IsPointer,
        PropertyFlagsMax
    };

    class PropertyData
    {
    public:
        using PropertyFlagsBits = std::bitset<(uint16_t)PropertyFlags::PropertyFlagsMax>;

        inline PropertyData(stdrttr::string Name, const class Type& type, std::size_t offset, PropertyFlagsBits flags);
        inline PropertyData(PropertyData&& data);
        inline PropertyData(const PropertyData& data);

        inline const stdrttr::string& Name() const;
        inline const class Type& Type() const;
        inline const std::size_t Offset() const;

        inline const PropertyFlagsBits& Flags() const;

    protected:
        stdrttr::string _Name;
        const class Type& _Type;
        std::size_t _Offset;
        PropertyFlagsBits _Flags;
    };

    class Property
    {
    public:
        inline Property(Property&& data);
        inline Property(const Property& data);
        inline Property(const PropertyData& data);
        inline Property& operator=(const Property& prop);

        inline const stdrttr::string& Name() const;
        inline const class Type& Type() const;
        inline const std::size_t Offset() const;

        inline bool IsConst();
        inline bool IsPointer();

    protected:
        PropertyData _PropData;
    };
}
