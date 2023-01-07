#pragma once

#include <memory>
#include <vector>
#include <bitset>
#include <stdexcept>
#include <type_traits>
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
    class TypeData;
    class TypeHelperBase;
    class TypeManager;

    inline TypeManager& Types();
}

//definitions
#include "simplerttrassert.h"
#include "stdtypes.h"
#include "helpers.h"
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
#include "property.inl"
#include "method.inl"
#include "variant.inl"
#include "variant_tostring.inl"
#include "meta.inl"
#include "types.inl"
#include "typedata.inl"
#include "typebinding.inl"
#include "registration.inl"

#define SIMPLERTTR  namespace {                                     \
                        class AutoRegister                          \
                        {                                           \
                        public:                                     \
                            AutoRegister();                         \
                        };                                          \
                        static const AutoRegister AutoReg;          \
                    }                                               \
                    AutoRegister::AutoRegister()
