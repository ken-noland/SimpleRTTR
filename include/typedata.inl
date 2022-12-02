namespace SimpleRTTR
{
    //---
    // Type Data
    TypeData::TypeData()
        :
        Size(-1)
    {

    }

    TypeData::TypeData(const TypeData& typeData)
        :
        Name(typeData.Name),
        FullyQualifiedName(typeData.FullyQualifiedName),
        Size(typeData.Size),
        Properties(typeData.Properties),
        Methods(typeData.Methods),
        Namespaces(typeData.Namespaces),
        TemplateParams(typeData.TemplateParams)
    {
    }

    TypeData::TypeData(TypeData&& typeData)
        :
        Name(std::move(typeData.Name)),
        FullyQualifiedName(std::move(typeData.FullyQualifiedName)),
        Size(typeData.Size),
        Properties(std::move(typeData.Properties)),
        Methods(std::move(typeData.Methods)),
        Namespaces(std::move(typeData.Namespaces)),
        TemplateParams(std::move(typeData.TemplateParams))
    {
        typeData.Size = -1;
    }

    TypeData& TypeData::operator=(const TypeData& typeData)
    {
        Name = typeData.Name;
        FullyQualifiedName = typeData.FullyQualifiedName;
        Size = typeData.Size;
        Properties = typeData.Properties;
        Methods = typeData.Methods;
        Namespaces = typeData.Namespaces;
        TemplateParams = typeData.TemplateParams;
        return *this;
    }

    bool TypeData::Equals(const TypeData& typeData) const
    {
        return (this == &typeData) || (
            Size == typeData.Size &&
            FullyQualifiedName.compare(typeData.FullyQualifiedName) == 0);
    }
}