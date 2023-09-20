#pragma once

namespace SimpleRTTR
{
    class Type
    {
    public:
        using PropertyList = TypeData::PropertyList;
        using MethodList = TypeData::MethodList;
        using NamespaceList = TypeData::NamespaceList;
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
        using PropertyFunction = std::function<void(const class Property&)>;

        inline const MethodList& Methods() const;
        using MethodFunction = std::function<void(const class Method&)>;

        inline const MetaContainer& Meta() const;
        using MetaFunction = std::function<void(const class Meta&)>;

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

        inline stdrttr::string ToString(const Variant& var) const;

        inline const stdrttr::string& GetFullyQualifiedName() const;

    protected:
        const TypeData& _TypeData;
    };

    class TypeStorage
    {
    public:
        using TypePointer = std::unique_ptr<TypeData>;
        using TypeList = stdrttr::vector<TypePointer>;

        using Iterator = TypeList::iterator;
        using ConstIterator = TypeList::const_iterator;

        static inline const TypeData& InvalidTypeData();
        static inline constexpr std::size_t InvalidTypeSize();

        template<typename ClassType>
        inline bool HasTypeData() const;
        inline bool HasTypeData(const stdrttr::string& name, std::size_t size) const;
        inline bool HasTypeData(const TypeHelperBase& typeHelper) const;

        template<typename ClassType>
        inline const TypeData& GetTypeData() const;
        inline const TypeData& GetTypeData(const stdrttr::string& name, std::size_t size) const;
        inline const TypeData& GetTypeData(const TypeHelperBase& typeHelper) const;

        template<typename ClassType>
        inline TypeData& GetOrCreateType(bool _addedByUser);

        inline Iterator Begin() { return _Data.begin(); }
        inline ConstIterator Begin() const { return _Data.begin(); }

        inline Iterator End() { return _Data.end(); }
        inline ConstIterator End() const { return _Data.end(); }


    protected:
        friend class TypeManager;

        //only the TypeManager can create a TypeStorage
        inline TypeStorage(std::function<void(class TypeData&)> OnRegisterTypeCallback);

        inline TypeData& GetOrUpdateTypeData(const TypeHelperBase& typeHelper, bool addedByUser);
        inline TypeData& RegisterType(const TypeHelperBase& typeHelper, bool addedByUser);
        inline TypeData& RegisterType(const TypeData& typeData);


        TypeList _Data;

        std::function<void(class TypeData&)> _OnRegisterTypeCallback;
    };

    //Since we store types in TypeData but we need to convert it to Type, this is done in a special iterator.
    template<typename TypeMgr>
    class TypeIterable
    {
    public:

        class TypeIteratorProxy : public IteratorProxyBase<TypeStorage::TypeList::iterator>
        {
        public:
            TypeIteratorProxy(iterator iter) : IteratorProxyBase(iter) {}
            value_type operator*() const { return *(*_Iter); }  //Notice we use value_type instead of reference here. This converts TypeData to Type.
            pointer operator->() { return *_Iter; }
        };

        class ConstTypeIteratorProxy : public IteratorProxyBase<TypeStorage::TypeList::const_iterator>
        {
        public:
            ConstTypeIteratorProxy(const_iterator iter) : IteratorProxyBase(iter) {}
            const value_type operator*() const { return *(*_Iter); } //Notice we use value_type instead of reference here. This converts TypeData to Type.
            const pointer operator->() const { return *_Iter; }
        };

        using iterator = TypeIteratorProxy;
        using const_iterator = ConstTypeIteratorProxy;

        iterator begin() { return static_cast<TypeMgr*>(this)->Begin(); }
        const_iterator begin() const { return static_cast<TypeMgr*>(this)->Begin(); }
        iterator end() { return static_cast<TypeMgr*>(this)->End(); }
        const_iterator end() const { return static_cast<TypeMgr*>(this)->End(); }
    };

    class TypeManager;
    inline TypeManager& Types();

    class TypeManager : public
        TypeIterable<TypeManager>
    {
    public:
        inline TypeManager();
        inline ~TypeManager();

        static inline TypeManager& GetInstance();

        template<class ClassType>
        inline bool HasType() const;
        inline bool HasType(const std::type_info& typeInfo) const;
        inline bool HasType(const stdrttr::string& name, std::size_t size = TypeStorage::InvalidTypeSize()) const;

        template<class ClassType>
        inline const Type GetType() const;
        inline const Type GetType(const std::type_info& typeInfo) const;
        inline const Type GetType(const stdrttr::string& name, std::size_t size = TypeStorage::InvalidTypeSize()) const;

        template<class ClassType>
        inline const Type GetOrCreateType();

        inline TypeIteratorProxy Begin();
        inline ConstTypeIteratorProxy Begin() const;

        inline TypeIteratorProxy End();
        inline ConstTypeIteratorProxy End() const;

    protected:
        friend class TypeBindingBase;
        friend class RegistrationManager;

        inline void BeginRegistration(const char* filename);
        inline void EndRegistration();

        inline TypeStorage& GetStorage();  //only accessible from TypeBinding
        inline const TypeStorage& GetStorage() const;  //only accessible from TypeBinding

        inline bool HasType(const TypeHelperBase& typeHelper) const;
        inline const Type GetType(const TypeHelperBase& typeHelper) const;

        inline void RegisterType(const TypeData& data);

        //called once the type has been registered
        inline void OnTypeRegistered(TypeData& data);

    private:
        TypeStorage _TypeDataStorage;

        std::vector<Meta> _UserTypeMetadata;
    };
}
