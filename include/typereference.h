#pragma once

namespace SimpleRTTR
{
    class TypeReference
    {
    public:
        inline TypeReference(const class Type&);
        inline TypeReference(const std::type_index& typeIndex);
        inline TypeReference(const TypeReference&);

        inline bool operator==(const Type& type) const;
        inline bool operator==(const TypeReference& type) const;

        inline class Type type() const;
        inline const std::type_index& type_index() const;

        inline const std::string& fully_qualified_name() const;
        inline std::size_t hash() const;

    protected:
        std::type_index _TypeIndex;
    };
}