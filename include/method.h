#pragma once

namespace SimpleRTTR
{
    class Type;

    class Parameter
    {
    public:
        inline Parameter(const std::string& name, const TypeReference& type);
        inline Parameter(Parameter&& param);
        inline Parameter(const Parameter& param);
        inline Parameter& operator=(const Parameter& param);

        inline const std::string& name() const;
        inline Type type() const;

        inline std::size_t hash() const;

    protected:
        std::string _Name;
        TypeReference _Type;
    };

    class ParameterContainer : public DefaultContainer<Parameter>
    {
    };

    class Method
    {
    public:
        inline Method(const std::string& name, const TypeReference& retType, const ParameterContainer& params);
        inline Method(Method&& method);
        inline Method(const Method& method);
        inline Method& operator=(const Method& method);

        inline bool operator==(const Method& method) const;
        inline bool operator!=(const Method& method) const;

        inline bool equals(const Method& method) const;

        inline std::size_t hash() const;

        inline const std::string& name() const;
        inline const Type return_type() const;
        inline const ParameterContainer& parameters() const;

        inline const MetaContainer& meta() const;

    protected:

        friend MetaContainer& _InternalGetMetadata(Method& method);

        std::string _Name;
        TypeReference _RetType;
        ParameterContainer   _Params;
        MetaContainer _Meta;
    };


    class MethodContainer : public DefaultContainer<Method>
    {
    public:
        inline bool has(const std::string& key) const;
        inline const std::optional<std::reference_wrapper<const Method>> get(const std::string& key) const;
    };

    class ConstructorContainer : public DefaultContainer<Method>
    {

    };
}
