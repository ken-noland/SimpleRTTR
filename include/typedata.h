#pragma once

namespace SimpleRTTR
{
    class TypeData
    {
    public:
        using PropertyList = stdrttr::vector<Property>;
        using MethodList = stdrttr::vector<Method>;
        using MetaList = stdrttr::vector<Meta>;
        using NamespaceList = stdrttr::vector<stdrttr::string>;
        using TemplateTypeList = stdrttr::vector<TypeReference>;
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

        inline const MetaList& GetMetadata() const;

        inline const ToStringFunction GetToStringFunction() const;

        inline Property& GetOrCreateProperty(const stdrttr::string& name, const TypeReference& type, std::size_t offset);
        inline Method& GetOrCreateMethod(Method& method);
        inline Meta& GetOrCreateMetadata(Meta& meta);


    protected:
        stdrttr::string _Name;
        stdrttr::string _FullyQualifiedName;
        std::size_t _Size;
        bool _RegisteredByUser;

        PropertyList _Properties;
        MethodList _Methods;
        NamespaceList _Namespaces;
        TemplateTypeList _TemplateParams;

        MetaList _Metadata;

        ToStringFunction _ToStringFunc;

        friend class TypeStorage;
        inline TypeData(const stdrttr::string& name,
            const stdrttr::string& fullyQualifiedName,
            std::size_t size,
            bool registeredByUser,
            PropertyList properties,
            MethodList methods,
            MetaList metadata,
            NamespaceList namespaces,
            TemplateTypeList templateParams,
            ToStringFunction toStringFunc);

        inline TypeData(const stdrttr::string& name,
            const stdrttr::string& fqn,
            std::size_t size);
    };
}
