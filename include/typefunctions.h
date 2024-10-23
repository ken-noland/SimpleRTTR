#pragma once

namespace SimpleRTTR
{
    struct TypeFunctions
    {
        using ConstructorFunction = void* (*)(void* objMem);
        using DestructorFunction = void (*)(void* objMem);

        using CopyConstructorFunction = void* (*)(void* dest, const void* src);
        using MoveConstructorFunction = void* (*)(void* dest, void* src);

        using EqualOperatorFunction = bool (*)(const void* lhs, const void* rhs);

        //using ToStringFunction = std::string(*)(const void* obj);

        using ConversionFunction = void (*)(void* dest, const void* src);

        using AssignmentOperatorFunction = void* (*)(void* dest, const void* src);

        TypeFunctions() :
            Constructor(nullptr),
            Destructor(nullptr),
            CopyConstructor(nullptr),
            MoveConstructor(nullptr),
            EqualOperator(nullptr),
            AssignmentOperator(nullptr)
        {}

        ConstructorFunction Constructor;
        DestructorFunction Destructor;

        CopyConstructorFunction CopyConstructor;
        MoveConstructorFunction MoveConstructor;

        //equality operators
        EqualOperatorFunction EqualOperator;

        AssignmentOperatorFunction AssignmentOperator;

        std::vector<std::pair<std::type_index, ConversionFunction>> ConversionFunctions;
    };

    template<typename ClassType>
    inline void* DefaultConstructorHelper(void* mem)
    {
        new (mem) ClassType();
        return mem;
    }

    template<typename ClassType>
    inline void DefaultDestructorHelper(void* mem)
    {
        return reinterpret_cast<ClassType*>(mem)->~ClassType();
    }


    template<typename ClassType>
    inline void* DefaultCopyConstructorHelper(void* dest, const void* src)
    {
        new (dest) ClassType(*static_cast<const ClassType*>(src));
        return dest;
    }

    template<typename ClassType>
    inline void* DefaultMoveConstructorHelper(void* dest, void* src)
    {
        new (dest) ClassType(std::move(*static_cast<ClassType*>(src)));
        return dest;
    }

    template<typename LHSClassType, typename RHSClassType = LHSClassType>
    inline bool DefaultEqualityOperator(const void* lhs, const void* rhs)
    {
        // Check if LHS and RHS are references, and compare the values they refer to
        if constexpr (std::is_reference_v<LHSClassType> || std::is_reference_v<RHSClassType>)
        {
            using LHSValueType = std::remove_reference_t<LHSClassType>;
            using RHSValueType = std::remove_reference_t<RHSClassType>;

            const LHSValueType* lhsValuePtr = reinterpret_cast<const LHSValueType*>(lhs);
            const RHSValueType* rhsValuePtr = reinterpret_cast<const RHSValueType*>(rhs);

            return *lhsValuePtr == *rhsValuePtr;
        }
        else
        {
            // Compare non-reference types
            const LHSClassType* lhsTypePtr = reinterpret_cast<const LHSClassType*>(lhs);
            const RHSClassType* rhsTypePtr = reinterpret_cast<const RHSClassType*>(rhs);

            return *lhsTypePtr == *rhsTypePtr;
        }
    }

    //specialization for const char*, const char*
    template<>
    inline bool DefaultEqualityOperator<const char*, const char*>(const void* lhs, const void* rhs)
    {
        // Dereference void* to get the actual const char* pointers
        const char* const* lhsPtr = reinterpret_cast<const char* const*>(lhs);
        const char* const* rhsPtr = reinterpret_cast<const char* const*>(rhs);

        // Compare the contents of the strings
        return std::strcmp(*lhsPtr, *rhsPtr) == 0;
    }


    template<typename ClassType>
    inline void* FundamentalConstructorHelper(void* mem)
    {
        *static_cast<ClassType*>(mem) = ClassType();

        return mem;
    }

    template<typename ClassType>
    inline void FundamentalDestructorHelper(void* mem)
    {
    }

    template<typename ClassType>
    inline void* FundamentalCopyHelper(void* dest, const void* src)
    {
        if constexpr (std::is_reference_v<ClassType>)
        {
            // Copy the address of the referenced object
            using DereferencedType = std::remove_reference_t<ClassType>;
            *reinterpret_cast<DereferencedType**>(dest) = const_cast<DereferencedType*>(*reinterpret_cast<const DereferencedType* const*>(src));
        }
        else
        {
            *static_cast<ClassType*>(dest) = *static_cast<const ClassType*>(src);
        }

        return dest;
    }

    template<typename ClassType>
    inline void* FundamentalMoveHelper(void* dest, void* src)
    {
        if constexpr (std::is_reference_v<ClassType>)
        {
            // Move the address of the referred object
            *reinterpret_cast<std::remove_reference_t<ClassType>**>(dest) = std::move(*reinterpret_cast<std::remove_reference_t<ClassType>**>(src));
        }
        else
        {
            *static_cast<ClassType*>(dest) = std::move(*static_cast<ClassType*>(src));
        }

        return dest;
    }

    template<typename ClassType>
    inline void* DefaultAssignmentOperatorHelper(void* dest, const void* src)
    {
        using DecayedType = std::remove_cv_t<std::decay_t<ClassType>>;
        *reinterpret_cast<DecayedType*>(dest) = *reinterpret_cast<const DecayedType*>(src);
        return dest;
    }

    template<typename DstType, typename SrcType>
    void IntegralConversionHelper(void* dest, const void* src)
    {
        *static_cast<DstType*>(dest) = static_cast<DstType>(*static_cast<const SrcType*>(src));
    }

    template<typename DstType, typename SrcType>
    void FloatingPointConversionHelper(void* dest, const void* src)
    {
        *static_cast<DstType*>(dest) = static_cast<DstType>(*static_cast<const SrcType*>(src));
    }

} // namespace SimpleRTTR