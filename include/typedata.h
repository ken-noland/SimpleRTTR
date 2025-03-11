#pragma once

namespace SimpleRTTR
{
    enum class TypeFlag : uint64_t
    {
        None                        = 0,
        IsEmpty                     = 1ULL << 0,
        IsVoid                      = 1ULL << 1,
        IsNullPointer               = 1ULL << 2,
        IsIntegral                  = 1ULL << 3,
        IsFloatingPoint             = 1ULL << 4,
        IsArray                     = 1ULL << 5,

        IsEnum                      = 1ULL << 6,
        IsUnion                     = 1ULL << 7,
        IsClass                     = 1ULL << 8,
        IsFunction                  = 1ULL << 9,

        IsPointer                   = 1ULL << 10,
        IsReference                 = 1ULL << 11,

        IsMemberPointer             = 1ULL << 12,
        IsMemberObject              = 1ULL << 13,
        IsMemberFunction            = 1ULL << 14,

        IsTriviallyConstructible    = 1ULL << 15,
        IsTriviallyCopyable         = 1ULL << 16,
        IsTriviallyAssignable       = 1ULL << 17,

        IsNoThrowConstructible      = 1ULL << 18,
        IsNoThrowCopyable           = 1ULL << 19,
        IsNoThrowAssignable         = 1ULL << 20,

        IsMoveConstructible         = 1ULL << 21,
        IsMoveAssignable            = 1ULL << 22,

        IsDestructible              = 1ULL << 23,

        IsIterable                  = 1ULL << 24,

        IsConst                     = 1ULL << 25,
        IsVolatile                  = 1ULL << 26,
        IsAbstract                  = 1ULL << 27,
        IsFinal                     = 1ULL << 28,
        IsTrivial                   = 1ULL << 29,
        IsPolymorphic               = 1ULL << 30,
        IsStandardLayout            = 1ULL << 31,
        IsPOD                       = 1ULL << 32,
        IsAggregate                 = 1ULL << 33,
        IsLiteral                   = 1ULL << 34,
        IsSigned                    = 1ULL << 35,
        IsUnsigned                  = 1ULL << 36,
        IsArithmetic                = 1ULL << 37,
        IsFundamental               = 1ULL << 38,
        IsObject                    = 1ULL << 39,
        IsScalar                    = 1ULL << 40,
        IsCompound                  = 1ULL << 41,
    };


    class TypeData
    {
    public:
        using NamespaceContainer = std::vector<std::string>;
        using TemplateTypeContainer = std::vector<TypeReference>;

        inline TypeData(const TypeData& typeData);
        inline TypeData(TypeData&& typeData);
        inline TypeData& operator=(const TypeData& typeData);

        inline bool equals(const TypeData& typeData) const;

        inline const std::string& name() const;
        inline const std::string& fully_qualified_name() const;
        inline std::size_t size() const;
        inline std::size_t hash() const;

        inline bool has_flag(TypeFlag flag) const;

        inline const std::type_index& type_index() const;

        //TODO: Remove this function. It's only used in the TypeStorage class.
        inline bool is_registered_by_user() const;

        inline const ConstructorContainer& constructors() const;
        inline const PropertyContainer& properties() const;
        inline const MethodContainer& methods() const;
        inline const NamespaceContainer& namespaces() const;
        inline const TemplateTypeContainer& template_params() const;
        inline const ValueContainer& values() const;

        inline const MetaContainer& meta() const;

        inline const TypeFunctions& type_functions() const;

    protected:
        std::string _Name;
        std::string _FullyQualifiedName;
        std::size_t _Size;
        std::type_index _TypeIndex;

        uint64_t _Flags;

        bool _RegisteredByUser;

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

        TypeFunctions _TypeFunctions;

        friend class TypeStorage;
        inline TypeData(const std::string& name,
            const std::string& fullyQualifiedName,
            std::size_t size,
            uint64_t flags,
            std::type_index typeIndex,
            bool registeredByUser,
            const NamespaceContainer& namespaces,
            const TemplateTypeContainer& templateParams,
            TypeFunctions typeFunctions
        );

        inline TypeData(const std::string& name,
            const std::string& fqn,
            std::size_t size, 
            uint64_t flags, 
            std::type_index typeIndex);
    };
}
