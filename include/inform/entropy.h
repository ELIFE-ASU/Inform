// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/distribution.h>

namespace inform
{
    auto shannon_entropy(distribution const& pdf) -> double;
    auto renyi_entropy(distribution const& pdf, double order) -> double;
}
