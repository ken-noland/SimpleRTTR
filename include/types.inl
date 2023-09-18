
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
        return _TypeData.GetName();
    }

    const stdrttr::string& Type::FullyQualifiedName() const
    {
        return _TypeData.GetFullyQualifiedName();
    }

    std::size_t Type::Size() const
    {
        return _TypeData.GetSize();
    }

    const Type::PropertyList& Type::Properties() const
    {
        return _TypeData.GetPropertyList();
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
        return _TypeData.GetMethodList();
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

    const MetaContainer& Type::Meta() const
    {
        return _TypeData.GetMetadata();
    }

    void Type::ForEach(Type::MetaFunction function) const
    {
        MetaContainer::const_iterator iter = Meta().begin();
        while (iter != Meta().end())
        {
            function(*iter);
            ++iter;
        }
    }

    const Type::NamespaceList& Type::Namespaces() const
    {
        return _TypeData.GetNamespaces();
    }

    const Type::TemplateTypeList& Type::TemplateParams() const
    {
        return _TypeData.GetTemplateParams();
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
        return _TypeData.GetToStringFunction()(var);
    }

    const stdrttr::string& Type::GetFullyQualifiedName() const
    {
        return _TypeData.GetFullyQualifiedName();
    }

    //--
    //Type Storage
    TypeStorage::TypeStorage(std::function<void(class TypeData&)> onRegisterTypeCallback)
        :
        _OnRegisterTypeCallback(onRegisterTypeCallback)
    {

    }


    const TypeData& TypeStorage::InvalidTypeData()
    {
        static TypeData invalidTypeData("<invalid>","<invalid>",-1);
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
                return size == typeData->GetSize() && name == typeData->GetName();
                }) != _Data.end();
        }
        else
        {
            return std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
                return name == typeData->GetName();
                }) != _Data.end();
        }
    }

    inline bool TypeStorage::HasTypeData(const TypeHelperBase& typeHelper) const
    {
        return std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
            return typeHelper.Size() == typeData->GetSize() && typeHelper.QualifiedName() == typeData->GetFullyQualifiedName();
            }) != _Data.end();
    }

    template<typename ClassType>
    const TypeData& TypeStorage::GetTypeData() const
    {
        TypeHelper<ClassType> typeHelper;
        return GetTypeData(typeHelper);
    }

    const TypeData& TypeStorage::GetTypeData(const stdrttr::string& name, std::size_t size) const
    {
        TypeList::const_iterator found = _Data.end();
        if (size != InvalidTypeSize())
        {
            found = std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
                return size == typeData->GetSize() && (name.compare(typeData->GetFullyQualifiedName()) == 0 || name.compare(typeData->GetName()) == 0);
                });
        }
        else
        {
            found = std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
                return name.compare(typeData->GetFullyQualifiedName()) == 0 || name.compare(typeData->GetName()) == 0;
                });
        }
        if(found == _Data.end()) { return InvalidTypeData(); }
        return *(*found).get();
    }

    const TypeData& TypeStorage::GetTypeData(const TypeHelperBase& typeHelper) const
    {
        TypeList::const_iterator found = std::find_if(_Data.begin(), _Data.end(), [&](const std::unique_ptr<TypeData>& typeData) {
            return typeHelper.QualifiedName() == typeData->GetFullyQualifiedName();
            });
        if (found == _Data.end()) { return InvalidTypeData(); }
        return *(*found).get();
    }

    TypeData& TypeStorage::GetOrUpdateTypeData(const TypeHelperBase& typeHelper, bool addedByUser)
    {
        TypeList::iterator typeIter = std::find_if(_Data.begin(), _Data.end(), [&](const std::unique_ptr<TypeData>& typeData) {
            return typeHelper.QualifiedName() == typeData->GetFullyQualifiedName();
            });

        TypeData* type = (*typeIter).get();
        if (!type->IsRegisteredByUser() && addedByUser)
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
        TypeData data(
            typeHelper.Name(),
            typeHelper.QualifiedName(),
            typeHelper.Size(),
            addedByUser,
            typeHelper.Namespaces(),
            typeHelper.TemplateParams(),
            typeHelper.ToStringFunc());

        return RegisterType(data);
    }

    TypeData& TypeStorage::RegisterType(const TypeData& typeData)
    {
        _Data.push_back(std::make_unique<TypeData>(typeData));

        TypeData& newTypeData = (*_Data.back().get());
        if (_OnRegisterTypeCallback)
        {
            _OnRegisterTypeCallback(newTypeData);
        }
        return newTypeData;
    }

    void TypeStorage::ForEach(TypeDataFunction eval) const
    {

    }



    TypeManager& Types() 
    { 
        return TypeManager::GetInstance(); 
    }

    TypeManager::TypeManager()
        :
        _TypeDataStorage([this](TypeData& typeData) { OnTypeRegistered(typeData); })
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

    void TypeManager::BeginRegistration(const char* filename)
    {
        Meta source_filename("source_filename", filename);
        _UserTypeMetadata.push_back(source_filename);
    }

    void TypeManager::EndRegistration()
    {
        _UserTypeMetadata.clear();
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

    void TypeManager::OnTypeRegistered(TypeData& data)
    {
        if (data.IsRegisteredByUser())
        {
            printf("Adding metadata to type %s\n", data.GetName().c_str());
            //copy over the metadata in the user type meta section
            for (const Meta& meta : _UserTypeMetadata)
            {
                data.AddMetadata(meta);
            }
        }
    }

    void TypeManager::ForEach(TypeManager::TypeFunction eval) const
    {
        GetStorage().ForEach([&](const TypeData& typeData) {
            Type type(typeData);
            eval(type);
            });
    }
}
