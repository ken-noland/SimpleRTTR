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
        _TypeData.Metadata.push_back(meta);
        return *this;
    }

    template<typename ClassType>
    template <typename MetaKey, typename MetaValue>
    TypeBinding<ClassType>& TypeBinding<ClassType>::Meta(MetaKey key, const std::initializer_list<MetaValue>& value)
    {
        //need to copy the contents of value to vector since initializer_list only stores stack pointers
        class Meta meta(key, stdrttr::vector<MetaValue>(value));
        _TypeData.Metadata.push_back(meta);
        return *this;
    }


    template<typename ClassType>
    template<typename MemberType>
    PropertyBinding<ClassType> TypeBinding<ClassType>::Property(MemberType memberPtr, const stdrttr::string& name)
    {
        const Type& type = PropertyHelper<ClassType>(memberPtr);
        std::size_t offset = OffsetHelper<ClassType>(memberPtr);

        //constexpr bool isConst = std::is_const<MemberType>();
        //constexpr bool isPointer = std::is_pointer<MemberType>();

        //PropertyData::PropertyFlagsBits flags;

        PropertyData propData(name, type.GetFullyQualifiedName(), offset);
        class Property prop(propData);

        //check to see if property exists
        TypeData::PropertyList::iterator iter = std::find_if(_TypeData.Properties.begin(), _TypeData.Properties.end(), [&prop](const class Property& existing)
            {
                if (existing.Offset() == prop.Offset() &&
                    existing.Name() == prop.Name() &&
                    existing.Type() == prop.Type()) {
                    return true;
                }
                return false;
            });
        
        if (iter == _TypeData.Properties.end()) 
        {
            //add it to the list if it doesn't exist already
            _TypeData.Properties.push_back(prop);
            return PropertyBinding<ClassType>(_TypeData.Properties.back(), _TypeData);
        }
        else
        {
            return PropertyBinding<ClassType>(*iter, _TypeData);
        }


        
    }

    template<typename ClassType>
    template<typename MethodType>
    MethodBinding<ClassType> TypeBinding<ClassType>::Method(MethodType methodPtr, const stdrttr::string& name)
    {
        class Method method = MethodHelper(methodPtr, name);

        //if you catch this error, then that means you should be specifying the parameter names(see the function below)
        SIMPLERTTR_ASSERT(method.Parameters().size() == 0);

        _TypeData.Methods.push_back(method);
        return MethodBinding<ClassType>(_TypeData.Methods.back(), _TypeData);
    }

    template<typename ClassType>
    template<typename MethodType>
    MethodBinding<ClassType> TypeBinding<ClassType>::Method(MethodType methodPtr, const stdrttr::string& name, const std::initializer_list<stdrttr::string>& paramNames)
    {
        class Method method = MethodHelper(methodPtr, name);

        //if you catch this error, then that means you need an equal number of names to parameters
        SIMPLERTTR_ASSERT(method.Parameters().size() == paramNames.size());

        _TypeData.Methods.push_back(method);
        return MethodBinding<ClassType>(_TypeData.Methods.back(), _TypeData);
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
