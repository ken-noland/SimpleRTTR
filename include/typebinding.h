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

    protected:
        inline TypeBinding();

        TypeData& _TypeData;
    };

    template<typename ClassType>
    class PropertyBinding : public TypeBinding<ClassType>
    {
    public:
        inline PropertyBinding(class Property& method, TypeData& typeData);

        template <typename... MetaType>
        inline PropertyBinding& Meta(MetaType...);

        template <typename MetaKey, typename MetaValue>
        inline PropertyBinding& Meta(MetaKey key, const std::initializer_list<MetaValue>& value);

    protected:
        class Property& _Property;
    };

    template<typename ClassType>
    class MethodBinding : public TypeBinding<ClassType>
    {
    public:
        inline MethodBinding(class Method& method, TypeData& typeData);

        template <typename... MetaType>
        inline MethodBinding& Meta(MetaType...);

        template <typename MetaKey, typename MetaValue>
        inline MethodBinding& Meta(MetaKey key, const std::initializer_list<MetaValue>& value);

    protected:
        class Method& _Method;
    };
}
