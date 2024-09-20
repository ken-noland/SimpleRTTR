#pragma once

namespace SimpleRTTR
{
    class TypeBindingBase
    {
    protected:
        //friendly accessors for protected function
        inline TypeStorage& get_storage();
        inline void register_type(const TypeData& typeData);
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
        inline TypeBinding& meta(MetaKey key, MetaValue value);

        template <typename MetaKey, typename MetaValue>
        inline TypeBinding& meta(MetaKey key, const std::initializer_list<MetaValue>& value);

        template <typename MemberType>
        inline PropertyBinding<ClassType> property(MemberType memberPtr, const std::string& name);

        template <typename MethodType>
        inline MethodBinding<ClassType> method(MethodType methodPtr, const std::string& name);

        template <typename MethodType>
        inline MethodBinding<ClassType> method(MethodType methodPtr, const std::string& name, const std::initializer_list<std::string>& paramNames);

        inline MethodBinding<ClassType> constructor();

        // constructors are a bit annoying since they have limitations around them which prevent them from being introspected
        template<typename... ConstructorArgs>
        inline MethodBinding<ClassType> constructor(const std::initializer_list<std::string>& paramNames);

        template <typename EnumType>
        inline ValueBinding<ClassType> value(EnumType value, const std::string& name);

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
        inline PropertyBinding& meta(MetaKey key, MetaValue value);

        template <typename MetaKey, typename MetaValue>
        inline PropertyBinding& meta(MetaKey key, const std::initializer_list<MetaValue>& value);

    protected:
        class PropertyData& _PropertyData;
    };

    template<typename ClassType>
    class MethodBinding : public TypeBinding<ClassType>
    {
    public:
        inline MethodBinding(class Method& method, TypeData& typeData);

        template <typename MetaKey, typename MetaValue>
        inline MethodBinding& meta(MetaKey key, MetaValue value);

        template <typename MetaKey, typename MetaValue>
        inline MethodBinding& meta(MetaKey key, const std::initializer_list<MetaValue>& value);

    protected:
        class Method& _Method;
    };

    template<typename ClassType>
    class ValueBinding : public TypeBinding<ClassType>
    {
    public:
        inline ValueBinding(class Value& value, TypeData& typeData);

        template <typename MetaKey, typename MetaValue>
        inline ValueBinding& meta(MetaKey key, MetaValue value);

        template <typename MetaKey, typename MetaValue>
        inline ValueBinding& meta(MetaKey key, const std::initializer_list<MetaValue>& value);

    protected:
        class Value& _Value;
    };
}
