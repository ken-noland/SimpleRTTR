namespace SimpleRTTR
{
    Parameter::Parameter(const std::string& name, const TypeReference& type)
        :
        _Name(name),
        _Type(type)
    {

    }

    Parameter::Parameter(Parameter&& param)
        :
        _Name(param._Name),
        _Type(param._Type)
    {

    }

    Parameter::Parameter(const Parameter& param)
        :
        _Name(param._Name),
        _Type(param._Type)
    {

    }

    Parameter& Parameter::operator=(const Parameter& param)
    {
        _Name = param._Name;
        _Type = param._Type;
        return *this;
    }


    const std::string& Parameter::name() const
    {
        return _Name;
    }

    Type Parameter::type() const
    {
        return _Type.type();
    }

    std::size_t Parameter::hash() const
    {
        std::size_t seed = 0;
        hash_combine(seed, _Name, _Type);
        return seed;
    }

    Method::Method(const std::string& name, const TypeReference& retType, const ParameterContainer& params)
        :
        _Name(name),
        _RetType(retType),
        _Params(std::move(params))
    {

    }

    Method::Method(Method&& method)
        :
        _Name(method._Name),
        _RetType(method._RetType),
        _Params(std::move(method._Params)),
        _Meta(std::move(method._Meta))
    {

    }

    Method::Method(const Method& method)
        :
        _Name(method._Name),
        _RetType(method._RetType),
        _Params(method._Params),
        _Meta(method._Meta)
    {

    }

    Method& Method::operator=(const Method& method)
    {
        _Name = method._Name;
        _RetType = method._RetType;
        _Params = method._Params;
        _Meta = method._Meta;
        return *this;
    }

    bool Method::operator==(const Method& method) const
    {
        return equals(method);
    }

    bool Method::operator!=(const Method& method) const
    {
        return !equals(method);
    }

    bool Method::equals(const Method& method) const
    {
        //TODO: probably need to compare parameters, meta, and return type as well
        return _Name == method._Name;
    }

    std::size_t Method::hash() const
    {
        std::size_t seed = 0;
        hash_combine(seed, _Name, _RetType, _Params, _Meta);
        return seed;
    }


    const std::string& Method::name() const
    {
        return _Name;
    }

    const Type Method::return_type() const
    {
        return _RetType.type();
    }

    const ParameterContainer& Method::parameters() const
    {
        return _Params;
    }

    const MetaContainer& Method::meta() const
    {
        return _Meta;
    }

    bool MethodContainer::has(const std::string& name) const
    {
        for (const Method& method : _Data)
        {
            if (name == method.name())
            {
                return true;
            }
        }
        return false;
    }

    const std::optional<std::reference_wrapper<const Method>> MethodContainer::get(const std::string& name) const
    {
        for (const Method& method : _Data)
        {
            if (name == method.name())
            {
                return method;
            }
        }
        return std::nullopt;
    }

    inline MetaContainer& _InternalGetMetadata(Method& method)
    {
        return method._Meta;
    }
}