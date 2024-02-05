#pragma once

namespace SimpleRTTR
{
    class TypeReference
    {
    public:
        inline TypeReference(const class Type&);
        inline TypeReference(const TypeReference&);

        inline class Type Type() const;

        inline const stdrttr::string& GetFullyQualifiedName() const;

    protected:
        //TODO: Instead of a string, use a hash of the whole data type
        stdrttr::string _FullyQualifiedName;
    };
}