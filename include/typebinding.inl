namespace SimpleRTTR
{
    //using a hackish way of extracting the offset by using a union
    template<typename ClassType>
    union PointerOffsetUnion
    {
        ClassType memberPtr;
        std::size_t offset;
    };

    template<typename ClassType, typename MemberType>
    typename std::enable_if<std::is_class<ClassType>::value == true, std::size_t>::type
        OffsetHelper(MemberType ClassType::* memberPtr)
    {
        PointerOffsetUnion<MemberType ClassType::*> ptr;
        ptr.memberPtr = memberPtr;
        //we are technically in "undefined" territory here, but what I've noticed is that the offset is encoded in the bottom 32 bits of
        //  the member data pointer.
        return ptr.offset & 0x00000000FFFFFFFF;
    }

    //---
    // Type Binding
    TypeStorage& TypeBindingBase::get_storage()
    {
        return TypeManager::instance().get_storage();
    }

    void TypeBindingBase::register_type(const TypeData& typeData)
    {
        (void)typeData;
    }

    template<typename ClassType>
    TypeBinding<ClassType>::TypeBinding()
        :
        _TypeData(GetStorage().get_or_create_type<ClassType>())
    {
        register_type(_TypeData);
    }

    template<typename ClassType>
    TypeBinding<ClassType>::TypeBinding(TypeData& typeData)
        :
        _TypeData(typeData)
    {
        register_type(_TypeData);
    }

    template<typename ClassType>
    TypeBinding<ClassType>::~TypeBinding()
    {
    }

    template<typename ClassType, typename MemberType>
    typename std::enable_if<std::is_class<ClassType>::value == true, const Type>::type
        PropertyHelper(MemberType ClassType::* memberPtr)
    {
        return types().get_or_create_type<MemberType>();
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    TypeBinding<ClassType>& TypeBinding<ClassType>::meta(MetaKey key, MetaValue value)
    {
        class Meta meta(key, value);
        _InternalGetMetadata(_TypeData).add(meta);
        return *this;
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    TypeBinding<ClassType>& TypeBinding<ClassType>::meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        //need to copy the contents of value to vector since initializer_list only stores stack pointers
        _InternalGetMetadata(_TypeData).add(SimpleRTTR::Meta(key, std::vector<MetaValue>(value)));
        return *this;
    }


    template<typename ClassType>
    template<typename MemberType>
    PropertyBinding<ClassType> TypeBinding<ClassType>::property(MemberType memberPtr, const std::string& name)
    {
        TypeReference type = PropertyHelper<ClassType>(memberPtr);
        std::size_t offset = OffsetHelper<ClassType>(memberPtr);

        PropertyContainer& properties = _InternalGetProperties(_TypeData);
        properties.add(SimpleRTTR::Property(SimpleRTTR::PropertyData(name, type, offset)));
        return PropertyBinding<ClassType>(properties.back(), _TypeData);
    }

    //// constructors do not have function pointers like normal methods, so we need a wrapper that allows us to deduce the arguments for the constructor
    //template <typename ClassType>
    //struct ConstructorTraits;

    //template <typename ClassType, typename... TArgs>
    //struct ConstructorTraits<ClassType(TArgs...)>
    //{
    //    using Args = std::tuple<TArgs...>;
    //};

    template<typename ClassType>
    MethodBinding<ClassType> TypeBinding<ClassType>::constructor()
    {
        static_assert(std::is_constructible<ClassType>::value, "ClassType must be default constructible, or use the Constructor<...>({...}) binding with the correct arguments");

        class Method constructorData = ConstructorHelper<ClassType>();

        ConstructorContainer& constructors = _InternalGetConstructors(_TypeData);
        constructors.add(constructorData);
        class Method& method = constructors.back();

        return MethodBinding<ClassType>(method, _TypeData);
    }

    template<typename ClassType>
    template<typename... ConstructorArgs>
    MethodBinding<ClassType> TypeBinding<ClassType>::constructor(const std::initializer_list<std::string>& paramNames)
    {
        static_assert((std::is_constructible<ClassType, ConstructorArgs...>::value), "ClassType must be constructible with the arguments provided");
        SIMPLERTTR_ASSERT_MSG(sizeof...(ConstructorArgs) == paramNames.size(), "You must specify the correct number of argument names")

        class Method constructorData = ConstructorHelper<ClassType, ConstructorArgs...>(paramNames);

        ConstructorContainer& constructors = _InternalGetConstructors(_TypeData);
        constructors.add(constructorData);
        class Method& method = constructors.back();

        return MethodBinding<ClassType>(method, _TypeData);
    }

    template<typename ClassType>
    template<typename MethodType>
    MethodBinding<ClassType> TypeBinding<ClassType>::method(MethodType methodPtr, const std::string& name)
    {
        class Method methodData = MethodHelper(methodPtr, name, {});

        //if you catch this error, then that means you should be specifying the parameter names(see the function below)
        SIMPLERTTR_ASSERT(methodData.parameters().size() == 0);

        MethodContainer& methods = _InternalGetMethods(_TypeData);
        methods.add(methodData);
        class Method& method = methods.back();

        return MethodBinding<ClassType>(method, _TypeData);
    }

    template<typename ClassType>
    template<typename MethodType>
    MethodBinding<ClassType> TypeBinding<ClassType>::method(MethodType methodPtr, const std::string& name, const std::initializer_list<std::string>& paramNames)
    {
        class Method methodData = MethodHelper(methodPtr, name, paramNames);

        //if you catch this error, then that means you need an equal number of names to parameters
        SIMPLERTTR_ASSERT(methodData.parameters().size() == paramNames.size());

        MethodContainer& methods = _InternalGetMethods(_TypeData);
        methods.add(methodData);
        class Method& method = methods.back();

        return MethodBinding<ClassType>(method, _TypeData);
    }

    template<typename ClassType>
    template <typename EnumType>
    inline ValueBinding<ClassType> TypeBinding<ClassType>::value(EnumType value, const std::string& name)
    {
        ValueContainer& values = _InternalGetValues(_TypeData);
        values.add(SimpleRTTR::Value(name, value));
        return ValueBinding<ClassType>(values.back(), _TypeData);
    }


    template<typename ClassType>
    PropertyBinding<ClassType>::PropertyBinding(class Property& property, TypeData& typeData)
        :
        TypeBinding<ClassType>(typeData),
        _PropertyData(_InternalPropertyGetPropertyDataRef(property))
    {
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    inline PropertyBinding<ClassType>& PropertyBinding<ClassType>::meta(MetaKey key, MetaValue value)
    {
        _InternalPropertyDataGetMetaListRef(_PropertyData).add(SimpleRTTR::Meta(key, value));
        return *this;
    }

    template<typename ClassType>
    template<typename MetaKey, typename MetaValue>
    inline PropertyBinding<ClassType>& PropertyBinding<ClassType>::meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        //need to copy the contents of value to vector since initializer_list only stores stack pointers
        _InternalPropertyDataGetMetaListRef(_PropertyData).add(SimpleRTTR::Meta(key, std::vector<MetaValue>(value)));
        return *this;
    }

    template<typename ClassType>
    MethodBinding<ClassType>::MethodBinding(class Method& method, TypeData& typeData)
        :
        TypeBinding<ClassType>(typeData),
        _Method(method)
    {
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    inline MethodBinding<ClassType>& MethodBinding<ClassType>::meta(MetaKey key, MetaValue value)
    {
        _InternalGetMetadata(_Method).add(SimpleRTTR::Meta(key, value));
        return *this;
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    inline MethodBinding<ClassType>& MethodBinding<ClassType>::meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        //need to copy the contents of value to vector since initializer_list only stores stack pointers
        _InternalGetMetadata(_Method).add(SimpleRTTR::Meta(key, std::vector<MetaValue>(value)));
        return *this;
    }

    template<typename ClassType>
    ValueBinding<ClassType>::ValueBinding(class Value& value, TypeData& typeData)
        : TypeBinding<ClassType>(typeData),
        _Value(value)
    {
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    inline ValueBinding<ClassType>& ValueBinding<ClassType>::meta(MetaKey key, MetaValue value)
    {
        _InternalGetMetadata(_Value).add(SimpleRTTR::Meta(key, value));
        return *this;
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    inline ValueBinding<ClassType>& ValueBinding<ClassType>::meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        //need to copy the contents of value to vector since initializer_list only stores stack pointers
        _InternalGetMetadata(_Value).add(CLASS_SPECIFIER Meta(key, std::vector<MetaValue>(value)));
        return *this;
    }
}
