// Copyright 2024 Man Group Operations Limited
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <concepts>

#include "sparrow/array/data_type.hpp"
#include "sparrow/layout/fixed_size_layout.hpp"
#include "sparrow/layout/null_layout.hpp"
#include "sparrow/layout/variable_size_binary_layout.hpp"

namespace sparrow
{

    template <class T>
    struct common_native_types_traits
    {
        using value_type = T;
        template <class DS>
        using default_layout = fixed_size_layout<T, DS>;
    };

    template <>
    struct arrow_traits<null_type>
    {
        static constexpr data_type type_id = data_type::NA;
        using value_type = null_type;
        template <class DS>
        using default_layout = null_layout<DS>;
    };

    template <>
    struct arrow_traits<bool> : common_native_types_traits<bool>
    {
        static constexpr data_type type_id = data_type::BOOL;
    };

    template <>
    struct arrow_traits<std::uint8_t> : common_native_types_traits<std::uint8_t>
    {
        static constexpr data_type type_id = data_type::UINT8;
    };

    template <>
    struct arrow_traits<std::int8_t> : common_native_types_traits<std::int8_t>
    {
        static constexpr data_type type_id = data_type::INT8;
    };

    template <>
    struct arrow_traits<char> : common_native_types_traits<char>
    {
        static constexpr data_type type_id = data_type::UINT8;
    };

    template <>
    struct arrow_traits<std::uint16_t> : common_native_types_traits<std::uint16_t>
    {
        static constexpr data_type type_id = data_type::UINT16;
    };

    template <>
    struct arrow_traits<std::int16_t> : common_native_types_traits<std::int16_t>
    {
        static constexpr data_type type_id = data_type::INT16;
    };

    template <>
    struct arrow_traits<std::uint32_t> : common_native_types_traits<std::uint32_t>
    {
        static constexpr data_type type_id = data_type::UINT32;
    };

    template <>
    struct arrow_traits<std::int32_t> : common_native_types_traits<std::int32_t>
    {
        static constexpr data_type type_id = data_type::INT32;
    };

    template <>
    struct arrow_traits<std::uint64_t> : common_native_types_traits<std::uint64_t>
    {
        static constexpr data_type type_id = data_type::UINT64;
    };

    template <>
    struct arrow_traits<std::int64_t> : common_native_types_traits<std::int64_t>
    {
        static constexpr data_type type_id = data_type::INT64;
    };

    template< std::floating_point T >
    constexpr
    data_type expected_float_type(T = {})
    {
        switch(sizeof(T))
        {
            case 2: return data_type::HALF_FLOAT;
            case 4: return data_type::FLOAT;
            case 8: return data_type::DOUBLE;
            default: return data_type::NA;
        }
    }

    template < std::floating_point T >
    struct arrow_traits<T> : common_native_types_traits<T>
    {
        static constexpr data_type type_id = expected_float_type<T>();
    };


    template <>
    struct arrow_traits<std::string>
    {
        static constexpr data_type type_id = data_type::STRING;
        using value_type = std::string;
        template <class DS>
        using default_layout = variable_size_binary_layout<value_type, const std::string_view, DS>;
    };

    template <>
    struct arrow_traits<std::vector<byte_t>>
    {
        static constexpr data_type type_id = data_type::STRING;
        using value_type = std::vector<byte_t>;
        template <class DS>
        using default_layout = variable_size_binary_layout<value_type, const std::span<byte_t>, DS>;
    };

    template <>
    struct arrow_traits<timestamp> : common_native_types_traits<timestamp>
    {
        static constexpr data_type type_id = data_type::TIMESTAMP;
    };

    namespace predicate
    {

        struct
        {
            template <class T>
            consteval bool operator()(mpl::typelist<T>)
            {
                return sparrow::is_arrow_base_type<T>;
            }
        } constexpr is_arrow_base_type;

        struct
        {
            template <class T>
            consteval bool operator()(mpl::typelist<T>)
            {
                return sparrow::is_arrow_traits<arrow_traits<T>>;
            }
        } constexpr has_arrow_traits;
    }

}
