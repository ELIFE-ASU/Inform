// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <cstdint>
#include <vector>

namespace inform
{
    class distribution
    {
        using histogram = std::vector<uint64_t>;

        public:
            distribution() = delete;
            distribution(size_t n);
            distribution(distribution const&) = default;
            distribution(distribution&&) = default;

            auto operator=(distribution const&) -> distribution& = default;
            auto operator=(distribution&&) -> distribution& = default;

            auto is_valid() const -> bool;
            auto size() const -> std::size_t;
            auto count() const -> uint64_t;

            auto tic(uint64_t event, uint64_t n = 1) -> uint64_t;
            auto set(uint64_t event, uint64_t occurance) -> uint64_t;

        private:
            histogram hist;
            uint64_t sample_size;
    };
}
