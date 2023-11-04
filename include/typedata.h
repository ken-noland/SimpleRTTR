#pragma once

namespace SimpleRTTR
{
    class TypeData
    {
    public:
        using PropertyList = stdrttr::vector<Property>;
        using MethodList = stdrttr::vector<Method>;
        using NamespaceList = stdrttr::vector<stdrttr::string>;
        using TemplateTypeList = stdrttr::vector<TypeReference>;
        using ValuesList = stdrttr::vector<Value>;
        using ToStringFunction = std::add_pointer<stdrttr::string(const Variant&)>::type;

        inline TypeData(const TypeData& typeData);
        inline TypeData(TypeData&& typeData);
        inline TypeData& operator=(const TypeData& typeData);

        inline bool Equals(const TypeData& typeData) const;

        inline const stdrttr::string& GetName() const;
        inline const stdrttr::string& GetFullyQualifiedName() const;
        inline std::size_t GetSize() const;
        inline bool IsRegisteredByUser() const;

        inline const PropertyList& GetPropertyList() const;
        inline const MethodList& GetMethodList() const;
        inline const NamespaceList& GetNamespaces() const;
        inline const TemplateTypeList& GetTemplateParams() const;

        inline bool HasValue(const stdrttr::string& name) const;
        inline const Variant& Value(const stdrttr::string& name) const;

        inline const MetaContainer& GetMetadata() const;

        inline const ToStringFunction GetToStringFunction() const;

        //TODO: I'm not to sure about leaving these as public. It feels a bit hackish, but then again, the 
        //  TypeData class is meant for internal use only... soooo.... ¯\_(?)_/¯
        inline Property& GetOrCreateProperty(const stdrttr::string& name, const TypeReference& type, std::size_t offset);
        inline Method& GetOrCreateMethod(Method& method);
        inline Meta& AddMetadata(const Meta& meta);
        inline class Value& AddValue(const class Value& value);


    protected:
        stdrttr::string _Name;
        stdrttr::string _FullyQualifiedName;
        std::size_t _Size;
        bool _RegisteredByUser;

        PropertyList _Properties;
        MethodList _Methods;
        NamespaceList _Namespaces;
        TemplateTypeList _TemplateParams;
        ValuesList _Values;

        MetaContainer _Metadata;

        ToStringFunction _ToStringFunc;

        friend class TypeStorage;
        inline TypeData(const stdrttr::string& name,
            const stdrttr::string& fullyQualifiedName,
            std::size_t size,
            bool registeredByUser,
            NamespaceList namespaces,
            TemplateTypeList templateParams,
            ToStringFunction toStringFunc);

        inline TypeData(const stdrttr::string& name,
            const stdrttr::string& fqn,
            std::size_t size);
    };
}
