
namespace SimpleRTTR
{

    //--
    //Type Reference
    TypeReference::TypeReference(const class Type& type)
        :
        _TypeIndex(type.type_index())
    {

    }

    TypeReference::TypeReference(const std::type_index& typeIndex)
        :
        _TypeIndex(typeIndex)
    {
    }

    TypeReference::TypeReference(const TypeReference& ref)
        :
        _TypeIndex(ref.type_index())
    {

    }

    inline bool TypeReference::operator==(const class Type& type) const
    {
        // TODO: maybe check the Type equality rather than the type index
        return _TypeIndex == type.type_index();
    }

    inline bool TypeReference::operator==(const TypeReference& type) const
    {
        // TODO: maybe check the Type equality rather than the type index
        return _TypeIndex == type.type_index();
    }

    class Type TypeReference::type() const
    {
        return SimpleRTTR::types().get_type(_TypeIndex);
    }

    const std::type_index& TypeReference::type_index() const
    {
        return _TypeIndex;
    }

    inline const std::string& TypeReference::fully_qualified_name() const
    {
        return type().fully_qualified_name();
    }

    inline std::size_t TypeReference::hash() const
    {
        return type().hash();
    }

}