namespace SimpleRTTR
{
    RegistrationManager::RegistrationManager()
    {
        TypeStorage& storage = types().get_storage();

        //register a few of the basic fundamental types
        storage.get_or_create_type<void>(false);
        storage.get_or_create_type<char>(false);
        storage.get_or_create_type<short>(false);
        storage.get_or_create_type<int>(false);
        storage.get_or_create_type<long>(false);
        storage.get_or_create_type<unsigned char>(false);
        storage.get_or_create_type<unsigned short>(false);
        storage.get_or_create_type<unsigned int>(false);
        storage.get_or_create_type<unsigned long>(false);
        storage.get_or_create_type<float>(false);
        storage.get_or_create_type<double>(false);
        storage.get_or_create_type<void*>(false);
    }

    //---
    // Registration
    RegistrationManager& registration()
    {
        return RegistrationManager::instance();
    }

    template<typename ClassType>
    TypeBinding<ClassType> RegistrationManager::type()
    {
        return TypeBinding<ClassType>(types().get_storage().get_or_create_type<ClassType>(true));
    }

    TypeBinding<SimpleRTTR::Type> RegistrationManager::type(const SimpleRTTR::Type& type)
    {
        return TypeBinding<SimpleRTTR::Type>(_InternalGetTypeData(type));
    }

    RegistrationManager& RegistrationManager::instance()
    {
        static RegistrationManager mgr;
        return mgr;
    }

    void RegistrationManager::begin_registration(const char* filename)
    {
        types().begin_registration(filename);
    }

    void RegistrationManager::end_registration()
    {
        types().end_registration();
    }


    void _InternalBeginRegistration(const char* filename)
    {
        registration().begin_registration(filename);
    }

    void _InternalEndRegistration()
    {
        registration().end_registration();
    }
}