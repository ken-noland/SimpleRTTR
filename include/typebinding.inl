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
//        TypeManager::GetInstance().RegisterType(typeData);
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
        _TypeData.AddMetadata(meta);
        return *this;
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    TypeBinding<ClassType>& TypeBinding<ClassType>::Meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        //need to copy the contents of value to vector since initializer_list only stores stack pointers
        class Meta meta(key, stdrttr::vector<MetaValue>(value));
        _TypeData.AddMetadata(meta);
        return *this;
    }


    template<typename ClassType>
    template<typename MemberType>
    PropertyBinding<ClassType> TypeBinding<ClassType>::Property(MemberType memberPtr, const stdrttr::string& name)
    {
        Type type = PropertyHelper<ClassType>(memberPtr);
        std::size_t offset = OffsetHelper<ClassType>(memberPtr);

        class Property& prop = _TypeData.GetOrCreateProperty(name, type, offset);
        return PropertyBinding<ClassType>(prop, _TypeData);
    }

    template<typename ClassType>
    template<typename MethodType>
    MethodBinding<ClassType> TypeBinding<ClassType>::Method(MethodType methodPtr, const stdrttr::string& name)
    {
        class Method methodData = MethodHelper(methodPtr, name, {});

        //if you catch this error, then that means you should be specifying the parameter names(see the function below)
        SIMPLERTTR_ASSERT(methodData.Parameters().size() == 0);

        class Method& method = _TypeData.GetOrCreateMethod(methodData);
        return MethodBinding<ClassType>(method, _TypeData);
    }

    template<typename ClassType>
    template<typename MethodType>
    MethodBinding<ClassType> TypeBinding<ClassType>::Method(MethodType methodPtr, const stdrttr::string& name, const std::initializer_list<stdrttr::string>& paramNames)
    {
        class Method methodData = MethodHelper(methodPtr, name, paramNames);

        //if you catch this error, then that means you need an equal number of names to parameters
        SIMPLERTTR_ASSERT(methodData.Parameters().size() == paramNames.size());

        class Method& method = _TypeData.GetOrCreateMethod(methodData);
        return MethodBinding<ClassType>(method, _TypeData);
    }

    template<typename ClassType>
    template <typename EnumType>
    inline ValueBinding<ClassType> TypeBinding<ClassType>::Value(EnumType value, const stdrttr::string& name)
    {
        class Value valueData;
        return ValueBinding<ClassType>(valueData, _TypeData);
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
        class Meta meta(key, value);
        PropertyData::MetaList& propertyDataMetaList = _InternalPropertyDataGetMetaListRef(_PropertyData);
        propertyDataMetaList.push_back(meta);
        return *this;
    }

    template<typename ClassType>
    template<typename MetaKey, typename MetaValue>
    inline PropertyBinding<ClassType>& PropertyBinding<ClassType>::Meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        //need to copy the contents of value to vector since initializer_list only stores stack pointers
        class Meta meta(key, stdrttr::vector<MetaValue>(value));
        PropertyData::MetaList& propertyDataMetaList = _InternalPropertyDataGetMetaListRef(_PropertyData);
        propertyDataMetaList.push_back(meta);
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
    ValueBinding<ClassType>::ValueBinding(class Value& value, TypeData& typeData)
        :
        TypeBinding(typeData),
        _Value(value)
    {

    }

    template<typename ClassType>
    template <typename... MetaType>
    inline ValueBinding<ClassType>& ValueBinding<ClassType>::Meta(MetaType...)
    {
        return *this;
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    inline ValueBinding<ClassType>& ValueBinding<ClassType>::Meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        return *this;
    }

}
