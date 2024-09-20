namespace SimpleRTTR
{
    template<typename VariantType>
    Variant::Variant(VariantType value)
        :
        _type(types().get_type<VariantType>())
    {
        static_assert(std::is_copy_constructible<VariantType>::value, "VariantType must be copy constructible");
        static_assert(sizeof(_storage) >= sizeof(VariantType), "Storage is not large enough for VariantType");

        SIMPLERTTR_ASSERT_MSG(_type != Type::invalid_type(), "Attempting to use Variant with unknown type");

        TypeFunctions::CopyConstructorFunction copy = _type.type_functions().CopyConstructor;
        SIMPLERTTR_ASSERT_MSG(copy != nullptr, "Attempting to use variant with no copy function");

        //copy the value over
        copy(&_storage, &value);
    }

    // Constructor for void pointers
    Variant::Variant(void* valuePtr, const SimpleRTTR::Type& type)
        :
        _type(type)
    {
        SIMPLERTTR_ASSERT_MSG(_type != Type::invalid_type(), "Attempting to use Variant with unknown type.");
        SIMPLERTTR_ASSERT_MSG(_type.size() > sizeof(_storage), "Storage is not large enough for Variant Type");

        SIMPLERTTR_ASSERT(!"Not implemented yet");

        // use the copy constructor pointer
    }

    // Copy constructor
    Variant::Variant(const Variant& var)
        : 
        _type(var._type)
    {
        SIMPLERTTR_ASSERT_MSG(_type != Type::invalid_type(), "Attempting to use Variant with unknown type.");

        SIMPLERTTR_ASSERT(!"Not implemented yet");

        // use the copy constructor pointer
    }

    // Move constructor
    Variant::Variant(Variant&& var)
        : 
        _type(var._type)
    {
        SIMPLERTTR_ASSERT_MSG(_type != Type::invalid_type(), "Attempting to use Variant with unknown type.");

        SIMPLERTTR_ASSERT(!"Not implemented yet");

        // use the move constructor pointer
    }

    // Destructor
    Variant::~Variant()
    {
        destroy();
    }

    // Copy assignment
    Variant& Variant::operator=(const Variant& var)
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        //if(this != &var)
        //{
        //    Destroy();
        //    _typeIndex = var._typeIndex;
        //    _copyFunc = var._copyFunc;
        //    _deleteFunc = var._deleteFunc;
        //    if(_typeIndex != typeid(void))
        //    {
        //        _copyFunc(_storage, var._storage);
        //    }
        //}
        return *this;
    }

    bool Variant::operator==(const Variant& var) const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        // Ensure that the types are the same before comparing
        //if(_typeIndex == var._typeIndex && _equalityFunc)
        //{
        //    return _equalityFunc(_storage, var._storage);
        //}
        return false;
    }

    bool Variant::operator!=(const Variant& var) const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        return false;
    }

    template<typename ObjectType>
    bool Variant::operator==(const ObjectType& var) const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        //if(_typeIndex == typeid(ObjectType) && _equalityFunc)
        //{
        //    return _equalityFunc(_storage, *reinterpret_cast<const StorageType*>(&var));
        //}
        return false;
    }

    void Variant::destroy()
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        //if(_typeIndex != typeid(void))
        //{
        //    // Use the stored delete function to destroy the object
        //    _deleteFunc(_storage);
        //    _typeIndex = std::type_index(typeid(void));
        //}
    }

    template<typename VariantType>
    VariantType Variant::get_as() const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        //// 1. Handle direct type match (if type matches exactly, return it)
        //if(_typeIndex == std::type_index(typeid(VariantType)))
        //{
        //    return *reinterpret_cast<const VariantType*>(&_storage);
        //}

        //// 2. Handle basic type conversion (char, short, int, etc.)
        //if constexpr(std::is_arithmetic_v<VariantType>)
        //{
        //    if constexpr(std::is_arithmetic_v<std::decay_t<decltype(*reinterpret_cast<const VariantType*>(&_storage))>>)
        //    {
        //        // Convert between basic arithmetic types
        //        return static_cast<VariantType>(*reinterpret_cast<const std::decay_t<decltype(*reinterpret_cast<const VariantType*>(&_storage))>*>(&_storage));
        //    }
        //}

        //// Check if the stored type is an enum and the target is fundamental
        //if constexpr(std::is_integral_v<VariantType>)
        //{
        //    const SimpleRTTR::Type& storedType = types().get_type(_typeIndex.name());
        //    if(storedType.has_flag(TypeFlag::IsEnum))
        //    {
        //        // Cast the enum to its underlying integer type
        //        return static_cast<VariantType>(*reinterpret_cast<const int*>(&_storage));
        //    }
        //}

        //// Check if the stored type is fundamental (e.g., int) and the target type is an enum
        //if constexpr(std::is_enum_v<VariantType>)
        //{
        //    //TODO: check the stored type
        //    //const SimpleRTTR::Type& storedType = types().get_type(_typeIndex.name());

        //    // Cast the int to an enum
        //    return static_cast<VariantType>(*reinterpret_cast<const int*>(&_storage));
        //}

        //// 4. Handle pointer to pointer conversion(checking inheritance)
        //// TODO!

        throw std::bad_cast();
    }


    // Specializations for char* <-> std::string and wchar_t* <-> std::wstring conversions
    template<>
    inline std::string Variant::get_as<std::string>() const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        //if(_typeIndex == std::type_index(typeid(const char*)))
        //{
        //    return std::string(*reinterpret_cast<const char* const*>(&_storage));
        //}
        //else if(_typeIndex == std::type_index(typeid(std::string)))
        //{
        //    return *reinterpret_cast<const std::string*>(&_storage);
        //}

        throw std::bad_cast();
    }

    template<>
    inline const char* Variant::get_as<const char*>() const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        //if(_typeIndex == std::type_index(typeid(std::string)))
        //{
        //    return reinterpret_cast<const std::string*>(&_storage)->c_str();
        //}
        //else if(_typeIndex == std::type_index(typeid(const char*)))
        //{
        //    return *reinterpret_cast<const char* const*>(&_storage);
        //}

        throw std::bad_cast();
    }

    template<>
    inline std::wstring Variant::get_as<std::wstring>() const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        //if(_typeIndex == std::type_index(typeid(const wchar_t*)))
        //{
        //    return std::wstring(*reinterpret_cast<const wchar_t* const*>(&_storage));
        //}
        //else if(_typeIndex == std::type_index(typeid(std::wstring)))
        //{
        //    return *reinterpret_cast<const std::wstring*>(&_storage);
        //}

        throw std::bad_cast();
    }

    template<>
    inline const wchar_t* Variant::get_as<const wchar_t*>() const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        //if(_typeIndex == std::type_index(typeid(std::wstring)))
        //{
        //    return reinterpret_cast<const std::wstring*>(&_storage)->c_str();
        //}
        //else if(_typeIndex == std::type_index(typeid(const wchar_t*)))
        //{
        //    return *reinterpret_cast<const wchar_t* const*>(&_storage);
        //}

        throw std::bad_cast();
    }

    inline void Variant::copy_to(void* dest, const Type& destType) const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        //_copyFunc(*reinterpret_cast<StorageType*>(destination), _storage);
    }

    const SimpleRTTR::Type& Variant::type() const
    {
        return _type;
    }

    std::size_t Variant::hash() const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        return 0;
    }

    inline std::string Variant::to_string() const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        return "";
    }
}
