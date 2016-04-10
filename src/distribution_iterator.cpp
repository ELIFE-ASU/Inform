// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/distribution.h>

namespace inform
{
    distribution_iterator::distribution_iterator(distribution const& dist,
                                                   distribution::histogram::const_iterator it)
                                                   : dist{dist}, it{it} {}

    auto distribution_iterator::operator==(distribution_iterator const& other) const -> bool
    {
        return &dist == &other.dist && it == other.it;
    }

    auto distribution_iterator::operator!=(distribution_iterator const& other) const -> bool
    {
        return &dist != &other.dist || it != other.it;
    }

    auto distribution_iterator::operator++() -> distribution_iterator&
    {
        ++it;
        return *this;
    }

    auto distribution_iterator::operator++(int) -> distribution_iterator
    {
        return {dist, it++};
    }

    auto distribution_iterator::operator*() const -> double
    {
        return static_cast<double>(*it) / dist.count();
    }
}
