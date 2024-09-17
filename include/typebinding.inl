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
    TypeStorage& TypeBindingBase::GetStorage()
    {
        return TypeManager::GetInstance().GetStorage();
    }

    void TypeBindingBase::RegisterType(const TypeData& typeData)
    {
        (void)typeData;
    }

    template<typename ClassType>
    TypeBinding<ClassType>::TypeBinding()
        :
        _TypeData(GetStorage().GetOrCreateType<ClassType>())
    {
        RegisterType(_TypeData);
    }

    template<typename ClassType>
    TypeBinding<ClassType>::TypeBinding(TypeData& typeData)
        :
        _TypeData(typeData)
    {
        RegisterType(_TypeData);
    }

    template<typename ClassType>
    TypeBinding<ClassType>::~TypeBinding()
    {
    }

    template<typename ClassType, typename MemberType>
    typename std::enable_if<std::is_class<ClassType>::value == true, const Type>::type
        PropertyHelper(MemberType ClassType::* memberPtr)
    {
        return Types().GetOrCreateType<MemberType>();
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    TypeBinding<ClassType>& TypeBinding<ClassType>::Meta(MetaKey key, MetaValue value)
    {
        class Meta meta(key, value);
        _InternalGetMetadata(_TypeData).Add(meta);
        return *this;
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    TypeBinding<ClassType>& TypeBinding<ClassType>::Meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        //need to copy the contents of value to vector since initializer_list only stores stack pointers
        _InternalGetMetadata(_TypeData).Add(CLASS_SPECIFIER Meta(key, stdrttr::vector<MetaValue>(value)));
        return *this;
    }


    template<typename ClassType>
    template<typename MemberType>
    PropertyBinding<ClassType> TypeBinding<ClassType>::Property(MemberType memberPtr, const stdrttr::string& name)
    {
        TypeReference type = PropertyHelper<ClassType>(memberPtr);
        std::size_t offset = OffsetHelper<ClassType>(memberPtr);

        PropertyContainer& properties = _InternalGetProperties(_TypeData);
	properties.Add(SimpleRTTR::Property(CLASS_SPECIFIER PropertyData(name, type, offset)));
        return PropertyBinding<ClassType>(properties.Back(), _TypeData);
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
    MethodBinding<ClassType> TypeBinding<ClassType>::Constructor()
    {
        static_assert(std::is_constructible<ClassType>::value, "ClassType must be default constructible, or use the Constructor<...>({...}) binding with the correct arguments");

        class Method constructorData = ConstructorHelper<ClassType>();

        ConstructorContainer& constructors = _InternalGetConstructors(_TypeData);
        constructors.Add(constructorData);
        class Method& method = constructors.Back();

        return MethodBinding<ClassType>(method, _TypeData);
    }

    template<typename ClassType>
    template<typename... ConstructorArgs>
    MethodBinding<ClassType> TypeBinding<ClassType>::Constructor(const std::initializer_list<stdrttr::string>& paramNames)
    {
        static_assert((std::is_constructible<ClassType, ConstructorArgs...>::value), "ClassType must be constructible with the arguments provided");
        SIMPLERTTR_ASSERT_MSG(sizeof...(ConstructorArgs) == paramNames.size(), "You must specify the correct number of argument names")

        class Method constructorData = ConstructorHelper<ClassType, ConstructorArgs...>(paramNames);

        ConstructorContainer& constructors = _InternalGetConstructors(_TypeData);
        constructors.Add(constructorData);
        class Method& method = constructors.Back();

        return MethodBinding<ClassType>(method, _TypeData);
    }

    template<typename ClassType>
    template<typename MethodType>
    MethodBinding<ClassType> TypeBinding<ClassType>::Method(MethodType methodPtr, const stdrttr::string& name)
    {
        class Method methodData = MethodHelper(methodPtr, name, {});

        //if you catch this error, then that means you should be specifying the parameter names(see the function below)
        SIMPLERTTR_ASSERT(methodData.Parameters().Size() == 0);

        MethodContainer& methods = _InternalGetMethods(_TypeData);
        methods.Add(methodData);
        class Method& method = methods.Back();

        return MethodBinding<ClassType>(method, _TypeData);
    }

    template<typename ClassType>
    template<typename MethodType>
    MethodBinding<ClassType> TypeBinding<ClassType>::Method(MethodType methodPtr, const stdrttr::string& name, const std::initializer_list<stdrttr::string>& paramNames)
    {
        class Method methodData = MethodHelper(methodPtr, name, paramNames);

        //if you catch this error, then that means you need an equal number of names to parameters
        SIMPLERTTR_ASSERT(methodData.Parameters().Size() == paramNames.size());

        MethodContainer& methods = _InternalGetMethods(_TypeData);
        methods.Add(methodData);
        class Method& method = methods.Back();

        return MethodBinding<ClassType>(method, _TypeData);
    }

    template<typename ClassType>
    template <typename EnumType>
    inline ValueBinding<ClassType> TypeBinding<ClassType>::Value(EnumType value, const stdrttr::string& name)
    {
        ValueContainer& values = _InternalGetValues(_TypeData);
        values.Add(SimpleRTTR::Value(name, value));
        return ValueBinding<ClassType>(values.Back(), _TypeData);
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
    inline PropertyBinding<ClassType>& PropertyBinding<ClassType>::Meta(MetaKey key, MetaValue value)
    {
        _InternalPropertyDataGetMetaListRef(_PropertyData).Add(SimpleRTTR::Meta(key, value));
        return *this;
    }

    template<typename ClassType>
    template<typename MetaKey, typename MetaValue>
    inline PropertyBinding<ClassType>& PropertyBinding<ClassType>::Meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        //need to copy the contents of value to vector since initializer_list only stores stack pointers
		_InternalPropertyDataGetMetaListRef(_PropertyData).Add(SimpleRTTR::Meta(key, stdrttr::vector<MetaValue>(value)));
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
    inline MethodBinding<ClassType>& MethodBinding<ClassType>::Meta(MetaKey key, MetaValue value)
    {
        _InternalGetMetadata(_Method).Add(CLASS_SPECIFIER Meta(key, value));
        return *this;
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    inline MethodBinding<ClassType>& MethodBinding<ClassType>::Meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        //need to copy the contents of value to vector since initializer_list only stores stack pointers
        _InternalGetMetadata(_Method).Add(CLASS_SPECIFIER Meta(key, stdrttr::vector<MetaValue>(value)));
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
    inline ValueBinding<ClassType>& ValueBinding<ClassType>::Meta(MetaKey key, MetaValue value)
    {
        _InternalGetMetadata(_Value).Add(CLASS_SPECIFIER Meta(key, value));
        return *this;
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    inline ValueBinding<ClassType>& ValueBinding<ClassType>::Meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        //need to copy the contents of value to vector since initializer_list only stores stack pointers
        _InternalGetMetadata(_Value).Add(CLASS_SPECIFIER Meta(key, stdrttr::vector<MetaValue>(value)));
        return *this;
    }
}
