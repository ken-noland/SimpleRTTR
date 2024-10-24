
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

    Type& Type::operator=(const Type& other)
    {
        if(this != &other) 
        { 
            // remove the const qualifier since this is just copying a reference over
            const_cast<TypeData&>(_TypeData) = other._TypeData;
        }
        return *this;
    }

    bool Type::operator==(const Type& type) const
    {
        return equals(type);
    }

    bool Type::operator!=(const Type& type) const
    {
        return !equals(type);
    }

    bool Type::equals(const Type& type) const
    {
        return this == &type || _TypeData.equals(type._TypeData);
    }

    bool Type::operator==(const std::type_info& info) const
    {
        return equals(info);
    }

    bool Type::operator!=(const std::type_info& info) const
    {
        return !equals(info);
    }

    bool Type::equals(const std::type_info& typeData) const
    {
        return type_index() == typeData;
    }

    const Type& Type::invalid_type()
    {
        static Type invalidType(TypeStorage::invalid_type_data());
        return invalidType;
    }

    const std::string& Type::name() const
    {
        return _TypeData.name();
    }

    const std::string& Type::fully_qualified_name() const
    {
        return _TypeData.fully_qualified_name();
    }

    std::size_t Type::size() const
    {
        return _TypeData.size();
    }

    std::size_t Type::hash() const
    {
        return _TypeData.hash();
    }

    bool Type::has_flag(TypeFlag flag) const
    {
        return _TypeData.has_flag(flag);
    }

    const std::type_index& Type::type_index() const
    {
        return _TypeData.type_index();
    }

    const ConstructorContainer& Type::constructors() const
    {
        return _TypeData.constructors();
    }

    const PropertyContainer& Type::properties() const
    {
        return _TypeData.properties();
    }

    const MethodContainer& Type::methods() const
    {
        return _TypeData.methods();
    }

    const Type::NamespaceList& Type::namespaces() const
    {
        return _TypeData.namespaces();
    }

    const Type::TemplateTypeList& Type::template_params() const
    {
        return _TypeData.template_params();
    }

    const ValueContainer& Type::values() const
    {
        return _TypeData.values();
    }

    const MetaContainer& Type::meta() const
    {
        return _TypeData.meta();
    }

    template<typename ClassType, typename Alloc, typename... Params>
    ClassType* Type::create_instance(Params... params) const
    {
        SIMPLERTTR_ASSERT(type_index() == typeid(ClassType));   //TODO: check inheritance!
        Alloc alloc; ClassType* pointer = alloc.allocate(1);
        //TODO: store the function pointer for the constructor/destructor and use that. It will help in cases
        //  where we don't have the headers to compile/link against.
        return new (pointer) ClassType(std::forward<Params>(params)...);   //TODO: use a user defined constructor(one that takes parameters)
    }

    template<typename... Params>
    void* Type::create_instance(Params...) const
    {
        //SIMPLERTTR_ASSERT(fully_qualified_name() == TypeHelper<ClassType>().QualifiedName());   //TODO: check inheritance!
        //Alloc alloc; void* pointer = alloc.allocate(1);
        ////TODO: we really need to call the constructor here!
        //return pointer;   //TODO: use a user defined constructor(one that takes parameters)
        return nullptr;
    }

    template<typename ClassType, typename Alloc>
    void Type::destroy_instance(ClassType* ptr) const
    {
        SIMPLERTTR_ASSERT(type_index() == typeid(ClassType));   //TODO: check inheritance!
        Alloc alloc; ptr->~ClassType();
        alloc.deallocate(ptr, 1);
    }

    void Type::destroy_instance(void* ptr) const
    {
        (void)ptr;
        SIMPLERTTR_ASSERT(!"Not yet implemented");   //TODO: check inheritance!
    }

    const TypeFunctions& Type::type_functions() const
    {
        return _TypeData.type_functions();
    }

    inline TypeData& _InternalGetTypeData(const Type& type)
    {
        return const_cast<TypeData&>(type._TypeData);
    }

    //--
    //Type Storage
    TypeStorage::TypeStorage(std::function<void(class TypeData&)> onRegisterTypeCallback)
        :
        _OnRegisterTypeCallback(onRegisterTypeCallback)
    {

    }


    const TypeData& TypeStorage::invalid_type_data()
    {
        static TypeData invalidTypeData("<invalid>","<invalid>",(std::size_t)-1, 0, typeid(void));
        return invalidTypeData;
    }

    constexpr std::size_t TypeStorage::invalid_type_size()
    {
        return (std::size_t)-1;
    }

    template<typename ClassType>
    bool TypeStorage::has_type_data() const
    {
        TypeHelper<ClassType> typeHelper;
        return HasTypeData(typeHelper);
    }

    bool TypeStorage::has_type_data(const std::type_info& typeInfo) const
    {
        return has_type_data(std::type_index(typeInfo));
    }

    bool TypeStorage::has_type_data(const std::type_index& typeInfo) const
    {
        return std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
            return typeInfo == typeData->type_index();
            }) != _Data.end();
    }

    bool TypeStorage::has_type_data(const std::string& name, std::size_t size) const
    {
        // search by name and size, and if not found, then attempt to find just by name
        if (size != invalid_type_size())
        {
            return std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
                return size == typeData->size() && name == typeData->name();
                }) != _Data.end();
        }
        else
        {
            return std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
                return name == typeData->name();
                }) != _Data.end();
        }
    }

    bool TypeStorage::has_type_data(const TypeHelperBase& typeHelper) const
    {
        return std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
            return typeHelper.type_index() == typeData->type_index();
            }) != _Data.end();
    }

    template<typename ClassType>
    const TypeData& TypeStorage::get_type_data() const
    {
        return get_type_data(typeid(ClassType));
    }

    const TypeData& TypeStorage::get_type_data(const std::type_info& typeInfo) const
    {
        return get_type_data(std::type_index(typeInfo));
    }

    const TypeData& TypeStorage::get_type_data(const std::type_index& typeIndex) const
    {
        TypeList::const_iterator found = std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
            return typeIndex == typeData->type_index();
            });
        if(found == _Data.end()) { return invalid_type_data(); }
        return *(*found).get();
    }

    const TypeData& TypeStorage::get_type_data(const std::string& name, std::size_t size) const
    {
        //TODO: yeah, this funciton is all over the place. Searching by name and fully_qualified_name is just... wrong.
        //  instead, users should iterate over the types themselves using std::find_if rather then relying on this function.
        // 
        //  I've deprecated the function in TypeManager for now.
        TypeList::const_iterator found = _Data.end();
        if (size != invalid_type_size())
        {
            found = std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
                return size == typeData->size() && (name.compare(typeData->fully_qualified_name()) == 0 || name.compare(typeData->name()) == 0);
                });
        }
        else
        {
            found = std::find_if(_Data.begin(), _Data.end(), [&](const TypePointer& typeData) {
                return name.compare(typeData->fully_qualified_name()) == 0 || name.compare(typeData->name()) == 0;
                });
        }
        if(found == _Data.end()) { return invalid_type_data(); }
        return *(*found).get();
    }

    const TypeData& TypeStorage::get_type_data(const TypeHelperBase& typeHelper) const
    {
        TypeList::const_iterator found = std::find_if(_Data.begin(), _Data.end(), [&](const std::unique_ptr<TypeData>& typeData) {
            return typeHelper.type_index() == typeData->type_index();
            });
        if (found == _Data.end()) { return invalid_type_data(); }
        return *(*found).get();
    }

    TypeData& TypeStorage::get_or_update_type_data(const TypeHelperBase& typeHelper, bool addedByUser)
    {
        TypeList::iterator typeIter = std::find_if(_Data.begin(), _Data.end(), [&](const std::unique_ptr<TypeData>& typeData) {
            return typeHelper.type_index() == typeData->type_index();
            });

        TypeData* type = (*typeIter).get();
        if (!type->is_registered_by_user() && addedByUser)
        {
            //well, we have a type that was auto-registered before, but now is being added by the user. Easiest way to 
            // deal with it is to remove the entry and simply re-register it. 
            _Data.erase(typeIter);
            return register_type(typeHelper, addedByUser);
        }
        return *type;
    }


    template<typename ClassType>
    TypeData& TypeStorage::get_or_create_type(bool addedByUser)
    {
        TypeHelper<ClassType> typeHelper;
        if (has_type_data(typeHelper))
        {
            return get_or_update_type_data(typeHelper, addedByUser);
        }
        else
        {
            return register_type(typeHelper, addedByUser);
        }
    }

    TypeData& TypeStorage::register_type(const TypeHelperBase& typeHelper, bool addedByUser)
    {
        TypeData data(
            typeHelper.name(),
            typeHelper.fully_qualified_name(),
            typeHelper.size(),
            typeHelper.flags(),
            typeHelper.type_index(),
            addedByUser,
            typeHelper.namespaces(),
            typeHelper.template_params(),
            typeHelper.type_functions()
        );

        return register_type(data);
    }

    TypeData& TypeStorage::register_type(const TypeData& typeData)
    {
        _Data.push_back(std::make_unique<TypeData>(typeData));

        TypeData& newTypeData = (*_Data.back().get());
        if (_OnRegisterTypeCallback)
        {
            _OnRegisterTypeCallback(newTypeData);
        }
        return newTypeData;
    }

    TypeManager::TypeManager()
        :
        _TypeDataStorage([this](TypeData& typeData) { on_type_registered(typeData); })
    {

    }

    TypeManager::~TypeManager()
    {

    }

    std::unique_ptr<TypeManager>& TypeManager::instance()
    {
        static std::unique_ptr<TypeManager> instance = std::make_unique<TypeManager>();
        return instance;
    }

    template<class ClassType>
    bool TypeManager::has_type() const
    {
        return get_storage().has_type_data(typeid(ClassType));
    }

    bool TypeManager::has_type(const std::type_info& typeInfo) const
    {
        return get_storage().has_type_data(typeInfo);
    }

    bool TypeManager::has_type(const std::type_index& typeIndex) const
    {
        return get_storage().has_type_data(typeIndex);
    }

    bool TypeManager::has_type(const TypeHelperBase& typeHelper) const
    {
        return get_storage().has_type_data(typeHelper.type_index());
    }

    template<class ClassType>
    const Type TypeManager::get_type() const
    {
        return get_storage().get_type_data<ClassType>();
    }

    const Type TypeManager::get_type(const std::type_info& typeInfo) const
    {
        return get_storage().get_type_data(typeInfo);
    }

    const Type TypeManager::get_type(const std::type_index& typeIndex) const
    {
        return get_storage().get_type_data(typeIndex);
    }

    const Type TypeManager::get_type(const TypeHelperBase& typeHelper) const
    {
        return get_storage().get_type_data(typeHelper);
    }

    template<class ClassType>
    const Type TypeManager::get_or_create_type()
    {
        TypeHelper<ClassType> helper;
        if (has_type(helper))
        {
            return get_type(helper);
        }
        else
        {
            Type type(get_storage().get_or_create_type<ClassType>(false));
            return type;
        }
    }
            
    TypeManager::iterator TypeManager::begin()
    {
        return get_storage().begin();
    }

    TypeManager::const_iterator TypeManager::begin() const
    {
        return get_storage().begin();
    }

    TypeManager::iterator TypeManager::end()
    {
        return get_storage().end();
    }

    TypeManager::const_iterator TypeManager::end() const
    {
        return get_storage().end();
    }


    void TypeManager::begin_registration(const char* filename)
    {
        Meta source_filename("source_filename", filename);
        _UserTypeMetadata.push_back(source_filename);
    }

    void TypeManager::end_registration()
    {
        _UserTypeMetadata.clear();
    }

    TypeStorage& TypeManager::get_storage()
    {
        return _TypeDataStorage;
    }

    const TypeStorage& TypeManager::get_storage() const
    {
        return _TypeDataStorage;
    }

    void TypeManager::register_type(const TypeData& data)
    {
        get_storage().register_type(data);
    }

    void TypeManager::on_type_registered(TypeData& data)
    {
        if (data.is_registered_by_user())
        {
            //copy over the metadata in the user type meta section
            for (const Meta& meta : _UserTypeMetadata)
            {
                _InternalGetMetadata(data).add(meta);
            }
        }
    }
}
