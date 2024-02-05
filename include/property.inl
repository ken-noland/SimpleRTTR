namespace SimpleRTTR
{
    inline PropertyData& _InternalPropertyGetPropertyDataRef(Property& prop);

    PropertyData::PropertyData(const stdrttr::string& name, const TypeReference& type, std::size_t offset)
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
        return Equals(data);
    }

    bool PropertyData::Equals(const PropertyData& data) const
    {
        //TODO: add type
        return (_Name.compare(data._Name) == 0) && _Offset == data._Offset;
    }

    const stdrttr::string& PropertyData::Name() const
    {
        return _Name;
    }

    Type PropertyData::Type() const
    {
        return _Type.Type();
    }

    const std::size_t PropertyData::Offset() const
    {
        return _Offset;
    }

    const MetaContainer& PropertyData::Meta() const
    {
        return _Meta;
    }

    template<typename ClassType>
    inline void PropertyData::Set(ClassType* obj, const Variant& value) const
    {
        void* ptr = reinterpret_cast<void*>(reinterpret_cast<std::size_t>(obj) + _Offset);

        TypeData::UnsafeCopyFunction unsafeCopyFunc = _InternalGetTypeData(Type()).GetUnsafeCopyFunction();
        if(unsafeCopyFunc)
        {
            unsafeCopyFunc(value, ptr, _Type);
        }
        else
        {
            SIMPLERTTR_ASSERT(!"No copy function available");
        }
    }

    inline MetaContainer& _InternalPropertyDataGetMetaListRef(PropertyData& prop)
    {
        return prop._Meta;
    }

    Property::Property(Property&& prop)
        :
        _PropData(prop._PropData)
    {

    }


    const Property& Property::InvalidProperty()
    {
        static Property InvalidProperty(PropertyData("invalid", Type::InvalidType(), -1));
        return InvalidProperty;
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
        return Equals(data);
    }

    bool Property::operator!=(const Property& data) const
    {
        return !Equals(data);
    }

    bool Property::Equals(const Property& data) const
    {
        return _PropData.Equals(data._PropData);
    }

    const stdrttr::string& Property::Name() const
    {
        return _PropData.Name();
    }

    const std::size_t Property::Offset() const
    {
        return _PropData.Offset();
    }

    const class Type Property::Type() const
    {
        return _PropData.Type();
    }

    const MetaContainer& Property::Meta() const
    {
        return _PropData.Meta();
    }

    template<typename ClassType>
    inline void Property::Set(ClassType* obj, const Variant& value) const
    {
        _PropData.Set(obj, value);
    }

    bool Property::IsConst()
    {
        //TODO
        return false;
    }

    bool Property::IsPointer()
    {
        //TODO
        return false;
    }

    inline PropertyData& _InternalPropertyGetPropertyDataRef(Property& prop)
    {
        return prop._PropData;
    }

    bool PropertyContainer::Has(const stdrttr::string& key) const
    {
        ConstIterator found = std::find_if(Begin(), End(), [&key](const Property& prop) { return prop.Name().compare(key) == 0; });
        return found != End();
    }

    const Property& PropertyContainer::Get(const stdrttr::string& key) const
    {
        ConstIterator found = std::find_if(Begin(), End(), [&key](const Property& prop) { return prop.Name().compare(key) == 0; });
        if (found != End())
        {
            return *found;
        }
        else
        {
            throw std::runtime_error("Property not found");
        }
    }
}