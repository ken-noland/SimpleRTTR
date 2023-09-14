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
        using MetaList = stdrttr::vector<Meta>;

        inline PropertyData(const stdrttr::string& name, const stdrttr::string& type, std::size_t offset);
        inline PropertyData(PropertyData&& data);
        inline PropertyData(const PropertyData& data);

        inline const stdrttr::string& Name() const;
        inline const class Type Type() const;
        inline const std::size_t Offset() const;
        inline const MetaList& Meta() const;

    protected:
        stdrttr::string _Name;
        stdrttr::string _Type;
        std::size_t _Offset;
        PropertyFlagsBits _Flags;


        friend MetaList& _InternalPropertyDataGetMetaListRef(PropertyData& prop);
        MetaList _Meta;
    };

    class Property
    {
    public:
        using MetaList = PropertyData::MetaList;

        inline Property(Property&& data);
        inline Property(const Property& data);
        inline Property(const PropertyData& data);
        inline Property& operator=(const Property& prop);

        inline const stdrttr::string& Name() const;
        inline const class Type Type() const;
        inline const std::size_t Offset() const;
        inline const MetaList& Meta() const;

        using MetaFunction = std::function<void(const class Meta&)>;
        inline void ForEach(MetaFunction eval) const;


        inline bool IsConst();
        inline bool IsPointer();

    protected:
        friend PropertyData& _InternalPropertyGetPropertyDataRef(Property& prop);
        PropertyData _PropData;
    };
}
