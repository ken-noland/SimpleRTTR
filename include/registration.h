#pragma once

namespace SimpleRTTR
{
    class RegistrationManager;

    inline RegistrationManager& registration();

    class RegistrationManager
    {
    public:
        template<typename ClassType>
        inline TypeBinding<ClassType> type();

        inline TypeBinding<SimpleRTTR::Type> type(const SimpleRTTR::Type& type);

        static inline RegistrationManager& instance();

    protected:
        friend void _InternalBeginRegistration(const char* filename);
        friend void _InternalEndRegistration();

        inline void begin_registration(const char* filename);
        inline void end_registration();

        inline RegistrationManager();  //should only be used by the root registration manager
    };

    //---
    // internal functions
    inline void _InternalBeginRegistration(const char* filename);
    inline void _InternalEndRegistration();
}
