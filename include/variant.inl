namespace SimpleRTTR
{
    template<typename VariantType>
    Variant::Variant(VariantType value)
        :
        _typeRef(typeid(VariantType))
    {
        static_assert(std::is_copy_constructible<VariantType>::value, "VariantType must be copy constructible");
        static_assert(sizeof(_storage) >= sizeof(VariantType), "Storage is not large enough for VariantType");

        Type type = types().get_or_create_type<VariantType>();    //we must dynamically look up the Type since the underlying storage for all types can be invalidated
        SIMPLERTTR_ASSERT_MSG(type != Type::invalid_type(), "Attempting to use Variant with unknown type");
        SIMPLERTTR_ASSERT(_typeRef == type);   //sanity check in case something with type storage goes awry

        _copyFunc = type.type_functions().CopyConstructor;   //store the copy function for later use
        SIMPLERTTR_ASSERT_MSG(_copyFunc != nullptr, "Attempting to use Variant with no copy constructor");

        _moveFunc = type.type_functions().MoveConstructor;   //store the move function for later use
        SIMPLERTTR_ASSERT_MSG(_moveFunc != nullptr, "Attempting to use Variant with no move constructor");

        _destroyFunc = type.type_functions().Destructor;    //store the destructor function for later use
        SIMPLERTTR_ASSERT_MSG(_destroyFunc != nullptr, "Attempting to use Variant with no destructor function");

        _equalFunc = type.type_functions().EqualOperator;    //store the equality function for later use

        //copy the value over
        _copyFunc(&_storage, &value);
    }

    // Constructor for void pointers
    Variant::Variant(void* valuePtr, const SimpleRTTR::Type& type)
        :
        _typeRef(type),
        _copyFunc(type.type_functions().CopyConstructor),
        _moveFunc(type.type_functions().MoveConstructor),
        _destroyFunc(type.type_functions().Destructor),
        _equalFunc(type.type_functions().EqualOperator)
    {
        // use the copy constructor pointer
        _copyFunc(&_storage, valuePtr);
    }

    // Copy constructor
    Variant::Variant(const Variant& var)
        : 
        _typeRef(var._typeRef),
        _copyFunc(var._copyFunc),
        _moveFunc(var._moveFunc),
        _destroyFunc(var._destroyFunc),
        _equalFunc(var._equalFunc)
    {
        //call the copy function
        _copyFunc(&_storage, &var._storage);
    }

    // Move constructor
    Variant::Variant(Variant&& var)
        : 
        _typeRef(var._typeRef),
        _copyFunc(var._copyFunc),
        _moveFunc(var._moveFunc),
        _destroyFunc(var._destroyFunc),
        _equalFunc(var._equalFunc)
    {
        // use the move constructor pointer
        _moveFunc(&_storage, &var._storage);
    }

    // Destructor
    Variant::~Variant()
    {
        destroy();
    }

    // Copy assignment
    Variant& Variant::operator=(const Variant& var)
    {
        if(this != &var && _typeRef == var._typeRef)
        {
            destroy();
            _copyFunc(&_storage, &var._storage);
        }

        return *this;
    }

    bool Variant::operator==(const Variant& var) const
    {
        // Ensure that the types are the same before comparing
        if(!(_typeRef == var._typeRef))
        {
            return false;
        }
                
        return _equalFunc(&_storage, &var._storage);
    }

    bool Variant::operator!=(const Variant& var) const
    {
        return !operator==(var);
    }

    template<typename ObjectType>
    bool Variant::operator==(const ObjectType& var) const
    {
        // Check if we are comparing a member function pointer
        if constexpr (std::is_member_function_pointer_v<ObjectType>)
        {
            // Special comparison logic for member function pointers
            const ObjectType* storedPointer = reinterpret_cast<const ObjectType*>(&_storage);
            return *storedPointer == var;
        }
        // Check if the type is an array and decay it to a pointer for comparison
        else if constexpr (std::is_array_v<ObjectType>)
        {
            using DecayedType = std::decay_t<ObjectType>;  // Decay array to pointer
            return _equalFunc(&_storage, &(DecayedType)var);
        }
        else
        {
            // Ensure that the types are the same before comparing
            if(!(_typeRef.type_index == typeid(ObjectType)))
            {
                return false;
            }

            return _equalFunc(&_storage, var);
        }
    }

    void Variant::destroy()
    {
        // get the type
        _destroyFunc(&_storage);
    }

    template<typename VariantType>
    VariantType Variant::get_as() const
    {
        // 1. Handle direct type match (if type matches exactly, return it)
        if(_typeRef.type_index() == std::type_index(typeid(VariantType)))
        {
            return *reinterpret_cast<const VariantType*>(&_storage);
        }

        // 2. Handle conversion
        Type sourceType = _typeRef.type();
        for(const std::pair<std::type_index, TypeFunctions::ConversionFunction>& conversion : sourceType.type_functions().ConversionFunctions)
        {
            if(conversion.first == std::type_index(typeid(VariantType)))
            {
                using DestType = std::remove_cv_t<std::remove_reference_t<VariantType>>;
                DestType dest;
                conversion.second(&dest, &_storage);
                return dest;
            }
        }

        throw std::bad_cast();
    }


    // Specializations for char* <-> std::string and wchar_t* <-> std::wstring conversions
    template<>
    inline std::string Variant::get_as<std::string>() const
    {
        if(_typeRef.type_index() == std::type_index(typeid(const char*)))
        {
            return std::string(*reinterpret_cast<const char* const*>(&_storage));
        }
        else if(_typeRef.type_index() == std::type_index(typeid(std::string)))
        {
            return *reinterpret_cast<const std::string*>(&_storage);
        }

        throw std::bad_cast();
    }

    template<>
    inline const char* Variant::get_as<const char*>() const
    {
        if(_typeRef.type_index() == std::type_index(typeid(std::string)))
        {
            return reinterpret_cast<const std::string*>(&_storage)->c_str();
        }
        else if(_typeRef.type_index() == std::type_index(typeid(const char*)))
        {
            return *reinterpret_cast<const char* const*>(&_storage);
        }

        throw std::bad_cast();
    }

    template<>
    inline std::wstring Variant::get_as<std::wstring>() const
    {
        if(_typeRef.type_index() == std::type_index(typeid(const wchar_t*)))
        {
            return std::wstring(*reinterpret_cast<const wchar_t* const*>(&_storage));
        }
        else if(_typeRef.type_index() == std::type_index(typeid(std::wstring)))
        {
            return *reinterpret_cast<const std::wstring*>(&_storage);
        }

        throw std::bad_cast();
    }

    template<>
    inline const wchar_t* Variant::get_as<const wchar_t*>() const
    {
        if(_typeRef.type_index() == std::type_index(typeid(std::wstring)))
        {
            return reinterpret_cast<const std::wstring*>(&_storage)->c_str();
        }
        else if(_typeRef.type_index() == std::type_index(typeid(const wchar_t*)))
        {
            return *reinterpret_cast<const wchar_t* const*>(&_storage);
        }

        throw std::bad_cast();
    }

    SimpleRTTR::Type Variant::type() const
    {
        return _typeRef.type();
    }

    std::size_t Variant::hash() const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        return 0;
    }

    std::string Variant::to_string() const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        return "";
    }

    const void* Variant::ptr() const
    {
        return reinterpret_cast<const void*>(&_storage);
    }
}
