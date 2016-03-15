// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/distribution.h>

#include <string>

namespace inform
{
    distribution::distribution(size_t n) : hist(n,0), sample_size{0}
    {
        if (n == 0)
        {
            auto msg = "inform::distribution: cannot have size 0";
            throw std::invalid_argument(msg);
        }
    }

    distribution::distribution(std::initializer_list<uint64_t>&& l) : hist{l}
    {
        if (hist.size() == 0)
        {
            auto msg = "inform::distribution: cannot have size 0";
            throw std::invalid_argument(msg);
        }
        sample_size = std::accumulate(std::begin(hist), std::end(hist), 0);
    }

    distribution::distribution(std::vector<uint64_t> const& v) : hist{v}
    {
        if (hist.size() == 0)
        {
            auto msg = "inform::distribution: cannot have size 0";
            throw std::invalid_argument(msg);
        }
        sample_size = std::accumulate(std::begin(hist), std::end(hist), 0);
    }

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

    auto distribution::get(uint64_t event) const -> uint64_t
    {
        return hist.at(event);
    }

    auto distribution::at(uint64_t event) const -> double
    {
        return static_cast<double>(hist.at(event)) / sample_size;
    }

    auto distribution::operator[](uint64_t event) const -> double
    {
        return static_cast<double>(hist[event]) / sample_size;
    }

    auto distribution::begin() const -> iterator
    {
        return {*this, std::begin(hist)};
    }

    auto distribution::end() const -> iterator
    {
        return {*this, std::end(hist)};
    }
}