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

    inline TypeManager& Types();
}

//definitions
#include "simplerttrassert.h"
#include "stdtypes.h"
#include "hash.h"
#include "typereference.h"
#include "helpers.h"
#include "iterator.h"
#include "variant.h"
#include "meta.h"
#include "property.h"
#include "method.h"
#include "value.h"
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
#include "typedata.inl"
#include "typebinding.inl"
#include "registration.inl"

//forward declaration of auto-register
namespace {
    class AutoRegister;
}

#define SIMPLE_RTTR_ALLOW_ACCESS friend class AutoRegister;

#define SIMPLERTTR  namespace {                                         \
                        class AutoRegister                              \
                        {                                               \
                        public:                                         \
                            AutoRegister()                              \
                            {                                           \
                                SimpleRTTR::_InternalBeginRegistration( \
                                    __FILE__                            \
                                );                                      \
                                Register();                             \
                                SimpleRTTR::_InternalEndRegistration(); \
                            }                                           \
                            void Register();                            \
                        };                                              \
                        static const AutoRegister AutoReg;              \
                    }                                                   \
                    void AutoRegister::Register()
