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

        using NamespaceList = TypeData::NamespaceList;

        //we need to store the types as pointers initially, which we then convert to references later.
        using TemplateTypePtr = const Type*;
        using TemplateTypeList = stdrttr::vector<TemplateTypePtr>;

        TypeHelperBase(const std::type_info& typeInfo, std::size_t size, QualifiedNameParseFunc parseQualifiedName = nullptr)
            :
            _Size(size)
        {
            ParseName(typeInfo, parseQualifiedName);
        }

        inline const stdrttr::string& Name() const { return _Name; };
        inline const stdrttr::string& QualifiedName() const { return _QualifiedName; }
        inline const NamespaceList& Namespaces() const { return _Namespaces; }
        inline const TemplateTypeList& TemplateParams() const { return _TemplateParams; }

        inline std::size_t Size() const { return _Size; }
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
            std::size_t slen = strlen(typeName);
            char* charQualifiedName = static_cast<char*>(alloca(slen + 1));
            memcpy(charQualifiedName, typeName, slen + 1);

            if (parseQualifiedName != nullptr)
            {
                parseQualifiedName(charQualifiedName);
            }

            return stdrttr::string(charQualifiedName);
        }

        inline void ParseName(const std::type_info& typeInfo, QualifiedNameParseFunc parseQualifiedName)
        {
            _QualifiedName = trim(ParseFullyQualifiedName(typeInfo.name(), parseQualifiedName));

            stdrttr::string name;

            //remove the preceding "class " bit
            if (_QualifiedName.find("class ", 0) == 0)
            {
                name = _QualifiedName.substr(6);
            }
            else
            {
                name = _QualifiedName;
            }

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
            _Name = trim(name);
        }


        stdrttr::string  _Name;
        stdrttr::string  _TypeID;
        stdrttr::string  _QualifiedName;
        NamespaceList    _Namespaces;
        TemplateTypeList _TemplateParams;
        std::size_t      _Size;
    };

    template<typename ClassType>
    inline void TemplateParameterHelper(TypeHelperBase::TemplateTypeList& outParams);

    template<typename ClassType, typename... TemplateArgs >
    inline typename std::enable_if<sizeof...(TemplateArgs) != 0, void>::type
        TemplateParameterHelper(TypeHelperBase::TemplateTypeList& outParams);

    class TypeHelper1
    {
    protected:
        static void ParseQualifiedName(char* name)
        {
#       if defined(__clang__)
#       elif defined(__GNUC__) || defined(__GNUG__)
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
        TypeHelper() : TypeHelperBase(typeid(this), sizeof(ClassType), &ParseQualifiedName) 
        {
            static_assert(sizeof(ClassType) > 0, "Classes must be fully declared before extracting the type information");
        }
    };

    template <>
    class TypeHelper<void> : public TypeHelperBase, TypeHelper1
    {
    public:
        TypeHelper<void>() : TypeHelperBase(typeid(this), 0, &ParseQualifiedName) {}
    };

    template <template <typename... > class Tmpl, typename ...Args>
    class TypeHelper<Tmpl<Args...>> : public TypeHelperBase, TypeHelper1
    {
    public:
        TypeHelper()
            :
            TypeHelperBase(typeid(this), sizeof(Tmpl<Args...>), ParseQualifiedName)
        {
            TemplateParameterHelper<Args...>(_TemplateParams);
        }
    };

    template<typename ClassType>
    inline void TemplateParameterHelper(TypeHelperBase::TemplateTypeList& outParams)
    {
        //it's worth noting that this creates a pointer from a reference to a unique_ptr buried in 
        //  the TypeStorage vector... not something that should EVER be done under normal circumstances
        outParams.push_back(&Types().GetOrCreateType<ClassType>());
    };

    template<typename ClassType, typename... TemplateArgs >
    inline typename std::enable_if<sizeof...(TemplateArgs) != 0, void>::type
        TemplateParameterHelper(TypeHelperBase::TemplateTypeList& outParams)
    {
        TemplateParameterHelper<ClassType>(outParams);
        TemplateParameterHelper<TemplateArgs...>(outParams);
    };

    template<typename ParameterType>
    void ParameterHelper(stdrttr::vector<Parameter>& outTypes)
    {
        Parameter param(Types().GetOrCreateType<ParameterType>());
        outTypes.push_back(param);
    }

    template<typename ClassType, typename... Parameters>
    typename std::enable_if<sizeof...(Parameters) != 0, void>::type
        ParameterHelper(stdrttr::vector<Parameter>& outTypes)
    {
        ParameterHelper<ClassType>(outTypes);
        ParameterHelper<Parameters...>(outTypes);
    }

    template<typename RetType, typename ClassType, class... ParameterTypes>
    Method MethodHelper(RetType(ClassType::*)(ParameterTypes...), const stdrttr::string& name)
    {
        const Type& returnType = Types().GetOrCreateType<RetType>();
        stdrttr::vector<Parameter> params;
        ParameterHelper<ParameterTypes...>(params);

        return Method(name, returnType, params);
    }

    template<typename RetType, typename ClassType>
    Method MethodHelper(RetType(ClassType::*)(void), const stdrttr::string& name)
    {
        const Type& returnType = Types().GetOrCreateType<RetType>();
        stdrttr::vector<Parameter> params;
        return Method(name, returnType, params);
    }
}