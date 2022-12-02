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

        static inline RegistrationManager& GetInstance();

    protected:
        inline RegistrationManager();  //should only be used by the root registration manager
    };
}
