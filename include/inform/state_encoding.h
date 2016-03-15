// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <cstdint>
#include <type_traits>
#include <vector>

namespace inform
{
    template <typename Iter>
    auto encode_state(Iter first, Iter last) -> uint64_t
    {
        static_assert(std::is_convertible<typename std::iterator_traits<Iter>::value_type, bool>::value,
            "iterator must iterate over values convertible to bool");

        auto const size = std::distance(first,last);
        static decltype(size) const max_size = 8 * sizeof(uint64_t);
        if (size <= 0)
        {
            auto msg = "inform::encode_state: state length must be positive, nonzero";
            throw std::invalid_argument(msg);
        }
        else if (size > max_size)
        {
            auto msg = "inform::encode_state: state length cannot exceed " + std::to_string(max_size);
            throw std::invalid_argument(msg);
        }

        uint64_t encoding = 0;
        for (uint64_t k = 0; first != last; ++k, ++first)
        {
            encoding += *first * (1 << k);
        }

        return encoding;
    }

    template <typename Container = std::vector<bool>>
    auto encode_state(Container state) -> uint64_t
    {
        return encode_state(std::begin(state), std::end(state));
    }

    template <typename Iter>
    auto decode_state(uint64_t encoding, uint64_t length, Iter first) -> Iter
    {
        static decltype(length) const max_size = 8 * sizeof(encoding);
        if (length > max_size)
        {
            auto msg = "inform::decode_state: decoded state length cannot exceed " + std::to_string(max_size);
            throw std::invalid_argument(msg);
        }

        for(; length > 0; --length, ++first, encoding >>= 1)
        {
            *first = encoding & 1;
        }
        return first;
    }

    template <typename Container = std::vector<bool>>
    auto decode_state(uint64_t encoding, uint64_t length) -> Container;

    template <typename Container>
    auto decode_state(uint64_t encoding, uint64_t length) -> Container
    {
        Container xs;
        decode_state(encoding, length, std::back_inserter(xs));
        return xs;
    }

    template <>
    auto decode_state<std::vector<bool>>(uint64_t encoding, uint64_t length) -> std::vector<bool>
    {
        std::vector<bool> xs;
        xs.reserve(length);
        decode_state(encoding, length, std::back_inserter(xs));
        return xs;
    }
}
