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

    Method::Method(const stdrttr::string& name, const TypeReference& retType, const Method::ParamList& params)
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
        _Params(std::move(method._Params))
    {

    }

    Method::Method(const Method& method)
        :
        _Name(std::move(method._Name)),
        _RetType(method._RetType),
        _Params(std::move(method._Params))
    {

    }

    Method& Method::operator=(const Method& method)
    {
        return *this;
    }

    const stdrttr::string& Method::Name() const
    {
        return _Name;
    }

    const Type Method::ReturnType() const
    {
        return _RetType.Type();
    }

    const Method::ParamList& Method::Parameters() const
    {
        return _Params;
    }

}