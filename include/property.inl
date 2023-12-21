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

    inline MetaContainer& _InternalPropertyDataGetMetaListRef(PropertyData& prop)
    {
        return prop._Meta;
    }

    Property::Property(Property&& prop)
        :
        _PropData(prop._PropData)
    {

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

    void Property::ForEach(Property::MetaFunction eval) const
    {
        std::for_each(Meta().Begin(), Meta().End(), eval);
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

    PropertyContainer::Iterator PropertyContainer::Begin()
    {
        return _Properties.begin();
    }

    PropertyContainer::ConstIterator PropertyContainer::Begin() const
    {
        return _Properties.begin();
    }

    PropertyContainer::Iterator PropertyContainer::End()
    {
        return _Properties.end();
    }

    PropertyContainer::ConstIterator PropertyContainer::End() const
    {
        return _Properties.end();
    }

    void PropertyContainer::PushBack(const Property& prop)
    {
        _Properties.push_back(prop);
    }

    void PropertyContainer::PushBack(Property&& prop)
    {
        _Properties.push_back(std::move(prop));
    }

    std::size_t PropertyContainer::Size() const
    {
        return _Properties.size();
    }

    Property& PropertyContainer::Back()
    {
        return _Properties.back();
    }
    
    const Property& PropertyContainer::Back() const
    {
       return _Properties.back();
    }

    Property& PropertyContainer::operator[](std::size_t index)
    {
        return _Properties[index];
    }

    const Property& PropertyContainer::operator[](std::size_t index) const
    {
        return _Properties[index];
    }
}