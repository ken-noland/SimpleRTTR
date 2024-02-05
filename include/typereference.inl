
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

    inline std::size_t TypeReference::Hash() const
    {
        //TODO: the type reference should use the hash value from the type to begin with(sd in, we should remove the _FullyQualifiedName member and replace it with an ID), 
        // but at the moment I don't have time to move this around and the performance impact is minimal.
        return Type().Hash();
    }

}