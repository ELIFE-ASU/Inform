// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <cstdint>
#include <type_traits>

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
}
