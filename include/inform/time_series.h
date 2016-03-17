// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/entropy.h>
#include <inform/state_encoding.h>
#include <iterator>
#include <type_traits>

#include <algorithm>

namespace inform
{
    template <typename Iter>
    auto active_information_distributions(Iter first, Iter last, uint64_t k,
                                          distribution& states,
                                          distribution& histories,
                                          distribution& futures) -> void
    {
        using iterator_type = typename std::iterator_traits<Iter>::iterator_category;
        static_assert(std::is_same<iterator_type, std::random_access_iterator_tag>::value,
            "iterator must be a random access iterator");

        auto future = first + k;
        while (future != last)
        {
            auto const history = encode_state(first, future);
            auto const state   = history ^ (*future << k);

            states.tic(state);
            histories.tic(history);
            futures.tic(*future);

            ++first;
            ++future;
        }
    }

    template <typename Iter>
    auto active_information(Iter first, Iter last, uint64_t k) -> double
    {
        int64_t const size = std::distance(first, last);
        if (size <= 1)
        {
            auto msg = "inform::active_information: time series must have more than one observation";
            throw std::invalid_argument(msg);
        }
        else if (size - k <= 1)
        {
            auto msg = "inform::active_information: history length must be at least one less than the time series length";
            throw std::invalid_argument(msg);
        }

        auto states    = distribution(2 << k);
        auto histories = distribution(1 << k);
        auto futures   = distribution(2);

        active_information_distributions(first, last, k, states, histories, futures);

        return mutual_information(states, histories, futures);
    }

    template <typename Iter>
    auto active_information(Iter first, Iter last, size_t time_steps, uint64_t k) -> double
    {
        int64_t const size = std::distance(first, last);
        if (size <= 1)
        {
            auto msg = "inform::active_information: time series must have more than one observation";
            throw std::invalid_argument(msg);
        }
        else if (size - k <= 1)
        {
            auto msg = "inform::active_information: history length must be at least one less than the time series length";
            throw std::invalid_argument(msg);
        }
        else if (size % time_steps != 0)
        {
            auto msg = "inform::active_information: each time series must have the same number of time steps";
            throw std::invalid_argument(msg);
        }

        auto states    = distribution(2 << k);
        auto histories = distribution(1 << k);
        auto futures   = distribution(2);

        for (auto tail = first + time_steps; first != last; first += time_steps, tail += time_steps)
        {
            active_information_distributions(first, tail, k, states, histories, futures);
        }

        return mutual_information(states, histories, futures);
    }
}
