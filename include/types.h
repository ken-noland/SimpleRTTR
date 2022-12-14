#pragma once

namespace SimpleRTTR
{
    class Type
    {
    public:
        using PropertyList = TypeData::PropertyList;
        using MethodList = TypeData::MethodList;
        using MetaList = TypeData::MetaList;
        using NamespaceList = TypeData::NamespaceList;
        using TemplateTypeRef = TypeData::TemplateTypeRef;
        using TemplateTypeList = TypeData::TemplateTypeList;

        inline Type(const TypeData& data);
        inline Type(const Type& type);
        inline Type(Type&& type);
        inline ~Type();

        inline bool operator==(const Type& type) const;
        inline bool operator!=(const Type& type) const;
        inline bool Equals(const Type& type) const;

        inline bool operator==(const std::type_info& info) const;
        inline bool operator!=(const std::type_info& info) const;
        inline bool Equals(const std::type_info& typeData) const;

        static inline const Type& InvalidType();

        inline const stdrttr::string& Name() const;
        inline const stdrttr::string& FullyQualifiedName() const;
        inline std::size_t Size() const;

        inline const PropertyList& Properties() const;
        inline const MethodList& Methods() const;

        inline const MetaList& Meta() const;

        inline const NamespaceList& Namespaces() const;
        inline const TemplateTypeList& TemplateParams() const;

        template<typename ClassType, typename Alloc = stdrttr::custom_allocator<ClassType>, typename... Params>
        inline ClassType* CreateInstance(Params...) const;

        template<typename... Params>
        inline void* CreateInstance(Params...) const;

        template<typename ClassType, typename Alloc = stdrttr::custom_allocator<ClassType>>
        inline void DestroyInstance(ClassType* ptr) const;

        inline void DestroyInstance(void* ptr) const;

        template<typename ClassType, typename PropType>
        inline bool SetProperty(ClassType* Instance, const stdrttr::string& propertyName, const PropType& value) const;

    protected:
        const TypeData& _TypeData;
    };

    class TypeStorage
    {
    public:
        static inline const TypeData& InvalidTypeData();

        template<typename ClassType>
        inline bool HasTypeData();
        inline bool HasTypeData(const stdrttr::string& name, std::size_t size);

        template<typename ClassType>
        inline TypeData& GetTypeData();
        inline TypeData& GetTypeData(const stdrttr::string& name, std::size_t size);

        template<typename ClassType>
        inline TypeData& GetOrCreateType();

    protected:
        inline bool HasTypeData(const TypeHelperBase& typeHelper) const;
        inline TypeData& GetTypeData(const TypeHelperBase& typeHelper);

        inline TypeData& RegisterType(const TypeHelperBase& typeHelper);

        stdrttr::vector<std::unique_ptr<TypeData>> _Data;
    };

    class TypeManager;
    inline TypeManager& Types();

    class TypeManager
    {
    public:
        inline TypeManager();
        inline ~TypeManager();

        static inline TypeManager& GetInstance();

        template<class ClassType>
        inline bool HasType() const;
        inline bool HasType(const std::type_info& typeInfo) const;
        inline bool HasType(const stdrttr::string& name, std::size_t size = -1) const;

        template<class ClassType>
        inline const Type& GetType() const;
        inline const Type& GetType(const std::type_info& typeInfo) const;
        inline const Type& GetType(const stdrttr::string& name, std::size_t size = -1) const;

        template<class ClassType>
        inline const Type& GetOrCreateType();

        using TypeFunction = std::add_pointer<void(const Type&)>::type;
        inline void ForEach(TypeFunction eval) const;

    protected:
        friend class TypeBindingBase;
        friend class RegistrationManager;

        inline TypeStorage& GetStorage();  //only accessible from TypeBinding
        TypeStorage _TypeDataStorage;

        inline bool HasType(const TypeHelperBase& typeHelper) const;
        inline const Type& GetType(const TypeHelperBase& typeHelper) const;

        inline void RegisterType(const TypeData& data);

        using TypePointer = std::unique_ptr<Type>;
        using TypeList = stdrttr::vector<TypePointer>;

        TypeList _Types;

    //TEMP
    public:
        using iterator = TypeList::const_iterator;

        iterator begin() { return _Types.begin(); }
        iterator end() { return _Types.end(); }
    };
}
