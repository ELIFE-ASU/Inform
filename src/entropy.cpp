// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/entropy.h>

#include <cmath>

namespace inform
{
    auto shannon_entropy(distribution const& pdf, double base) -> double
    {
        if (!pdf.is_valid())
        {
            auto msg = "inform::shannon_entropy: provided distribution is invalid";
            throw std::invalid_argument(msg);
        }
        if (base <= 1)
        {
            auto msg = "inform::shannon_entropy: provided base must be greater than 1";
            throw std::invalid_argument(msg);
        }
        auto h = 0.0;
        for (auto const& p : pdf)
        {
            if (p != 0.0)
            {
                h -= p * std::log(p);
            }
        }
        return h / std::log(base);
    }
}
