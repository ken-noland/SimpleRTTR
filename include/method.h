#pragma once

namespace SimpleRTTR
{
    class Type;

    class Parameter
    {
    public:
        inline Parameter(const class Type& type);
        inline Parameter(Parameter&& param);
        inline Parameter(const Parameter& param);
        inline Parameter& operator=(const Parameter& param);

        inline const stdrttr::string& Name() const;
        inline const class Type& Type() const;

    protected:
        stdrttr::string _Name;
        const class Type& _Type;
    };

    class Method
    {
    public:
        using ParamList = stdrttr::vector<Parameter>;

        inline Method(const stdrttr::string& name, const Type& retType, const ParamList& params);
        inline Method(Method&& method);
        inline Method(const Method& method);
        inline Method& operator=(const Method& method);

        inline const stdrttr::string& Name() const;
        inline const Type& ReturnType() const;
        inline const ParamList& Parameters() const;

    protected:
        stdrttr::string _Name;
        const Type& _RetType;
        ParamList   _Params;
    };

}
