// Copyright (c) 2018 Johannes Zeppenfeld
// Distributed under the MIT license, see LICENSE file in repository root.

#ifndef REFLECT_DETAIL_TRAITS_H
#define REFLECT_DETAIL_TRAITS_H

#include <type_traits>

//------------------------------------------------------------------------------
//--                     Begin Namespace Reflect::Detail                      --
namespace Reflect { namespace Detail {

// Simplified enable_if for better usability.
enum struct EnableIfType { };

template <bool condition>
using EnableIf = typename std::enable_if<condition, EnableIfType>::type;

// Determines whether T_Derived is derived from or equivalent to T_Base.
// Evaluates to false if more than two types are specified.
// Note that void is considered a base of all types.
template <typename, typename ...>
struct IsDerived : std::false_type { };
template <typename T_Derived, typename T_Base>

struct IsDerived<T_Derived, T_Base>
: std::conditional<
    std::is_void<typename std::decay<T_Base>::type>::value ||
    std::is_base_of<typename std::decay<T_Base>::type,
                    typename std::decay<T_Derived>::type>::value ||
    std::is_same<typename std::decay<T_Base>::type,
                 typename std::decay<T_Derived>::type>::value,
    std::true_type, std::false_type
>::type { };

// Determines whether the first type is related to the second type in a way that
// could be resolved by the reflection system, i.e., if one type is derived from
// or is equivalent to the other.
// Evaluates to false if more than two types are specified.
// Note that void is considered a base of all types.
template <typename, typename ...>
struct IsRelated : std::false_type { };

template <typename T_Lhs, typename T_Rhs>
struct IsRelated<T_Lhs, T_Rhs>
: std::conditional<
    IsDerived<T_Lhs, T_Rhs>::value ||
    IsDerived<T_Rhs, T_Lhs>::value,
    std::true_type, std::false_type
>::type { };

// Determines whether the first type is of the same template class as the
// second, disregarding the type's template parameters.
// Evaluates to false if more than two types are specified.
template <typename, typename ...>
struct IsSameTemplateImpl : std::false_type { };

template <template <typename ...> class T, typename ...U, typename ...V>
struct IsSameTemplateImpl<T<U...>, T<V...>> : std::true_type { };

template <typename ...T_Args>
struct IsSameTemplate : IsSameTemplateImpl<typename std::decay<T_Args>::type...>
{ };

// Applies lvalue-to-rvalue, array-to-pointer and function-to-pointer
// conversions to type T. This is similar to std::decay, but does not remove
// cv-qualifiers.
template <typename T>
struct DecomposeImpl {
    using U = typename std::remove_reference<T>::type;
    using type = typename std::conditional<
        std::is_array<U>::value,
        typename std::remove_extent<U>::type *,
        typename std::conditional< 
            std::is_function<U>::value,
            typename std::add_pointer<U>::type,
            U
        >::type
    >::type;
};

template <typename T>
using Decompose = typename DecomposeImpl<T>::type;

} }
//--                      End Namespace Reflect::Detail                       --
//------------------------------------------------------------------------------

#endif
