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
        _Values(typeData._Values),
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
        _Values(std::move(typeData._Values)),
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
        _Values = typeData._Values;
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

    inline std::size_t TypeData::Hash() const
    {
        std::size_t hash = 0;
        HashCombine(hash,
            _FullyQualifiedName,
            _Size,
            _Properties,
            _Methods,
            _Namespaces,
            _TemplateParams,
            _Values,
            _Metadata);
        return hash;
    }


    bool TypeData::IsRegisteredByUser() const
    {
        return _RegisteredByUser;
    }

    const PropertyContainer& TypeData::GetProperties() const
    {
        return _Properties;
    }



    const MethodContainer& TypeData::GetMethods() const
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

    const ValueContainer& TypeData::GetValues() const
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


    inline MetaContainer& _InternalGetMetadata(TypeData& typeData)
    {
        return typeData._Metadata;
    }

    inline ValueContainer& _InternalGetValues(TypeData& typeData)
    {
        return typeData._Values;
    }

    inline PropertyContainer& _InternalGetProperties(TypeData& typeData)
    {
        return typeData._Properties;
    }

    inline MethodContainer& _InternalGetMethods(TypeData& typeData)
    {
        return typeData._Methods;
    }
}