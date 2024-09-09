#pragma once

namespace SimpleRTTR
{
    class TypeReference
    {
    public:
        inline TypeReference(const class Type&);
        inline TypeReference(const TypeReference&);

        inline bool operator==(const Type& type) const;
        inline bool operator==(const TypeReference& type) const;

        inline class Type Type() const;

        inline const stdrttr::string& GetFullyQualifiedName() const;
        inline std::size_t Hash() const;

    protected:
        //TODO: Instead of a string, use a hash of the whole data type
        stdrttr::string _FullyQualifiedName;
    };
}