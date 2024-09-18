namespace SimpleRTTR
{
    RegistrationManager::RegistrationManager()
    {
        TypeStorage& storage = Types().GetStorage();

        //register a few of the basic fundamental types
        storage.GetOrCreateType<void>(false);
        storage.GetOrCreateType<char>(false);
        storage.GetOrCreateType<short>(false);
        storage.GetOrCreateType<int>(false);
        storage.GetOrCreateType<long>(false);
        storage.GetOrCreateType<unsigned char>(false);
        storage.GetOrCreateType<unsigned short>(false);
        storage.GetOrCreateType<unsigned int>(false);
        storage.GetOrCreateType<unsigned long>(false);
        storage.GetOrCreateType<float>(false);
        storage.GetOrCreateType<double>(false);
        storage.GetOrCreateType<void*>(false);
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
        return TypeBinding<ClassType>(Types().GetStorage().GetOrCreateType<ClassType>(true));
    }

    TypeBinding<SimpleRTTR::Type> RegistrationManager::Type(const SimpleRTTR::Type& type)
    {
        return TypeBinding<SimpleRTTR::Type>(_InternalGetTypeData(type));
    }

    RegistrationManager& RegistrationManager::GetInstance()
    {
        static RegistrationManager mgr;
        return mgr;
    }

    void RegistrationManager::BeginRegistration(const char* filename)
    {
        Types().BeginRegistration(filename);
    }

    void RegistrationManager::EndRegistration()
    {
        Types().EndRegistration();
    }


    void _InternalBeginRegistration(const char* filename)
    {
        Registration().BeginRegistration(filename);
    }

    void _InternalEndRegistration()
    {
        Registration().EndRegistration();
    }
}