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
#include <sstream>

//predeclarations
namespace SimpleRTTR
{
    class RegistrationManager;
    class Type;
    class TypeData;
    class TypeHelperBase;
}

//definitions
#include "simplerttrassert.h"
#include "stdtypes.h"
#include "helpers.h"
#include "property.h"
#include "method.h"
#include "variant.h"
#include "meta.h"
#include "typedata.h"
#include "typehelpers.h"
#include "types.h"
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
