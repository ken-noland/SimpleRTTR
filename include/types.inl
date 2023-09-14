
namespace SimpleRTTR
{
    Type::Type(const TypeData& data)
        :
        _TypeData(data)
    {

    }

    Type::Type(const Type& type)
        :
        _TypeData(type._TypeData)
    {

    }

    Type::Type(Type&& type)
        :
        _TypeData(type._TypeData)
    {

    }


    Type::~Type()
    {

    }

    bool Type::operator==(const Type& type) const
    {
        return Equals(type);
    }

    bool Type::operator!=(const Type& type) const
    {
        return !Equals(type);
    }

    bool Type::Equals(const Type& type) const
    {
        return this == &type || _TypeData.Equals(type._TypeData);
    }

    bool Type::operator==(const std::type_info& info) const
    {
        return Equals(info);
    }

    bool Type::operator!=(const std::type_info& info) const
    {
        return !Equals(info);
    }

    bool Type::Equals(const std::type_info& typeData) const
    {
        //TODO: maybe not use a heavy constructor like TypeHelperBase(which is littered with allocations) to just test if a type if equal
        TypeHelperBase typeHelper(typeData, -1, nullptr);    
        return FullyQualifiedName().compare(typeHelper.QualifiedName()) == 0;
    }

    const Type& Type::InvalidType()
    {
        static Type invalidType(TypeStorage::InvalidTypeData());
        return invalidType;
    }

    const stdrttr::string& Type::Name() const
    {
        return _TypeData.Name;
    }

    const stdrttr::string& Type::FullyQualifiedName() const
    {
        return _TypeData.FullyQualifiedName;
    }

    std::size_t Type::Size() const
    {
        return _TypeData.Size;
    }

    const Type::PropertyList& Type::Properties() const
    {
        return _TypeData.Properties;
    }

    void Type::ForEach(Type::PropertyFunction function) const
    {
        Type::PropertyList::const_iterator iter = Properties().begin();
        while (iter != Properties().end())
        {
            function(*iter);
            ++iter;
        }
    }

    const Type::MethodList& Type::Methods() const
    {
        return _TypeData.Methods;
    }

    void Type::ForEach(Type::MethodFunction function) const
    {
        Type::MethodList::const_iterator iter = Methods().begin();
        while (iter != Methods().end())
        {
            function(*iter);
            ++iter;
        }
    }

    const Type::MetaList& Type::Meta() const
    {
        return _TypeData.Metadata;
    }

    void Type::ForEach(Type::MetaFunction function) const
    {
        Type::MetaList::const_iterator iter = Meta().begin();
        while (iter != Meta().end())
        {
            function(*iter);
            ++iter;
        }
    }

    const Type::NamespaceList& Type::Namespaces() const
    {
        return _TypeData.Namespaces;
    }

    const Type::TemplateTypeList& Type::TemplateParams() const
    {
        return _TypeData.TemplateParams;
    }

    template<typename ClassType, typename Alloc, typename... Params>
    ClassType* Type::CreateInstance(Params... params) const
    {
        SIMPLERTTR_ASSERT(FullyQualifiedName() == TypeHelper<ClassType>().QualifiedName());   //TODO: check inheritance!
        Alloc alloc; ClassType* pointer = alloc.allocate(1);
        //TODO: store the function pointer for the constructor/destructor and use that. It will help in cases
        //  where we don't have the headers to compile/link against.
        return new (pointer) ClassType(std::forward<Params>(params)...);   //TODO: use a user defined constructor(one that takes parameters)
    }

    template<typename... Params>
    void* Type::CreateInstance(Params...) const
    {
        //SIMPLERTTR_ASSERT(FullyQualifiedName() == TypeHelper<ClassType>().QualifiedName());   //TODO: check inheritance!
        //Alloc alloc; void* pointer = alloc.allocate(1);
        ////TODO: we really need to call the constructor here!
        //return pointer;   //TODO: use a user defined constructor(one that takes parameters)
        return nullptr;
    }

    template<typename ClassType, typename Alloc>
    void Type::DestroyInstance(ClassType* ptr) const
    {
        SIMPLERTTR_ASSERT(FullyQualifiedName() == TypeHelper<ClassType>().QualifiedName());   //TODO: check inheritance!
        Alloc alloc; ptr->~ClassType();
        alloc.deallocate(ptr, 1);
    }

    void Type::DestroyInstance(void* ptr) const
    {
        SIMPLERTTR_ASSERT(!"Not yet implemented");   //TODO: check inheritance!
    }

    stdrttr::string Type::ToString(const Variant& var) const
    {
        return _TypeData.ToString(var);
    }

    const stdrttr::string& Type::GetFullyQualifiedName() const
    {
        return _TypeData.GetFullyQualifiedName();
    }

    //--
    //Type Storage
    const TypeData& TypeStorage::InvalidTypeData()
    {
        static TypeData invalidTypeData;
        return invalidTypeData;
    }

    constexpr std::size_t TypeStorage::InvalidTypeSize()
    {
        return (std::size_t)-1;
    }

    template<typename ClassType>
    bool TypeStorage::HasTypeData() const
    {
        TypeHelper<ClassType> typeHelper;
        return HasTypeData(typeHelper);
    }

    bool TypeStorage::HasTypeData(const stdrttr::string& name, std::size_t size) const
    {
        if (size != InvalidTypeSize())
        {
            return std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
                return size == typeData->Size && name == typeData->Name;
                }) != _Data.end();
        }
        else
        {
            return std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
                return name == typeData->Name;
                }) != _Data.end();
        }
    }

    inline bool TypeStorage::HasTypeData(const TypeHelperBase& typeHelper) const
    {
        return std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
            return typeHelper.Size() == typeData->Size && typeHelper.QualifiedName() == typeData->FullyQualifiedName;
            }) != _Data.end();
    }

    template<typename ClassType>
    TypeData& TypeStorage::GetTypeData() const
    {
        TypeHelper<ClassType> typeHelper;
        return GetTypeData(typeHelper);
    }

    TypeData& TypeStorage::GetTypeData(const stdrttr::string& name, std::size_t size) const
    {
        TypeList::const_iterator found = _Data.end();
        if (size != InvalidTypeSize())
        {
            found = std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
                return size == typeData->Size && (name.compare(typeData->GetFullyQualifiedName()) == 0 || name.compare(typeData->Name)) == 0;
                });
        }
        else
        {
            found = std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
                return name.compare(typeData->GetFullyQualifiedName()) == 0 || name.compare(typeData->Name) == 0;
                });
        }
        if(found == _Data.end()) { throw std::invalid_argument("attempting to get a type that hasn't been registered"); }
        return *(*found).get();
    }

    TypeData& TypeStorage::GetTypeData(const TypeHelperBase& typeHelper) const
    {
        return *(*std::find_if(_Data.begin(), _Data.end(), [&](const std::unique_ptr<TypeData>& typeData) {
            return typeHelper.QualifiedName() == typeData->FullyQualifiedName;
            })).get();
    }

    TypeData& TypeStorage::GetOrUpdateTypeData(const TypeHelperBase& typeHelper, bool addedByUser)
    {
        TypeList::iterator typeIter = std::find_if(_Data.begin(), _Data.end(), [&](const std::unique_ptr<TypeData>& typeData) {
            return typeHelper.QualifiedName() == typeData->FullyQualifiedName;
            });

        TypeData* type = (*typeIter).get();
        if (!type->RegisteredByUser && addedByUser)
        {
            //well, we have a type that was auto-registered before, but now is being added by the user. Easiest way to 
            // deal with it is to remove the entry and simply re-register it. 
            _Data.erase(typeIter);
            return RegisterType(typeHelper, addedByUser);
        }
        return *type;
    }


    template<typename ClassType>
    TypeData& TypeStorage::GetOrCreateType(bool addedByUser)
    {
        TypeHelper<ClassType> typeHelper;
        if (HasTypeData(typeHelper))
        {
            return GetOrUpdateTypeData(typeHelper, addedByUser);
        }
        else
        {
            return RegisterType(typeHelper, addedByUser);
        }
    }

    TypeData& TypeStorage::RegisterType(const TypeHelperBase& typeHelper, bool addedByUser)
    {
        TypeData data;
        data.FullyQualifiedName = typeHelper.QualifiedName();
        data.Name = typeHelper.Name();
        data.Size = typeHelper.Size();
        data.Namespaces = typeHelper.Namespaces();
        data.ToString = typeHelper.ToStringFunc();
        data.RegisteredByUser = addedByUser;

        data.TemplateParams.reserve(typeHelper.TemplateParams().size());
        TypeHelperBase::TemplateTypeList::const_iterator iter = typeHelper.TemplateParams().begin();
        int index = 0;
        while (iter != typeHelper.TemplateParams().end())
        {
            data.TemplateParams.push_back(*(*iter));
            ++iter;
        }

        return RegisterType(data);
    }

    TypeData& TypeStorage::RegisterType(const TypeData& typeData)
    {
        _Data.push_back(std::make_unique<TypeData>(typeData));
        return (*_Data.back().get());
    }

    void TypeStorage::ForEach(TypeDataFunction eval) const
    {

    }



    TypeManager& Types() 
    { 
        return TypeManager::GetInstance(); 
    }

    TypeManager::TypeManager()
    {

    }

    TypeManager::~TypeManager()
    {

    }

    TypeManager& TypeManager::GetInstance()
    {
        static TypeManager instance;
        return instance;
    }

    template<class ClassType>
    bool TypeManager::HasType() const
    {
        TypeHelper<ClassType> helper;
        return HasType(helper);
    }

    bool TypeManager::HasType(const stdrttr::string& name, std::size_t size) const
    {
        return GetStorage().HasTypeData(name, size);
    }

    bool TypeManager::HasType(const TypeHelperBase& typeHelper) const
    {
        return GetStorage().HasTypeData(typeHelper);
    }

    template<class ClassType>
    const Type TypeManager::GetType() const
    {
        TypeHelper<ClassType> helper;
        return GetType(helper);
    }

    const Type TypeManager::GetType(const std::type_info& typeInfo) const
    {
        TypeHelperBase helper(typeInfo, -1, nullptr);
        return GetType(helper);
    }

    const Type TypeManager::GetType(const stdrttr::string& name, std::size_t size) const
    {
        Type type(GetStorage().GetTypeData(name, size));
        return type;
    }

    const Type TypeManager::GetType(const TypeHelperBase& typeHelper) const
    {
        Type type(GetStorage().GetTypeData(typeHelper));
        return type;
    }

    template<class ClassType>
    const Type TypeManager::GetOrCreateType()
    {
        TypeHelper<ClassType> helper;
        if (HasType(helper))
        {
            return GetType(helper);
        }
        else
        {
            Type type(GetStorage().GetOrCreateType<ClassType>(false));
            return type;
        }
    }


    TypeStorage& TypeManager::GetStorage()
    {
        return _TypeDataStorage;
    }

    const TypeStorage& TypeManager::GetStorage() const
    {
        return _TypeDataStorage;
    }

    void TypeManager::RegisterType(const TypeData& data)
    {
        GetStorage().RegisterType(data);
    }

    void TypeManager::ForEach(TypeManager::TypeFunction eval) const
    {
        GetStorage().ForEach([&](const TypeData& typeData) {
            Type type(typeData);
            eval(type);
            });
    }

}
