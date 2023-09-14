
namespace SimpleRTTR
{

    //--
    //Type Reference
    TypeReference::TypeReference(const class Type& type)
        :
        _FullyQualifiedName(type.GetFullyQualifiedName())
    {

    }

    TypeReference::TypeReference(const TypeReference& ref)
        :
        _FullyQualifiedName(ref._FullyQualifiedName)
    {

    }

    class Type TypeReference::Type() const
    {
        return Types().GetType(GetFullyQualifiedName());
    }

    inline const stdrttr::string& TypeReference::GetFullyQualifiedName() const
    {
        return _FullyQualifiedName;
    }
}