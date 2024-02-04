#pragma once

namespace SimpleRTTR
{
    class Type;

    class Parameter
    {
    public:
        inline Parameter(const stdrttr::string& name, const TypeReference& type);
        inline Parameter(Parameter&& param);
        inline Parameter(const Parameter& param);
        inline Parameter& operator=(const Parameter& param);

        inline const stdrttr::string& Name() const;
        inline const class Type Type() const;

    protected:
        stdrttr::string _Name;
        TypeReference _Type;
    };

    class Method
    {
    public:
        static inline const Method& InvalidMethod();

        using ParamList = stdrttr::vector<Parameter>;

        inline Method(const stdrttr::string& name, const TypeReference& retType, const ParamList& params);
        inline Method(Method&& method);
        inline Method(const Method& method);
        inline Method& operator=(const Method& method);

        inline bool operator==(const Method& method) const;
        inline bool operator!=(const Method& method) const;

        inline bool Equals(const Method& method) const;

        inline const stdrttr::string& Name() const;
        inline const Type ReturnType() const;
        inline const ParamList& Parameters() const;

        inline const MetaContainer& Meta() const;


    protected:

        friend MetaContainer& _InternalGetMetadata(Method& method);

        stdrttr::string _Name;
        TypeReference _RetType;
        ParamList   _Params;
        MetaContainer _Meta;
    };


    class MethodContainer : public DefaultContainer<Method>
    {
    public:
        inline bool Has(const stdrttr::string& key) const;
        inline const Method& Get(const stdrttr::string& key) const;
    };
}
