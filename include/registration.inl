namespace SimpleRTTR
{
    inline void RegisterFundamentalCppTypes(RegistrationManager& mgr)
    {
        //register a few of the basic fundamental types
        mgr.Type<char>();
        mgr.Type<short>();
        mgr.Type<int>();
        mgr.Type<long>();
        mgr.Type<unsigned char>();
        mgr.Type<unsigned short>();
        mgr.Type<unsigned int>();
        mgr.Type<unsigned long>();
        mgr.Type<float>();
        mgr.Type<double>();
        mgr.Type<void*>();
    }


    RegistrationManager::RegistrationManager()
    {
        //register basic C++ types
        RegisterFundamentalCppTypes(*this);
    }

    //---
    // Registration
    RegistrationManager& Registration()
    {
        return RegistrationManager::GetInstance();
    }

    template<typename ClassType>
    TypeBinding<ClassType> RegistrationManager::Type()
    {
        //SIMPLERTTR_ASSERT(!Types().HasType<ClassType>())
        return TypeBinding<ClassType>(Types().GetStorage().GetOrCreateType<ClassType>());
    }

    RegistrationManager& RegistrationManager::GetInstance()
    {
        static RegistrationManager mgr;
        return mgr;
    }
}