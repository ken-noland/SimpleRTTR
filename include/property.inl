namespace SimpleRTTR
{
    inline PropertyData& _InternalPropertyGetPropertyDataRef(Property& prop);

    PropertyData::PropertyData(const std::string& name, const TypeReference& type, std::size_t offset)
        :
        _Name(name),
        _Type(type),
        _Offset(offset)
    {
    }

    PropertyData::PropertyData(PropertyData&& data)
        :
        _Name(std::move(data._Name)),
        _Type(data._Type),
        _Offset(data._Offset),
        _Flags(data._Flags),
        _Meta(std::move(data._Meta))
    {
    }

    PropertyData::PropertyData(const PropertyData& data)
        :
        _Name(data._Name),
        _Type(data._Type),
        _Offset(data._Offset),
        _Flags(data._Flags),
        _Meta(data._Meta)
    {

    }

    bool PropertyData::operator==(const PropertyData& data) const
    {
        return equals(data);
    }

    bool PropertyData::equals(const PropertyData& data) const
    {
        //TODO: add type
        return (_Name.compare(data._Name) == 0) && _Offset == data._Offset;
    }

    const std::string& PropertyData::name() const
    {
        return _Name;
    }

    SimpleRTTR::Type PropertyData::type() const
    {
        return _Type.type();
    }

    const std::size_t PropertyData::offset() const
    {
        return _Offset;
    }

    const MetaContainer& PropertyData::meta() const
    {
        return _Meta;
    }

    std::size_t PropertyData::hash() const
    {
        std::size_t seed = 0;
        SimpleRTTR::hash_combine(seed, _Name, _Type, _Offset, _Flags);
        return seed;
    }

    template<typename ClassType>
    inline void PropertyData::set(ClassType* obj, const Variant& value) const
    {
        void* ptr = reinterpret_cast<void*>(reinterpret_cast<std::size_t>(obj) + _Offset);
//        Type().Set(ptr, value);

    }

    inline MetaContainer& _InternalPropertyDataGetMetaListRef(PropertyData& prop)
    {
        return prop._Meta;
    }

    Property::Property(Property&& prop) noexcept
        :
        _PropData(prop._PropData)
    {

    }

    const Property& Property::invalid_property()
    {
        static Property invalid_property(PropertyData("invalid", Type::invalid_type(), (std::size_t) - 1));
        return invalid_property;
    }


    Property::Property(const Property& prop)
        :
        _PropData(prop._PropData)
    {

    }


    Property::Property(const PropertyData& propData)
        :
        _PropData(propData)
    {

    }

    Property& Property::operator=(const Property& prop)
    {
        std::memcpy(&_PropData, &prop._PropData, sizeof(PropertyData));
        return *this;
    }

    bool Property::operator==(const Property& data) const
    {
        return equals(data);
    }

    bool Property::operator!=(const Property& data) const
    {
        return !equals(data);
    }

    bool Property::equals(const Property& data) const
    {
        return _PropData.equals(data._PropData);
    }

    const std::string& Property::name() const
    {
        return _PropData.name();
    }

    const std::size_t Property::offset() const
    {
        return _PropData.offset();
    }

    const SimpleRTTR::Type Property::type() const
    {
        return _PropData.type();
    }

    const MetaContainer& Property::meta() const
    {
        return _PropData.meta();
    }

    template<typename ClassType>
    void Property::set(ClassType* obj, const Variant& value) const
    {
        _PropData.set(obj, value);
    }

    Variant Property::get(void* obj) const
	{
		void* ptr = reinterpret_cast<void*>(reinterpret_cast<std::size_t>(obj) + offset());
		return Variant(ptr, type());
	}

	template <typename ClassType>
	ClassType Property::get(void* obj) const
	{
		void* ptr = reinterpret_cast<void*>(reinterpret_cast<std::size_t>(obj) + offset());
		return *reinterpret_cast<ClassType*>(ptr);
	}

    inline std::size_t Property::hash() const
    {
        return _PropData.hash();
    }


    inline PropertyData& _InternalPropertyGetPropertyDataRef(Property& prop)
    {
        return prop._PropData;
    }

    bool PropertyContainer::has(const std::string& key) const
    {
        ConstIterator found = std::find_if(begin(), end(), [&key](const Property& prop) { return prop.name().compare(key) == 0; });
        return found != end();
    }

    const Property& PropertyContainer::get(const std::string& key) const
    {
        ConstIterator found = std::find_if(begin(), end(), [&key](const Property& prop) { return prop.name().compare(key) == 0; });
        if (found != end())
        {
            return *found;
        }
        else
        {
            SIMPLERTTR_ASSERT_MSG(false, "Property not found");
        }
    }
}
