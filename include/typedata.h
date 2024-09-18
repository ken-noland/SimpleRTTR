#pragma once

namespace SimpleRTTR
{
    class TypeData
    {
    public:
        using NamespaceContainer = stdrttr::vector<stdrttr::string>;
        using TemplateTypeContainer = stdrttr::vector<TypeReference>;

        using UnsafeCopyFunction = std::add_pointer<void(const Variant& src, void* dest, const TypeReference& destType)>::type;
        using ToAnyFunction = std::add_pointer<std::any(const void*)>::type;
        using ToStringFunction = std::add_pointer<stdrttr::string(const Variant&)>::type;

        inline TypeData(const TypeData& typeData);
        inline TypeData(TypeData&& typeData);
        inline TypeData& operator=(const TypeData& typeData);

        inline bool Equals(const TypeData& typeData) const;

        inline const stdrttr::string& GetName() const;
        inline const stdrttr::string& GetFullyQualifiedName() const;
        inline std::size_t GetSize() const;
        inline std::size_t Hash() const;

        inline bool IsEnum() const;

        //TODO: Remove this function. It's only used in the TypeStorage class.
        inline bool IsRegisteredByUser() const;

        inline const ConstructorContainer& GetConstructors() const;
        inline const PropertyContainer& GetProperties() const;
        inline const MethodContainer& GetMethods() const;
        inline const NamespaceContainer& GetNamespaces() const;
        inline const TemplateTypeContainer& GetTemplateParams() const;
        inline const ValueContainer& GetValues() const;

        inline const MetaContainer& GetMetadata() const;

        // Okay, this might be worth explaining a bit. The UnsafeCopyFunction is a function pointer that is used to copy the data from the variant type
        //  to the destination pointer. This is used for the Variant::CopyTo function. 
        inline const UnsafeCopyFunction GetUnsafeCopyFunction() const;
        inline const ToStringFunction GetToStringFunction() const;

    protected:
        stdrttr::string _Name;
        stdrttr::string _FullyQualifiedName;
        std::size_t _Size;
        bool _RegisteredByUser;

        // This is just temporary until we get proper flag support in for types, as enum can easily be a flag
        bool _IsEnum;

        // to be used to access non-const versions of the containers
        friend ConstructorContainer& _InternalGetConstructors(TypeData& typeData);
        friend PropertyContainer& _InternalGetProperties(TypeData& typeData);
        friend MethodContainer& _InternalGetMethods(TypeData& typeData);
        friend MetaContainer& _InternalGetMetadata(TypeData& typeData);
        friend ValueContainer& _InternalGetValues(TypeData& typeData);

        ConstructorContainer _Constructors;
        PropertyContainer _Properties;
        MethodContainer _Methods;
        NamespaceContainer _Namespaces;
        TemplateTypeContainer _TemplateParams;
        ValueContainer _Values;

        MetaContainer _Metadata;

        UnsafeCopyFunction _UnsafeCopyFunc;
        ToAnyFunction _ToAnyFunc;
        ToStringFunction _ToStringFunc;

        friend class Variant;   //needs access to _ToAnyFunc

        friend class TypeStorage;
        inline TypeData(const stdrttr::string& name,
            const stdrttr::string& fullyQualifiedName,
            std::size_t size,
            bool isEnum,
            bool registeredByUser,
            const NamespaceContainer& namespaces,
            const TemplateTypeContainer& templateParams,
            UnsafeCopyFunction unsafeCopyFunction,
            ToAnyFunction toAnyFunc,
            ToStringFunction toStringFunc);

        inline TypeData(const stdrttr::string& name,
            const stdrttr::string& fqn,
            std::size_t size);
    };
}
