#pragma once

namespace SimpleRTTR
{
    class RegistrationManager;

    inline RegistrationManager& Registration();

    class RegistrationManager
    {
    public:
        template<typename ClassType>
        inline TypeBinding<ClassType> Type();

        inline TypeBinding<SimpleRTTR::Type> Type(const SimpleRTTR::Type& type);

        static inline RegistrationManager& GetInstance();

    protected:
        friend void _InternalBeginRegistration(const char* filename);
        friend void _InternalEndRegistration();

        inline void BeginRegistration(const char* filename);
        inline void EndRegistration();

        inline RegistrationManager();  //should only be used by the root registration manager
    };

    //---
    // internal functions
    inline void _InternalBeginRegistration(const char* filename);
    inline void _InternalEndRegistration();
}
