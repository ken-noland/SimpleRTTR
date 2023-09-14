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
        stdrttr::string _FullyQualifiedName;
    };
}