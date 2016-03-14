// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include <inform/distribution.h>

TEST(Distribution, SampleSizeConstructor)
{
    auto dist = inform::distribution(5);
    ASSERT_FALSE(dist.is_valid());
    ASSERT_EQ(size_t(5), dist.size());
}
