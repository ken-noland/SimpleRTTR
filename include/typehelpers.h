#pragma once

namespace SimpleRTTR
{
    //TODO: This whole file could use some cleaning up before posting to a live repository. It's just messy, but I did it
    //  this way to just get it up and running.
    // 
    //used as a base class for the type information. The subsequent child classes are mainly template specializations and
    //  template metaprogramming to extract the type information we need.
    class TypeHelperBase
    {
    public:
        typedef void(*QualifiedNameParseFunc)(char* name);

        using NamespaceContainer = TypeData::NamespaceContainer;
        using TemplateTypeContainer = stdrttr::vector<TypeReference>;

        typedef stdrttr::string(*ToStringFunction)(const Variant&);

        TypeHelperBase(const std::type_info& typeInfo, std::size_t size, std::type_index typeIndex, std::uint64_t flags,
            ToStringFunction toStringFunc,
            QualifiedNameParseFunc parseQualifiedName = nullptr)
            :
            _Size(size),
            _Flags(flags),
            _TypeIndex(typeIndex),
            _ToStringFunc(toStringFunc)
        {
            ParseName(typeInfo, parseQualifiedName);
        }

        inline const stdrttr::string& Name() const { return _Name; };
        inline const stdrttr::string& QualifiedName() const { return _QualifiedName; }
        inline const NamespaceContainer& Namespaces() const { return _Namespaces; }
        inline const TemplateTypeContainer& TemplateParams() const { return _TemplateParams; }
        inline const ToStringFunction& ToStringFunc() const { return _ToStringFunc; }

        inline std::size_t Size() const { return _Size; }

        inline std::uint64_t Flags() const { return _Flags; }

        inline std::type_index TypeIndex() const { return _TypeIndex; }

    protected:
        inline stdrttr::string RemoveTemplateArguments(const stdrttr::string str, std::size_t offset = 0)
        {
            //TODO: this whole thing can be done in-place instead of allocating new strings all the time.
            stdrttr::string retString(str);
            std::size_t startPos = retString.npos;
            for (std::size_t i = offset; i < retString.length(); i++)
            {
                if (retString[i] == '<')
                {
                    if (startPos != retString.npos)
                    {
                        //need to recursively check
                        retString = RemoveTemplateArguments(retString, i);
                    }
                    else
                    {
                        startPos = i;
                    }
                }
                else if (retString[i] == '>')
                {
                    retString = retString.erase(startPos, i - startPos + 1);
                    return retString;
                }
            }

            return retString;
        }

        inline stdrttr::string ParseFullyQualifiedName(const char* typeName, QualifiedNameParseFunc parseQualifiedName)
        {
#           if defined(__GNUC__) || defined(__GNUG__)
            int status = -1;
            std::unique_ptr<char, void(*)(void*)> res {
                abi::__cxa_demangle(typeName, NULL, NULL, &status),
                std::free
            };

            const char* realTypeName = (status == 0) ? res.get() : typeName;
#           else
            const char* realTypeName = typeName;
#           endif

            std::size_t slen = strlen(realTypeName);
            char* charQualifiedName = static_cast<char*>(alloca(slen + 1));
            memcpy(charQualifiedName, realTypeName, slen + 1);

            if (parseQualifiedName != nullptr)
            {
                parseQualifiedName(charQualifiedName);
            }

            return stdrttr::string(charQualifiedName);
        }

        inline void ParseName(const std::type_info& typeInfo, QualifiedNameParseFunc parseQualifiedName)
        {
            stdrttr::string qualifiedName = ParseFullyQualifiedName(typeInfo.name(), parseQualifiedName);
            _QualifiedName = trim(qualifiedName);

            stdrttr::string name = _QualifiedName;
#       if defined(_MSC_VER)
            //remove the preceding "class " bit
            if (name.find("class ", 0) == 0)
            {
                name = _QualifiedName.substr(6);
            }

            //remove the preceding "struct " bit
            if (name.find("struct ", 0) == 0)
            {
                name = name.substr(7);
            }

            //remove the preceding "enum " bit
            if (name.find("enum ", 0) == 0)
            {
                name = name.substr(5);
            }
#       endif

            name = RemoveTemplateArguments(name);

#       if defined(_MSC_VER)
            //remove the appended " __ptr64" from "void * __ptr64"
            if (name.find(" __ptr64", name.length() - 8) != name.npos)
            {
                name = name.substr(0, name.length() - 8);
            }
#       endif

            size_t index = 0;
            while ((index = name.find("::")) != name.npos)
            {
                _Namespaces.push_back(name.substr(0, index));
                name = name.substr(index + 2);
            }

#       if defined(_MSC_VER)
            //TODO: remove the space between the type and the "*" for MSVC compiler
#       endif

            _Name = trim(name);
        }


        // Helper to check if a type is iterable
        template <typename T>
        class IsIterableHelper {
        private:
            template <typename U>
            static auto test(int) -> decltype(std::begin(std::declval<U&>()) != std::end(std::declval<U&>()), std::true_type{});

            template <typename U>
            static std::false_type test(...);

        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };

        template <typename T>
        constexpr uint64_t ExtractTypeFlags() {
            uint64_t flags = (uint64_t)TypeFlag::None;

            if (std::is_empty_v<T>) flags |= (uint64_t)TypeFlag::IsEmpty;
            if (std::is_void_v<T>) flags |= (uint64_t)TypeFlag::IsVoid;
            if (std::is_null_pointer_v<T>) flags |= (uint64_t)TypeFlag::IsNullPointer;
            if (std::is_integral_v<T>) flags |= (uint64_t)TypeFlag::IsIntegral;
            if (std::is_floating_point_v<T>) flags |= (uint64_t)TypeFlag::IsFloatingPoint;
            if (std::is_array_v<T>) flags |= (uint64_t)TypeFlag::IsArray;
            if (std::is_enum_v<T>) flags |= (uint64_t)TypeFlag::IsEnum;
            if (std::is_union_v<T>) flags |= (uint64_t)TypeFlag::IsUnion;
            if (std::is_class_v<T>) flags |= (uint64_t)TypeFlag::IsClass;
            if (std::is_function_v<T>) flags |= (uint64_t)TypeFlag::IsFunction;
            if (std::is_pointer_v<T>) flags |= (uint64_t)TypeFlag::IsPointer;
            if (std::is_reference_v<T>) flags |= (uint64_t)TypeFlag::IsReference;
            if (std::is_member_pointer_v<T>) flags |= (uint64_t)TypeFlag::IsMemberPointer;
            if (std::is_member_object_pointer_v<T>) flags |= (uint64_t)TypeFlag::IsMemberObject;
            if (std::is_member_function_pointer_v<T>) flags |= (uint64_t)TypeFlag::IsMemberFunction;
            if (std::is_trivially_constructible_v<T>) flags |= (uint64_t)TypeFlag::IsTriviallyConstructible;
            if (std::is_trivially_copyable_v<T>) flags |= (uint64_t)TypeFlag::IsTriviallyCopyable;
//            if (std::is_trivially_assignable_v<T>) flags |= (uint64_t)TypeFlag::IsTriviallyAssignable;
            if (std::is_nothrow_constructible_v<T>) flags |= (uint64_t)TypeFlag::IsNoThrowConstructible;
//            if (std::is_nothrow_copyable_v<T>) flags |= (uint64_t)TypeFlag::IsNoThrowCopyable;
//            if (std::is_nothrow_assignable_v<T>) flags |= (uint64_t)TypeFlag::IsNoThrowAssignable;
            if (std::is_move_constructible_v<T>) flags |= (uint64_t)TypeFlag::IsMoveConstructible;
            if (std::is_move_assignable_v<T>) flags |= (uint64_t)TypeFlag::IsMoveAssignable;
            if (std::is_destructible_v<T>) flags |= (uint64_t)TypeFlag::IsDestructible;

            // Check if the type is iterable (has begin() and end())
            if (IsIterableHelper<T>::value) flags |= (uint64_t)TypeFlag::IsIterable;

            if (std::is_const_v<T>) flags |= (uint64_t)TypeFlag::IsConst;
            if (std::is_volatile_v<T>) flags |= (uint64_t)TypeFlag::IsVolatile;
            if (std::is_trivial_v<T>) flags |= (uint64_t)TypeFlag::IsTrivial;
            if (std::is_polymorphic_v<T>) flags |= (uint64_t)TypeFlag::IsPolymorphic;
            if (std::is_standard_layout_v<T>) flags |= (uint64_t)TypeFlag::IsStandardLayout;
            if (std::is_pod_v<T>) flags |= (uint64_t)TypeFlag::IsPOD;
            if (std::is_aggregate_v<T>) flags |= (uint64_t)TypeFlag::IsAggregate;
//            if (std::is_literal_type_v<T>) flags |= (uint64_t)TypeFlag::IsLiteral;
            if (std::is_signed_v<T>) flags |= (uint64_t)TypeFlag::IsSigned;
            if (std::is_unsigned_v<T>) flags |= (uint64_t)TypeFlag::IsUnsigned;
            if (std::is_arithmetic_v<T>) flags |= (uint64_t)TypeFlag::IsArithmetic;
            if (std::is_fundamental_v<T>) flags |= (uint64_t)TypeFlag::IsFundamental;
            if (std::is_object_v<T>) flags |= (uint64_t)TypeFlag::IsObject;
            if (std::is_scalar_v<T>) flags |= (uint64_t)TypeFlag::IsScalar;
            if (std::is_compound_v<T>) flags |= (uint64_t)TypeFlag::IsCompound;

            if (std::is_abstract_v<T>) flags |= (uint64_t)TypeFlag::IsAbstract;
            if (std::is_final_v<T>) flags |= (uint64_t)TypeFlag::IsFinal;

            return flags;
        }

        stdrttr::string         _Name;
        stdrttr::string         _TypeID;
        stdrttr::string         _QualifiedName;
        NamespaceContainer      _Namespaces;
        TemplateTypeContainer   _TemplateParams;
        std::size_t             _Size;
        std::type_index         _TypeIndex;

        std::uint64_t           _Flags;

        ToStringFunction        _ToStringFunc;
    };

    template<typename ClassType>
    inline void TemplateParameterHelper(TypeHelperBase::TemplateTypeContainer& outParams);

    template<typename ClassType, typename... TemplateArgs >
    inline typename std::enable_if<sizeof...(TemplateArgs) != 0, void>::type
        TemplateParameterHelper(TypeHelperBase::TemplateTypeContainer& outParams);

    class TypeHelper1
    {
    protected:
        static void ParseQualifiedName(char* name)
        {
#       if defined(__clang__)
#       elif defined(__GNUC__) || defined(__GNUG__)
            // For GCC compilers, after the name is demangled, the typeid looks
            // like this
            // "SimpleRTTR::TypeHelper<void>*"
            // ^ leading              ^    ^^ trailing
            const int leading = 23;
            const int trailing = 2;
#       elif defined(_MSC_VER)
            // For MSVC Compilers, typeid(this).name() returns the following
            // "class SimpleRTTR::TypeHelper<class Test> * __ptr64"
            //  ^ leading                   ^          ^ trailing^
            const int leading = 29;
            const int trailing = 11;
#       else
#       endif
            std::size_t len = strlen(name);
            memcpy(name, name + leading, len - leading);
            name[len - (leading + trailing)] = 0;
        }
    };


    template <typename ClassType>
    class TypeHelper : public TypeHelperBase, TypeHelper1
    {
    public:
        TypeHelper() : TypeHelperBase(typeid(this), sizeof(ClassType), typeid(ClassType), std::is_enum_v<ClassType>,
            (TypeHelperBase::ToStringFunction)&VariantToString<ClassType>,
            (TypeHelperBase::QualifiedNameParseFunc)&ParseQualifiedName)
        {
            static_assert(sizeof(ClassType) > 0, "Classes must be fully declared before extracting the type information");
        }
    };

    template <>
    class TypeHelper<void> : public TypeHelperBase, TypeHelper1
    {
    public:
        TypeHelper<void>() : TypeHelperBase(typeid(this), 0, typeid(void), (uint64_t)ExtractTypeFlags<void>(),
            (ToStringFunction)&VariantToString<void>,
            &ParseQualifiedName) {}
    };

    template <template <typename... > class Tmpl, typename ...Args>
    class TypeHelper<Tmpl<Args...>> : public TypeHelperBase, TypeHelper1
    {
    public:
        TypeHelper()
            :
            TypeHelperBase(typeid(this), sizeof(Tmpl<Args...>), typeid(Tmpl<Args...>), (uint64_t)ExtractTypeFlags<Tmpl<Args...>>(),
                (TypeHelperBase::ToStringFunction)&VariantToString<Tmpl, Args...>,
                (TypeHelperBase::QualifiedNameParseFunc)ParseQualifiedName)
        {
            TemplateParameterHelper<Args...>(_TemplateParams);
        }
    };

    template<typename ClassType>
    inline void TemplateParameterHelper(TypeHelperBase::TemplateTypeContainer& outParams)
    {
        //it's worth noting that this creates a pointer from a reference to a unique_ptr buried in 
        //  the TypeStorage vector... not something that should EVER be done under normal circumstances
        outParams.push_back(TypeReference(Types().GetOrCreateType<ClassType>()));
    };

    template<typename ClassType, typename... TemplateArgs >
    inline typename std::enable_if<sizeof...(TemplateArgs) != 0, void>::type
        TemplateParameterHelper(TypeHelperBase::TemplateTypeContainer& outParams)
    {
        TemplateParameterHelper<ClassType>(outParams);
        TemplateParameterHelper<TemplateArgs...>(outParams);
    };

    template<typename ParameterType>
    inline void ParameterHelper(ParameterContainer& outTypes, std::initializer_list<stdrttr::string>::const_iterator nameIter, std::initializer_list<stdrttr::string>::const_iterator endIter)
    {
        SIMPLERTTR_ASSERT_MSG(nameIter != endIter, "Not enough names specified in the method declaration. You must have a name for each function parameter.");

        Parameter param(*nameIter, Types().GetOrCreateType<ParameterType>());
        outTypes.Add(param);
    }

    template<typename ClassType, typename... Parameters>
    typename std::enable_if<sizeof...(Parameters) != 0, void>::type
        ParameterHelper(ParameterContainer& outTypes, std::initializer_list<stdrttr::string>::const_iterator nameIter, std::initializer_list<stdrttr::string>::const_iterator endIter)
    {
        ParameterHelper<ClassType>(outTypes, nameIter, endIter);
        ParameterHelper<Parameters...>(outTypes, ++nameIter, endIter);
    }

    template<typename RetType, typename ClassType, class... ParameterTypes>
    inline Method MethodHelper(RetType(ClassType::*)(ParameterTypes...), const stdrttr::string& name, const std::initializer_list<stdrttr::string>& paramNames)
    {
        using NameIter = std::initializer_list<stdrttr::string>::const_iterator;
        NameIter paramNameIter = paramNames.begin();
        NameIter paramNameEnd = paramNames.end();

        Type returnType = Types().GetOrCreateType<RetType>();
        ParameterContainer params;
        ParameterHelper<ParameterTypes...>(params, paramNameIter, paramNameEnd);

        return Method(name, returnType, params);
    }

    template<typename RetType, typename ClassType>
    inline Method MethodHelper(RetType(ClassType::*)(void), const stdrttr::string& name, const std::initializer_list<stdrttr::string>& paramNames)
    {
        SIMPLERTTR_ASSERT_MSG(paramNames.size() == 0, "No need to specify parameters names on method that have 0 parameters");

        Type returnType = Types().GetOrCreateType<RetType>();
        return Method(name, returnType, {});
    }

    template<typename ClassType>
    inline Method ConstructorHelper()
    {
        ParameterContainer params;
        return Method("Constructor", Types().GetOrCreateType<ClassType>(), params);
    }

    template<typename ClassType, typename... ConstructorArgs>
    inline Method ConstructorHelper(const std::initializer_list<stdrttr::string>& paramNames)
    {
        using NameIter = std::initializer_list<stdrttr::string>::const_iterator;
        NameIter paramNameIter = paramNames.begin();
        NameIter paramNameEnd = paramNames.end();

        ParameterContainer params;
        ParameterHelper<ConstructorArgs...>(params, paramNameIter, paramNameEnd);

        return Method("Constructor", Types().GetOrCreateType<ClassType>(), params);
    }
}
