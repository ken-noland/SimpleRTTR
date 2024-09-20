#pragma once

namespace std {

    template <>
    struct hash<SimpleRTTR::Type> {
        inline std::size_t operator()(const SimpleRTTR::Type& type) const {
            return type.hash();
        }
    };

    template <>
        struct hash<SimpleRTTR::TypeReference> {
        inline std::size_t operator()(const SimpleRTTR::TypeReference& typeRef) const {
            return typeRef.hash();
        }
    };
    
    template <>
    struct hash<SimpleRTTR::Property> {
        inline std::size_t operator()(const SimpleRTTR::Property& property) const {
            return property.hash();
        }
    };

    template <>
    struct hash<SimpleRTTR::PropertyContainer> {
        inline std::size_t operator()(const SimpleRTTR::PropertyContainer& properties) const {
            return properties.hash();
        }
    };

    template <>
    struct hash<SimpleRTTR::Method> {
        inline std::size_t operator()(const SimpleRTTR::Method& method) const {
            return method.hash();
        }
    };

    template <>
    struct hash<SimpleRTTR::MethodContainer> {
        inline std::size_t operator()(const SimpleRTTR::MethodContainer& methods) const {
            return methods.hash();
        }
    };

    template <>
    struct hash<SimpleRTTR::Parameter> {
        inline std::size_t operator()(const SimpleRTTR::Parameter& parameter) const {
            return parameter.hash();
        }
    };

    template <>
    struct hash<SimpleRTTR::ParameterContainer> {
        inline std::size_t operator()(const SimpleRTTR::ParameterContainer& parameters) const {
            return parameters.hash();
        }
    };

    template <>
    struct hash<SimpleRTTR::Meta> {
        inline std::size_t operator()(const SimpleRTTR::Meta& meta) const {
            return meta.hash();
        }
    };

    template <>
    struct hash<SimpleRTTR::MetaContainer> {
        inline std::size_t operator()(const SimpleRTTR::MetaContainer& meta) const {
            return meta.hash();
        }
    };

    template <>
    struct hash<SimpleRTTR::Value> {
        inline std::size_t operator()(const SimpleRTTR::Value& value) const {
            return value.hash();
        }
    };

    template <>
    struct hash<SimpleRTTR::ValueContainer> {
        inline std::size_t operator()(const SimpleRTTR::ValueContainer& values) const {
            return values.hash();
        }
    };

    template <>
    struct hash<SimpleRTTR::Variant> {
        inline std::size_t operator()(const SimpleRTTR::Variant& var) const {
            return var.hash();
        }
    };

    //---
    //TODO: The following may change. There are a number of containers that haven't been wrapped in a container class, and I just don't have time to go through them all at the moment
    template <>
    struct hash<SimpleRTTR::TypeData::NamespaceContainer> {
        inline std::size_t operator()(const SimpleRTTR::TypeData::NamespaceContainer& namespaces) const {
            std::size_t seed = 0;
            for (const std::string& value : namespaces)
            {
                seed ^= std::hash<std::string>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    template <>
    struct hash<SimpleRTTR::TypeData::TemplateTypeContainer> {
        inline std::size_t operator()(const SimpleRTTR::TypeData::TemplateTypeContainer& templateParams) const {
            std::size_t seed = 0;
            for (const SimpleRTTR::TypeReference& typeRef : templateParams)
            {
                seed ^= typeRef.hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };


}
