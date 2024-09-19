namespace SimpleRTTR
{
    //---
    // Type Data
    TypeData::TypeData(const stdrttr::string& name,
        const stdrttr::string& fullyQualifiedName,
        std::size_t size,
        std::uint64_t flags,
        std::type_index typeIndex,
        bool registeredByUser,
        const NamespaceContainer& namespaces,
        const TemplateTypeContainer& templateParams,
        ToStringFunction toStringFunc)
        :
        _Name(name),
        _FullyQualifiedName(fullyQualifiedName),
        _Size(size),
        _Flags(flags),
        _TypeIndex(typeIndex),
        _RegisteredByUser(registeredByUser),
        _Namespaces(namespaces),
        _TemplateParams(templateParams),
        _ToStringFunc(toStringFunc)
    {

    }

    TypeData::TypeData(const stdrttr::string& name,
        const stdrttr::string& fqn,
        std::size_t size, 
        std::uint64_t flags, 
        std::type_index typeIndex)
        :
        _Name(name),
        _FullyQualifiedName(fqn),
        _Size(size),
        _Flags(flags),
        _TypeIndex(typeIndex),
        _RegisteredByUser(false)
    {
    }



    TypeData::TypeData(const TypeData& typeData)
        :
        _Name(typeData._Name),
        _FullyQualifiedName(typeData._FullyQualifiedName),
        _Size(typeData._Size),
        _TypeIndex(typeData._TypeIndex),
        _Flags(typeData._Flags),
        _RegisteredByUser(typeData._RegisteredByUser),
        _Properties(typeData._Properties),
        _Methods(typeData._Methods),
        _Namespaces(typeData._Namespaces),
        _TemplateParams(typeData._TemplateParams),
        _Values(typeData._Values),
        _Metadata(typeData._Metadata),
        _ToStringFunc(typeData._ToStringFunc)
    {
    }

    TypeData::TypeData(TypeData&& typeData)
        :
        _Name(std::move(typeData._Name)),
        _FullyQualifiedName(std::move(typeData._FullyQualifiedName)),
        _Size(typeData._Size),
        _TypeIndex(typeData._TypeIndex),
        _Flags(typeData._Flags),
        _RegisteredByUser(typeData._RegisteredByUser),
        _Properties(std::move(typeData._Properties)),
        _Methods(std::move(typeData._Methods)),
        _Namespaces(std::move(typeData._Namespaces)),
        _TemplateParams(std::move(typeData._TemplateParams)),
        _Values(std::move(typeData._Values)),
        _Metadata(std::move(typeData._Metadata)),
        _ToStringFunc(std::move(typeData._ToStringFunc))
    {
    }

    TypeData& TypeData::operator=(const TypeData& typeData)
    {
        _Name = typeData._Name;
        _FullyQualifiedName = typeData._FullyQualifiedName;
        _Size = typeData._Size;
        _TypeIndex = typeData._TypeIndex;
        _Flags = typeData._Flags;
        _RegisteredByUser = typeData._RegisteredByUser;
        _Properties = typeData._Properties;
        _Methods = typeData._Methods;
        _Namespaces = typeData._Namespaces;
        _TemplateParams = typeData._TemplateParams;
        _Values = typeData._Values;
        _Metadata = typeData._Metadata;
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

    bool TypeData::HasFlag(TypeFlag flag) const
    {
        return _Flags & static_cast<std::uint32_t>(flag);
    }

    const std::type_index& TypeData::GetTypeIndex() const
    {
        return _TypeIndex;
    }

    std::size_t TypeData::Hash() const
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

    const ConstructorContainer& TypeData::GetConstructors() const
    {
        return _Constructors;
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

    const TypeData::ToStringFunction TypeData::GetToStringFunction() const
    {
        return _ToStringFunc;
    }

    inline ConstructorContainer& _InternalGetConstructors(TypeData& typeData)
    {
        return typeData._Constructors;
    }

    inline PropertyContainer& _InternalGetProperties(TypeData& typeData)
    {
        return typeData._Properties;
    }

    inline MethodContainer& _InternalGetMethods(TypeData& typeData)
    {
        return typeData._Methods;
    }

    inline MetaContainer& _InternalGetMetadata(TypeData& typeData)
    {
        return typeData._Metadata;
    }

    inline ValueContainer& _InternalGetValues(TypeData& typeData)
    {
        return typeData._Values;
    }

}