namespace SimpleRTTR
{
    Parameter::Parameter(const class Type& type)
        :
        _Type(type)
    {

    }

    Parameter::Parameter(Parameter&& param)
        :
        _Type(param._Type)
    {

    }

    Parameter::Parameter(const Parameter& param)
        :
        _Type(param._Type)
    {

    }

    Parameter& Parameter::operator=(const Parameter& param)
    {
//        _Type = param._Type;
        return *this;
    }


    const stdrttr::string& Parameter::Name() const
    {
        return _Name;
    }

    const Type& Parameter::Type() const
    {
        return _Type;
    }

    Method::Method(const stdrttr::string& name, const Type& retType, const Method::ParamList& params)
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

    const Type& Method::ReturnType() const
    {
        return _RetType;
    }

    const Method::ParamList& Method::Parameters() const
    {
        return _Params;
    }

}