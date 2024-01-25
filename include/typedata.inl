namespace SimpleRTTR
{
    //---
    // Type Data
    TypeData::TypeData(const stdrttr::string& name,
        const stdrttr::string& fullyQualifiedName,
        std::size_t size,
        bool registeredByUser,
        const NamespaceContainer& namespaces,
        const TemplateTypeContainer& templateParams,
        UnsafeCopyFunction unsafeCopyFunc,
        ToStringFunction toStringFunc)
        :
        _Name(name),
        _FullyQualifiedName(fullyQualifiedName),
        _Size(size),
        _RegisteredByUser(registeredByUser),
        _Namespaces(namespaces),
        _TemplateParams(templateParams),
        _UnsafeCopyFunc(unsafeCopyFunc),
        _ToStringFunc(toStringFunc)
    {

    }

    TypeData::TypeData(const stdrttr::string& name,
        const stdrttr::string& fullyQualifiedName,
        std::size_t size)
        :
        _Name(name),
        _FullyQualifiedName(fullyQualifiedName),
        _Size(size),
        _RegisteredByUser(false)
    {
        _UnsafeCopyFunc = nullptr;
    }



    TypeData::TypeData(const TypeData& typeData)
        :
        _Name(typeData._Name),
        _FullyQualifiedName(typeData._FullyQualifiedName),
        _Size(typeData._Size),
        _RegisteredByUser(typeData._RegisteredByUser),
        _Properties(typeData._Properties),
        _Methods(typeData._Methods),
        _Namespaces(typeData._Namespaces),
        _TemplateParams(typeData._TemplateParams),
        _Metadata(typeData._Metadata),
        _UnsafeCopyFunc(typeData._UnsafeCopyFunc),
        _ToStringFunc(typeData._ToStringFunc)
    {
    }

    TypeData::TypeData(TypeData&& typeData)
        :
        _Name(std::move(typeData._Name)),
        _FullyQualifiedName(std::move(typeData._FullyQualifiedName)),
        _Size(typeData._Size),
        _RegisteredByUser(typeData._RegisteredByUser),
        _Properties(std::move(typeData._Properties)),
        _Methods(std::move(typeData._Methods)),
        _Namespaces(std::move(typeData._Namespaces)),
        _TemplateParams(std::move(typeData._TemplateParams)),
        _Metadata(std::move(typeData._Metadata)),
        _UnsafeCopyFunc(std::move(typeData._UnsafeCopyFunc)),
        _ToStringFunc(std::move(typeData._ToStringFunc))
    {
    }

    TypeData& TypeData::operator=(const TypeData& typeData)
    {
        _Name = typeData._Name;
        _FullyQualifiedName = typeData._FullyQualifiedName;
        _Size = typeData._Size;
        _RegisteredByUser = typeData._RegisteredByUser;
        _Properties = typeData._Properties;
        _Methods = typeData._Methods;
        _Namespaces = typeData._Namespaces;
        _TemplateParams = typeData._TemplateParams;
        _Metadata = typeData._Metadata;
        _UnsafeCopyFunc = typeData._UnsafeCopyFunc;
        _ToStringFunc = typeData._ToStringFunc;
        return *this;
    }

    bool TypeData::Equals(const TypeData& typeData) const
    {
        return (this == &typeData) || (
            _Size == typeData._Size &&
            _FullyQualifiedName.compare(typeData._FullyQualifiedName) == 0);
    }

    const stdrttr::string& TypeData::GetName() const
    {
        return _Name;
    }

    const stdrttr::string& TypeData::GetFullyQualifiedName() const
    {
        return _FullyQualifiedName;
    }

    std::size_t TypeData::GetSize() const
    {
        return _Size;
    }

    bool TypeData::IsRegisteredByUser() const
    {
        return _RegisteredByUser;
    }

    const PropertyContainer& TypeData::GetPropertyList() const
    {
        return _Properties;
    }



    const TypeData::MethodContainer& TypeData::GetMethodList() const
    {
        return _Methods;
    }

    const TypeData::NamespaceContainer& TypeData::GetNamespaces() const
    {
        return _Namespaces;
    }

    const TypeData::TemplateTypeContainer& TypeData::GetTemplateParams() const
    {
        return _TemplateParams;
    }

    const TypeData::ValuesContainer& TypeData::GetValues() const
    {
        return _Values;
    }

    const MetaContainer& TypeData::GetMetadata() const
    {
        return _Metadata;
    }

    inline const TypeData::UnsafeCopyFunction TypeData::GetUnsafeCopyFunction() const
    {
        return _UnsafeCopyFunc;
    }

    const TypeData::ToStringFunction TypeData::GetToStringFunction() const
    {
        return _ToStringFunc;
    }

    //const Property& TypeData::GetOrCreateProperty(const stdrttr::string& name, const TypeReference& type, std::size_t offset)
    //{
    //    PropertyData propData(name, type, offset);

    //    class Property prop(propData);

    //    //check to see if property exists
    //    PropertyContainer::iterator iter = std::find_if(_Properties.begin(), _Properties.end(), [&prop](const class Property& existing) { return existing == prop; });
    //    if (iter == _Properties.end())
    //    {
    //        _Properties.PushBack(prop);
    //        return _Properties.Back();
    //    }

    //    return (*iter);
    //}

    Method& TypeData::GetOrCreateMethod(Method& method)
    {
        //TODO: Check if the method exists already
        _Methods.push_back(method);
        return _Methods.back();
    }

    Meta& TypeData::AddMetadata(const Meta& meta)
    {
        //TODO: Check if the meta exists already
        _Metadata.PushBack(meta);
        return _Metadata.Back();
    }

    class Value& TypeData::AddValue(const class Value& value)
    {
        _Values.push_back(value);
        return _Values.back();
    }

    inline PropertyContainer& _InternalGetProperties(TypeData& typeData)
    {
        return typeData._Properties;
    }
}