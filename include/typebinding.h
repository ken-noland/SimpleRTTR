#pragma once

namespace SimpleRTTR
{
    class TypeBindingBase
    {
    protected:
        //friendly accessors for protected function
        inline TypeStorage& GetStorage();
        inline void RegisterType(const TypeData& typeData);
    };

    template<typename ClassType>
    class PropertyBinding;

    template<typename ClassType>
    class MethodBinding;

    template<typename ClassType>
    class ValueBinding;

    template<typename ClassType>
    class TypeBinding : public TypeBindingBase
    {
    public:
        inline TypeBinding(TypeData& typeData);
        inline ~TypeBinding();

        template <typename MetaKey, typename MetaValue>
        inline TypeBinding& Meta(MetaKey key, MetaValue value);

        template <typename MetaKey, typename MetaValue>
        inline TypeBinding& Meta(MetaKey key, const std::initializer_list<MetaValue>& value);

        template <typename MemberType>
        inline PropertyBinding<ClassType> Property(MemberType memberPtr, const stdrttr::string& name);

        template <typename MethodType>
        inline MethodBinding<ClassType> Method(MethodType methodPtr, const stdrttr::string& name);

        template <typename MethodType>
        inline MethodBinding<ClassType> Method(MethodType methodPtr, const stdrttr::string& name, const std::initializer_list<stdrttr::string>& paramNames);

        template <typename EnumType>
        inline ValueBinding<ClassType> Value(EnumType value, const stdrttr::string& name);

    protected:
        inline TypeBinding();

        TypeData& _TypeData;
    };

    template<typename ClassType>
    class PropertyBinding : public TypeBinding<ClassType>
    {
    public:
        inline PropertyBinding(class Property& property, TypeData& typeData);

        template <typename MetaKey, typename MetaValue>
        inline PropertyBinding& Meta(MetaKey key, MetaValue value);

        template <typename MetaKey, typename MetaValue>
        inline PropertyBinding& Meta(MetaKey key, const std::initializer_list<MetaValue>& value);

    protected:
        class PropertyData& _PropertyData;
    };

    template<typename ClassType>
    class MethodBinding : public TypeBinding<ClassType>
    {
    public:
        inline MethodBinding(class Method& method, TypeData& typeData);

        template <typename MetaKey, typename MetaValue>
        inline MethodBinding& Meta(MetaKey key, MetaValue value);

        template <typename MetaKey, typename MetaValue>
        inline MethodBinding& Meta(MetaKey key, const std::initializer_list<MetaValue>& value);

    protected:
        class Method& _Method;
    };

    template<typename ClassType>
    class ValueBinding : public TypeBinding<ClassType>
    {
    public:
        inline ValueBinding(class Value& value, TypeData& typeData);

        template <typename... MetaType>
        inline ValueBinding& Meta(MetaType...);

        template <typename MetaKey, typename MetaValue>
        inline ValueBinding& Meta(MetaKey key, const std::initializer_list<MetaValue>& value);

    protected:
        class Value& _Value;
    };
}
