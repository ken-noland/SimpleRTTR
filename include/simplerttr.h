#pragma once

#include <memory>
#include <vector>
#include <bitset>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <any>
#include <locale>
#include <string>
#include <cstring>
#include <sstream>
#include <functional>
#include <algorithm>
#include <optional>

#if defined(__GNUC__) || defined(__GNUG__)
#include <cxxabi.h>
#endif //__GNUC__

//predeclarations
namespace SimpleRTTR
{
    class RegistrationManager;
    class Type;
    class TypeReference;
    class Value;
    class TypeData;
    class TypeHelperBase;
    class TypeManager;

    inline TypeManager& types();
}

//definitions
#include "simplerttrassert.h"
#include "hash.h"
#include "typereference.h"
#include "helpers.h"
#include "iterator.h"
#include "typefunctions.h"
#include "variant.h"
#include "meta.h"
#include "property.h"
#include "method.h"
#include "value.h"
#include "templateparameter.h"
#include "typedata.h"
#include "types.h"
#include "typehelpers.h"
#include "typebinding.h"
#include "registration.h"

//implementations
#include "hash.inl"
#include "typereference.inl"
#include "property.inl"
#include "method.inl"
#include "variant.inl"
#include "value.inl"
#include "variant_copy.inl"
#include "variant_tostring.inl"
#include "meta.inl"
#include "types.inl"
#include "templateparameter.inl"
#include "typedata.inl"
#include "typebinding.inl"
#include "registration.inl"

//forward declaration of auto-register
namespace {
    class AutoRegister;
}

#define SIMPLE_RTTR_ALLOW_ACCESS friend class AutoRegister;

#define SIMPLERTTR  namespace {                                         \
                        class [[maybe_unused]] AutoRegister             \
                        {                                               \
                        public:                                         \
                            inline AutoRegister()                       \
                            {                                           \
                                SimpleRTTR::_InternalBeginRegistration( \
                                    __FILE__                            \
                                );                                      \
                                Register();                             \
                                SimpleRTTR::_InternalEndRegistration(); \
                            }                                           \
                            inline void Register();                     \
                        };                                              \
                        static volatile const AutoRegister AutoReg;     \
                    }                                                   \
                    void AutoRegister::Register()

#define REGISTER_PROPERTY(cls, name) \
	&cls::name, #name
