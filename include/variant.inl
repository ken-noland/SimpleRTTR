namespace SimpleRTTR
{
    namespace
    {

        template<typename VariantType>
        struct CompareInternal
        {
            static inline bool Compare(const std::any& lhs, const std::any& rhs);
        };

        template<typename VariantType>
        bool CompareInternal<VariantType>::Compare(const std::any& lhs, const std::any& rhs)
        {
            VariantType var1 = std::any_cast<VariantType>(lhs);
            VariantType var2 = std::any_cast<VariantType>(rhs);

            if constexpr (
                std::is_integral<VariantType>::value ||
                std::is_pointer_v<VariantType> ||
                std::is_member_function_pointer_v<VariantType>
                )
            {
                return var1 == var2;
            }

            SIMPLERTTR_ASSERT_MSG(false, "Unable to determine equality operator for non-integral types")
        }

        template<>
        bool CompareInternal<const char*>::Compare(const std::any& lhs, const std::any& rhs)
        {
            const char* var1 = std::any_cast<const char*>(lhs);
            const char* var2 = std::any_cast<const char*>(rhs);

            return var1 == var2 || std::strcmp(var1, var2) == 0;
        }

        template<>
        bool CompareInternal<std::nullptr_t>::Compare(const std::any& lhs, const std::any& rhs)
        {
            (void)lhs;
            return std::type_index(rhs.type()) == std::type_index(typeid(std::nullptr_t));
        }

        //find enabled(untainted) versions of the std::hash specialization
        template<typename T, typename = void>
        struct HasValidStdHash : std::false_type {};

        template<typename T>
        struct HasValidStdHash < T, std::void_t<decltype(std::hash<T>{}(std::declval<T>())) >> : std::true_type {};


        template <typename ClassType>
        struct HashInternal
        {
            static inline std::size_t Hash(const std::any& object) 
            { 
                if constexpr (HasValidStdHash<ClassType>::value)
                {
                    return std::hash<ClassType>()(std::any_cast<ClassType>(object));
                }

                SIMPLERTTR_ASSERT_MSG(false, "Unable to determine hash function"); 
                return 0; 
            };
        };

        // Generic copy function for any type
        template<typename T>
        void GenericCopy(Variant::StorageType& destination, const Variant::StorageType& source)
        {
            new (&destination) T(*reinterpret_cast<const T*>(&source));
        }

        // Generic delete function for any type
        template<typename T>
        void GenericDelete(Variant::StorageType& storage)
        {
            reinterpret_cast<T*>(&storage)->~T();
        }

        // Generic equality function for any type
        template<typename T>
        bool GenericEqual(const Variant::StorageType& lhs, const Variant::StorageType& rhs)
        {
            if constexpr(
                std::is_integral<T>::value ||
                std::is_pointer_v<T> ||
                std::is_member_function_pointer_v<T> ||
                std::is_null_pointer_v<T>
                )
            {
                return *reinterpret_cast<const T*>(&lhs) == *reinterpret_cast<const T*>(&rhs);
            }

            return false;
        }

        // Specialization for const char*
        template<>
        bool GenericEqual<const char*>(const Variant::StorageType& lhs, const Variant::StorageType& rhs)
        {
            const char* lhsStr = *reinterpret_cast<const char* const*>(&lhs);
            const char* rhsStr = *reinterpret_cast<const char* const*>(&rhs);

            return std::strcmp(lhsStr, rhsStr) == 0;
        }

        // Void copy
        void VoidCopy(Variant::StorageType& destination, const Variant::StorageType& source)
        {
            // for void ponters, we don't need to worry about copy constructors, so just raw copy the memory
            std::memcpy(&destination, &source, sizeof(Variant::StorageType));
        }

        // Void delete
        void VoidDelete(Variant::StorageType& storage)
        {
            // for void ponters, we don't manage the pointer memory or worry about their destructors, so ignore
        }


        // Copy function template
        template<typename T>
        void CopyInternal(Variant::StorageType& storage, const T& value)
        {
            new (&storage) T(value);
        }

        // Move function template
        template<typename T>
        void MoveInternal(Variant::StorageType& storage, T&& value)
        {
            new (&storage) T(std::move(value));
        }

        // Destroy function template
        template<typename T>
        void DestroyInternal(Variant::StorageType& storage)
        {
            reinterpret_cast<T*>(&storage)->~T();
        }
    }

    template<typename VariantType>
    Variant::Variant(VariantType value)
        :
        _typeIndex(typeid(VariantType)),
        _copyFunc(&GenericCopy<VariantType>),
        _deleteFunc(&GenericDelete<VariantType>),
        _equalityFunc(&GenericEqual<VariantType>)
    {
        static_assert(std::is_copy_constructible<VariantType>::value, "VariantType must be copy constructible");
        static_assert(sizeof(_storage) >= sizeof(VariantType), "Storage is not large enough for VariantType");

        CopyInternal<VariantType>(_storage, value);
    }

    // Constructor for void pointers
    Variant::Variant(void* valuePtr, SimpleRTTR::Type type)
        :
        _typeIndex(type.GetTypeIndex()),
        _copyFunc(&VoidCopy),
        _deleteFunc(&VoidDelete),
        _equalityFunc(&GenericEqual<void*>)
    {
        static_assert(sizeof(_storage) >= sizeof(void*), "Storage is not large enough for void*");

        std::memcpy(&_storage, &valuePtr, sizeof(void*));
    }

    // Copy constructor
    Variant::Variant(const Variant& var)
        : 
        _typeIndex(var._typeIndex),
        _copyFunc(var._copyFunc),
        _deleteFunc(var._deleteFunc),
        _equalityFunc(var._equalityFunc)
    {
        if(_typeIndex != typeid(void))
        {
            var._copyFunc(_storage, var._storage);
        }
    }

    // Move constructor
    Variant::Variant(Variant&& var)
        : 
        _typeIndex(var._typeIndex),
        _copyFunc(var._copyFunc),
        _deleteFunc(var._deleteFunc),
        _equalityFunc(var._equalityFunc)
    {
        if(_typeIndex != typeid(void))
        {
            var._copyFunc(_storage, var._storage);
            var.Destroy();
        }
    }

    // Destructor
    Variant::~Variant()
    {
        Destroy();
    }

    // Copy assignment
    Variant& Variant::operator=(const Variant& var)
    {
        if(this != &var)
        {
            Destroy();
            _typeIndex = var._typeIndex;
            _copyFunc = var._copyFunc;
            _deleteFunc = var._deleteFunc;
            if(_typeIndex != typeid(void))
            {
                _copyFunc(_storage, var._storage);
            }
        }
        return *this;
    }

    bool Variant::operator==(const Variant& var) const
    {
        // Ensure that the types are the same before comparing
        if(_typeIndex == var._typeIndex && _equalityFunc)
        {
            return _equalityFunc(_storage, var._storage);
        }
        return false;
    }

    bool Variant::operator!=(const Variant& var) const
    {
        return false;
    }

    template<typename ObjectType>
    bool Variant::operator==(const ObjectType& var) const
    {
        if(_typeIndex == typeid(ObjectType) && _equalityFunc)
        {
            return _equalityFunc(_storage, *reinterpret_cast<const StorageType*>(&var));
        }
        return false;
    }

    void Variant::Destroy()
    {
        if(_typeIndex != typeid(void))
        {
            // Use the stored delete function to destroy the object
            _deleteFunc(_storage);
            _typeIndex = std::type_index(typeid(void));
        }
    }

    template<typename VariantType>
    VariantType Variant::GetAs() const
    {
        // 1. Handle direct type match (if type matches exactly, return it)
        if(_typeIndex == std::type_index(typeid(VariantType)))
        {
            return *reinterpret_cast<const VariantType*>(&_storage);
        }

        // 2. Handle basic type conversion (char, short, int, etc.)
        if constexpr(std::is_arithmetic_v<VariantType>)
        {
            if constexpr(std::is_arithmetic_v<std::decay_t<decltype(*reinterpret_cast<const VariantType*>(&_storage))>>)
            {
                // Convert between basic arithmetic types
                return static_cast<VariantType>(*reinterpret_cast<const std::decay_t<decltype(*reinterpret_cast<const VariantType*>(&_storage))>*>(&_storage));
            }
        }

        // 3. Handle enum to int and int to enum conversion
        const SimpleRTTR::Type& storedType = Types().GetType(_typeIndex.name());
        const SimpleRTTR::Type& targetType = Types().GetType<VariantType>();

        // Check if the stored type is an enum and the target is fundamental
        if constexpr(std::is_fundamental_v<VariantType>)
        {
            if(storedType.HasFlag(TypeFlag::IsEnum))
            {
                // Cast the enum to its underlying integer type
                return static_cast<VariantType>(*reinterpret_cast<const int*>(&_storage));
            }
        }

        // Check if the stored type is fundamental (e.g., int) and the target type is an enum
        if constexpr(std::is_enum_v<VariantType>)
        {
            // Cast the int to an enum
            return static_cast<VariantType>(*reinterpret_cast<const int*>(&_storage));
        }

        // 4. Handle pointer to pointer conversion(checking inheritance)
        // TODO!

        throw std::bad_cast();
    }


    // Specializations for char* <-> std::string and wchar_t* <-> std::wstring conversions
    template<>
    inline std::string Variant::GetAs<std::string>() const
    {
        if(_typeIndex == std::type_index(typeid(const char*)))
        {
            return std::string(*reinterpret_cast<const char* const*>(&_storage));
        }
        else if(_typeIndex == std::type_index(typeid(std::string)))
        {
            return *reinterpret_cast<const std::string*>(&_storage);
        }

        throw std::bad_cast();
    }

    template<>
    inline const char* Variant::GetAs<const char*>() const
    {
        if(_typeIndex == std::type_index(typeid(std::string)))
        {
            return reinterpret_cast<const std::string*>(&_storage)->c_str();
        }
        else if(_typeIndex == std::type_index(typeid(const char*)))
        {
            return *reinterpret_cast<const char* const*>(&_storage);
        }

        throw std::bad_cast();
    }

    template<>
    inline std::wstring Variant::GetAs<std::wstring>() const
    {
        if(_typeIndex == std::type_index(typeid(const wchar_t*)))
        {
            return std::wstring(*reinterpret_cast<const wchar_t* const*>(&_storage));
        }
        else if(_typeIndex == std::type_index(typeid(std::wstring)))
        {
            return *reinterpret_cast<const std::wstring*>(&_storage);
        }

        throw std::bad_cast();
    }

    template<>
    inline const wchar_t* Variant::GetAs<const wchar_t*>() const
    {
        if(_typeIndex == std::type_index(typeid(std::wstring)))
        {
            return reinterpret_cast<const std::wstring*>(&_storage)->c_str();
        }
        else if(_typeIndex == std::type_index(typeid(const wchar_t*)))
        {
            return *reinterpret_cast<const wchar_t* const*>(&_storage);
        }

        throw std::bad_cast();
    }

    inline void Variant::CopyTo(void* destination) const
    {
        _copyFunc(*reinterpret_cast<StorageType*>(destination), _storage);
    }

    inline const class TypeReference Variant::Type() const
    {
        return TypeReference(Types().GetType(_typeIndex.name()));
    }

    std::size_t Variant::Hash() const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        return 0;
    }

    inline stdrttr::string Variant::ToString() const
    {
        SIMPLERTTR_ASSERT(!"Not implemented yet");
        return "";
    }
}
