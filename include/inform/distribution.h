// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <cstdint>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace inform
{
    class distribution_iterator;

    class distribution
    {
        private:
            friend class distribution_iterator;

            using histogram = std::vector<uint64_t>;

            histogram hist;
            uint64_t sample_size;

        public:
            using iterator = distribution_iterator;

            distribution() = delete;
            distribution(size_t n);
            distribution(std::initializer_list<uint64_t>&& l);
            distribution(std::vector<uint64_t> const& v);
            template <typename Iter> distribution(Iter first, Iter last);
            distribution(distribution const&) = default;
            distribution(distribution&&) = default;

            auto operator=(distribution const&) -> distribution& = default;
            auto operator=(distribution&&) -> distribution& = default;

            auto is_valid() const -> bool;
            auto size() const -> std::size_t;
            auto count() const -> uint64_t;

            auto tic(uint64_t event, uint64_t n = 1) -> uint64_t;
            auto set(uint64_t event, uint64_t occurrence) -> uint64_t;
            auto get(uint64_t event) const -> uint64_t;

            auto at(uint64_t event) const -> double;
            auto operator[](uint64_t event) const -> double;

            auto begin() const -> iterator;
            auto end() const -> iterator;
    };

    template <typename Iter>
    distribution::distribution(Iter first, Iter last) : hist{first,last}
    {
        if (hist.size() == 0)
        {
            auto msg = "inform::distribution: cannot have size 0";
            throw std::invalid_argument(msg);
        }
        sample_size = std::accumulate(std::begin(hist), std::end(hist), 0);
    }

    class distribution_iterator : public std::iterator<std::input_iterator_tag, double>
    {
        private:
            friend class distribution;

            distribution const& dist;
            distribution::histogram::const_iterator it;

            distribution_iterator(distribution const& dist, distribution::histogram::const_iterator it);

        public:
            distribution_iterator() = delete;
            distribution_iterator(distribution_iterator const&) = default;
            distribution_iterator(distribution_iterator&&) = default;

            auto operator=(distribution_iterator const&) -> distribution_iterator& = default;
            auto operator=(distribution_iterator&&) -> distribution_iterator& = default;

            auto operator==(distribution_iterator const& other) const -> bool;
            auto operator!=(distribution_iterator const& other) const -> bool;

            auto operator++() -> distribution_iterator&;
            auto operator++(int) -> distribution_iterator;
            auto operator*() const -> double;
    };
}
