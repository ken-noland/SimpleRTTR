#pragma once

namespace SimpleRTTR
{
    class Type
    {
    public:
        using NamespaceList = TypeData::NamespaceContainer;
        using TemplateTypeList = TypeData::TemplateTypeContainer;

        inline Type(const TypeData& data);
        inline Type(const Type& type);
        inline Type(Type&& type);
        inline ~Type();

        inline Type& operator=(const Type& other);

        inline bool operator==(const Type& type) const;
        inline bool operator!=(const Type& type) const;
        inline bool equals(const Type& type) const;

        inline bool operator==(const std::type_info& info) const;
        inline bool operator!=(const std::type_info& info) const;
        inline bool equals(const std::type_info& typeData) const;

        static inline const Type& invalid_type();

        inline const std::string& name() const;
        inline const std::string& fully_qualified_name() const;
        inline std::size_t size() const;
        inline std::size_t hash() const;

        inline bool has_flag(TypeFlag flag) const;

        inline const std::type_index& type_index() const;

        inline const ConstructorContainer& constructors() const;
        inline const PropertyContainer& properties() const;
        inline const MethodContainer& methods() const;
        inline const NamespaceList& namespaces() const;
        inline const TemplateTypeList& template_params() const;
        inline const ValueContainer& values() const;

        inline const MetaContainer& meta() const;


        template<typename ClassType, typename Alloc = std::allocator<ClassType>, typename... Params>
        inline ClassType* create_instance(Params...) const;

        template<typename... Params>
        inline void* create_instance(Params...) const;

        template<typename ClassType, typename Alloc = std::allocator<ClassType>>
        inline void destroy_instance(ClassType* ptr) const;

        inline void destroy_instance(void* ptr) const;

        inline const TypeFunctions& type_functions() const;

    protected:
        friend TypeData& _InternalGetTypeData(const Type& type);
        const TypeData& _TypeData;
    };

    class TypeStorage
    {
    public:
        using TypePointer = std::unique_ptr<TypeData>;
        using TypeList = std::vector<TypePointer>;

        using iterator = TypeList::iterator;
        using const_iterator = TypeList::const_iterator;

        static inline const TypeData& invalid_type_data();
        static inline constexpr std::size_t invalid_type_size();

        template<typename ClassType>
        inline bool has_type_data() const;
        inline bool has_type_data(const std::type_info& typeInfo) const;
        inline bool has_type_data(const std::type_index& typeInfo) const;
        inline bool has_type_data(const std::string& name, std::size_t size) const;
        inline bool has_type_data(const TypeHelperBase& typeHelper) const;

        template<typename ClassType>
        inline const TypeData& get_type_data() const;
        inline const TypeData& get_type_data(const std::type_info& typeInfo) const;
        inline const TypeData& get_type_data(const std::type_index& typeIndex) const;
        inline const TypeData& get_type_data(const std::string& name, std::size_t size) const;
        inline const TypeData& get_type_data(const TypeHelperBase& typeHelper) const;

        template<typename ClassType>
        inline TypeData& get_or_create_type(bool _addedByUser);

        inline iterator begin() { return _Data.begin(); }
        inline const_iterator begin() const { return _Data.begin(); }

        inline iterator end() { return _Data.end(); }
        inline const_iterator end() const { return _Data.end(); }

    protected:
        friend class TypeManager;

        //only the TypeManager can create a TypeStorage
        inline TypeStorage(std::function<void(class TypeData&)> OnRegisterTypeCallback);

        inline TypeData& get_or_update_type_data(const TypeHelperBase& typeHelper, bool addedByUser);
        inline TypeData& register_type(const TypeHelperBase& typeHelper, bool addedByUser);
        inline TypeData& register_type(const TypeData& typeData);

        TypeList _Data;

        std::function<void(class TypeData&)> _OnRegisterTypeCallback;
    };

    class TypeManager
    {
    public:
        inline TypeManager();
        inline ~TypeManager();

        static inline TypeManager& instance();

        template<class ClassType>
        inline bool has_type() const;
        inline bool has_type(const std::type_info& typeInfo) const;
        inline bool has_type(const std::type_index& typeInfo) const;

        template<class ClassType>
        inline const Type get_type() const;
        inline const Type get_type(const std::type_info& typeInfo) const;
        inline const Type get_type(const std::type_index& typeIndex) const;

        template<class ClassType>
        inline const Type get_or_create_type();

        // iterators
    public:

        // Custom iterator that wraps TypeData iterator but returns Type
        class TypeIterator {
        private:
            typename TypeStorage::iterator dataIterator;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Type;
            using difference_type = std::ptrdiff_t;
            using pointer = Type*;
            using reference = Type;

            // Constructor
            TypeIterator(typename TypeStorage::iterator it) : dataIterator(it) {}

            // Dereference: Convert TypeData to Type
            Type operator*() const {
                return SimpleRTTR::Type(*(*dataIterator).get());  // Convert TypeData to Type
            }

            // Pre-increment
            TypeIterator& operator++() {
                ++dataIterator;
                return *this;
            }

            // Post-increment
            TypeIterator operator++(int) {
                TypeIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            // Comparison operators
            bool operator==(const TypeIterator& other) const {
                return dataIterator == other.dataIterator;
            }

            bool operator!=(const TypeIterator& other) const {
                return dataIterator != other.dataIterator;
            }
        };

        // Const iterator that wraps TypeData const_iterator but returns Type
        class ConstTypeIterator {
        private:
            typename TypeStorage::const_iterator dataIterator;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Type;
            using difference_type = std::ptrdiff_t;
            using pointer = const Type*;
            using reference = const Type;

            ConstTypeIterator(typename TypeStorage::const_iterator it) : dataIterator(it) {}

            Type operator*() const {
                return SimpleRTTR::Type(*(*dataIterator).get());
            }

            ConstTypeIterator& operator++() {
                ++dataIterator;
                return *this;
            }

            ConstTypeIterator operator++(int) {
                ConstTypeIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator==(const ConstTypeIterator& other) const {
                return dataIterator == other.dataIterator;
            }

            bool operator!=(const ConstTypeIterator& other) const {
                return dataIterator != other.dataIterator;
            }
        };

        using iterator = TypeIterator;
        using const_iterator = ConstTypeIterator;

        inline iterator begin();
        inline const_iterator begin() const;

        inline iterator end();
        inline const_iterator end() const;

    protected:
        friend class TypeBindingBase;
        friend class RegistrationManager;

        inline void begin_registration(const char* filename);
        inline void end_registration();

        inline TypeStorage& get_storage();              //only accessible from TypeBinding
        inline const TypeStorage& get_storage() const;  //only accessible from TypeBinding

        inline bool has_type(const TypeHelperBase& typeHelper) const;
        inline const Type get_type(const TypeHelperBase& typeHelper) const;

        inline void register_type(const TypeData& data);

        //called once the type has been registered
        inline void on_type_registered(TypeData& data);

    private:
        TypeStorage _TypeDataStorage;

        std::vector<Meta> _UserTypeMetadata;
    };

    //singleton accessor for all types
    inline TypeManager& types() 
    { 
        return TypeManager::instance(); 
    }
}
