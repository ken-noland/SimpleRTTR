#pragma once

#include <memory>
#include <vector>
#include <bitset>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <any>
#include <locale>

//predeclarations
namespace SimpleRTTR
{
    class RegistrationManager;
    class Type;
    class TypeData;
    class TypeHelperBase;
}

//definitions
#include "stdtypes.h"
#include "helpers.h"
#include "assert.h"
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
