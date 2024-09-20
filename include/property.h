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

        inline bool equals(const PropertyData& data) const;

        inline PropertyData(const std::string& name, const TypeReference& type, std::size_t offset);
        inline PropertyData(PropertyData&& data);
        inline PropertyData(const PropertyData& data);

        inline const std::string& name() const;
        inline SimpleRTTR::Type type() const;
        inline const std::size_t offset() const;
        inline const MetaContainer& meta() const;

        inline std::size_t hash() const;

        template<typename ClassType>
        inline void set(ClassType* obj, const Variant& value) const;


    protected:
        std::string _Name;
        TypeReference _Type;
        std::size_t _Offset;
        PropertyFlagsBits _Flags;


        friend MetaContainer& _InternalPropertyDataGetMetaListRef(PropertyData& prop);
        MetaContainer _Meta;
    };

    class Property
    {
    public:
        static inline const Property& invalid_property();

        inline Property(Property&& data)  noexcept;
        inline Property(const Property& data);
        inline Property(const PropertyData& data);
        inline Property& operator=(const Property& prop);

        inline bool operator==(const Property& data) const;
        inline bool operator!=(const Property& data) const;

        inline bool equals(const Property& data) const;

        inline const std::string& name() const;
        inline const SimpleRTTR::Type type() const;
        inline const std::size_t offset() const;

        inline const SimpleRTTR::MetaContainer& meta() const;

        // Set a value given an instance of the class
        template<typename ClassType>
        inline void set(ClassType* obj, const Variant& value) const;

        // Get a value given an instance of the class
        inline Variant get(void* obj) const;

        template <typename ClassType>
		inline ClassType get(void* obj) const;

        inline std::size_t hash() const;

    protected:
        friend PropertyData& _InternalPropertyGetPropertyDataRef(Property& prop);
        PropertyData _PropData;
    };


    class PropertyContainer : public DefaultContainer<Property>
    {
    public:
        inline bool has(const std::string& key) const;
        inline const Property& get(const std::string& key) const;
    };
}
