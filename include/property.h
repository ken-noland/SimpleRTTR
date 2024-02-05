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

        inline bool operator==(const PropertyData& data) const;

        inline bool Equals(const PropertyData& data) const;

        inline PropertyData(const stdrttr::string& name, const TypeReference& type, std::size_t offset);
        inline PropertyData(PropertyData&& data);
        inline PropertyData(const PropertyData& data);

        inline const stdrttr::string& Name() const;
        inline class Type Type() const;
        inline const std::size_t Offset() const;
        inline const MetaContainer& Meta() const;

        template<typename ClassType>
        inline void Set(ClassType* obj, const Variant& value) const;

    protected:
        stdrttr::string _Name;
        TypeReference _Type;
        std::size_t _Offset;
        PropertyFlagsBits _Flags;


        friend MetaContainer& _InternalPropertyDataGetMetaListRef(PropertyData& prop);
        MetaContainer _Meta;
    };

    class Property
    {
    public:
        static inline const Property& InvalidProperty();

        inline Property(Property&& data);
        inline Property(const Property& data);
        inline Property(const PropertyData& data);
        inline Property& operator=(const Property& prop);

        inline bool operator==(const Property& data) const;
        inline bool operator!=(const Property& data) const;

        inline bool Equals(const Property& data) const;

        inline const stdrttr::string& Name() const;
        inline const class Type Type() const;
        inline const std::size_t Offset() const;
        inline const MetaContainer& Meta() const;

        template<typename ClassType>
        inline void Set(ClassType* obj, const Variant& value) const;

        inline bool IsConst();
        inline bool IsPointer();

    protected:
        friend PropertyData& _InternalPropertyGetPropertyDataRef(Property& prop);
        PropertyData _PropData;
    };


    class PropertyContainer : public DefaultContainer<Property>
    {
    public:
        inline bool Has(const stdrttr::string& key) const;
        inline const Property& Get(const stdrttr::string& key) const;
    };
}
