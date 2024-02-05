namespace SimpleRTTR
{
    Parameter::Parameter(const stdrttr::string& name, const TypeReference& type)
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


    const stdrttr::string& Parameter::Name() const
    {
        return _Name;
    }

    const Type Parameter::Type() const
    {
        return _Type.Type();
    }

    std::size_t Parameter::Hash() const
    {
        std::size_t seed = 0;
        HashCombine(seed, _Name, _Type);
        return seed;
    }


    const Method& Method::InvalidMethod()
    {
        static Method InvalidMethod("invalid", Type::InvalidType(), {});
        return InvalidMethod;
    }


    Method::Method(const stdrttr::string& name, const TypeReference& retType, const ParameterContainer& params)
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
        return Equals(method);
    }

    bool Method::operator!=(const Method& method) const
    {
        return !Equals(method);
    }

    bool Method::Equals(const Method& method) const
    {
        //TODO: probably need to compare parameters, meta, and return type as well
        return _Name == method._Name;
    }

    std::size_t Method::Hash() const
    {
        std::size_t seed = 0;
        HashCombine(seed, _Name, _RetType, _Params, _Meta);
        return seed;
    }


    const stdrttr::string& Method::Name() const
    {
        return _Name;
    }

    const Type Method::ReturnType() const
    {
        return _RetType.Type();
    }

    const ParameterContainer& Method::Parameters() const
    {
        return _Params;
    }

    const MetaContainer& Method::Meta() const
    {
        return _Meta;
    }

    bool MethodContainer::Has(const stdrttr::string& name) const
    {
        for (const Method& method : _Data)
        {
            if (name == method.Name())
            {
                return true;
            }
        }
        return false;
    }

    const Method& MethodContainer::Get(const stdrttr::string& name) const
    {
        for (const Method& method : _Data)
        {
            if (name == method.Name())
            {
                return method;
            }
        }
        return Method::InvalidMethod();
    }

    inline MetaContainer& _InternalGetMetadata(Method& method)
    {
        return method._Meta;
    }
}