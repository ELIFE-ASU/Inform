// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/distribution.h>

namespace inform
{
    distribution::distribution(size_t n) : hist(n,0), sample_size{0} {}

    auto distribution::is_valid() const -> bool
    {
        return count() != 0 && size() != 0;
    }

    auto distribution::size() const -> size_t
    {
        return hist.size();
    }

    auto distribution::count() const -> uint64_t
    {
        return sample_size;
    }

    auto distribution::tic(uint64_t event, uint64_t n) -> uint64_t
    {
        auto const x = (hist.at(event) += n);
        sample_size += n;
        return x;
    }

    auto distribution::set(uint64_t event, uint64_t occurrence) -> uint64_t
    {
        auto const delta = occurrence - hist.at(event);
        hist.at(event) = occurrence;
        sample_size += delta;
        return occurrence;
    }
}
