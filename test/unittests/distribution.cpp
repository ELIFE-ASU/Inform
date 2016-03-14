// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include <inform/distribution.h>

TEST(Distribution, SampleSizeConstructor)
{
    auto dist = inform::distribution(5);
    ASSERT_FALSE(dist.is_valid());
    ASSERT_EQ(size_t{5}, dist.size());
    ASSERT_EQ(uint64_t{0}, dist.count());
}

TEST(Distribution, Tic)
{
    auto dist = inform::distribution(2);
    ASSERT_FALSE(dist.is_valid());

    ASSERT_EQ(uint64_t{1}, dist.tic(0));
    ASSERT_EQ(uint64_t{1}, dist.count());
    ASSERT_TRUE(dist.is_valid());

    ASSERT_EQ(uint64_t{2}, dist.tic(0));
    ASSERT_EQ(uint64_t{2}, dist.count());

    ASSERT_EQ(uint64_t{2}, dist.tic(1,2));
    ASSERT_EQ(uint64_t{4}, dist.count());

    ASSERT_THROW(dist.tic(-1), std::out_of_range);
    ASSERT_THROW(dist.tic(dist.size()), std::out_of_range);
}
