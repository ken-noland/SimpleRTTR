namespace SimpleRTTR
{
    //---
    // Type Data
    TypeData::TypeData(const std::string& name,
        const std::string& fullyQualifiedName,
        std::size_t size,
        std::uint64_t flags,
        std::type_index typeIndex,
        bool registeredByUser,
        const NamespaceContainer& namespaces,
        const TemplateTypeContainer& templateParams,
        TypeFunctions typeFunctions,
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
        _TypeFunctions(typeFunctions),
        _ToStringFunc(toStringFunc)
    {

    }

    TypeData::TypeData(const std::string& name,
        const std::string& fqn,
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

    bool TypeData::equals(const TypeData& typeData) const
    {
        return (this == &typeData) || (
            _Size == typeData._Size &&
            _FullyQualifiedName.compare(typeData._FullyQualifiedName) == 0);
    }

    const std::string& TypeData::name() const
    {
        return _Name;
    }

    const std::string& TypeData::fully_qualified_name() const
    {
        return _FullyQualifiedName;
    }

    std::size_t TypeData::size() const
    {
        return _Size;
    }

    std::size_t TypeData::hash() const
    {
        std::size_t hash = 0;
        hash_combine(hash,
            _FullyQualifiedName,
            _Size,
            _Flags,
            _Properties,
            _Methods,
            _Namespaces,
            _TemplateParams,
            _Values,
            _Metadata);
        return hash;
    }

    bool TypeData::has_flag(TypeFlag flag) const
    {
        return _Flags & static_cast<std::uint64_t>(flag);
    }

    const std::type_index& TypeData::type_index() const
    {
        return _TypeIndex;
    }


    bool TypeData::is_registered_by_user() const
    {
        return _RegisteredByUser;
    }

    const ConstructorContainer& TypeData::constructors() const
    {
        return _Constructors;
    }

    const PropertyContainer& TypeData::properties() const
    {
        return _Properties;
    }

    const MethodContainer& TypeData::methods() const
    {
        return _Methods;
    }

    const TypeData::NamespaceContainer& TypeData::namespaces() const
    {
        return _Namespaces;
    }

    const TypeData::TemplateTypeContainer& TypeData::template_params() const
    {
        return _TemplateParams;
    }

    const ValueContainer& TypeData::values() const
    {
        return _Values;
    }

    const MetaContainer& TypeData::meta() const
    {
        return _Metadata;
    }

    const TypeData::ToStringFunction TypeData::to_string_function() const
    {
        return _ToStringFunc;
    }

    const TypeFunctions& TypeData::type_functions() const
    {
        return _TypeFunctions;
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