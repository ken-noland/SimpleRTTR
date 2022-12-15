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
        using TemplateTypeRef = std::reference_wrapper<const Type>;
        using TemplateTypeList = stdrttr::vector<TemplateTypeRef>;
        using ToStringFunction = std::add_pointer<stdrttr::string(const Variant&)>::type;

        inline TypeData(const TypeData& typeData);
        inline TypeData(TypeData&& typeData);
        inline TypeData& operator=(const TypeData& typeData);

        inline bool Equals(const TypeData& typeData) const;

        stdrttr::string FullyQualifiedName;
        stdrttr::string Name;
        std::size_t Size;

        PropertyList Properties;
        MethodList Methods;

        MetaList Meta;

        NamespaceList Namespaces;
        TemplateTypeList TemplateParams;

        ToStringFunction ToString;

    protected:
        friend class TypeStorage;
        inline TypeData(); //do not allow creation of TypeData outside of the TypeStorage
    };
}
