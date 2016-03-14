// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include <cmath>
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

TEST(Distribution, Set)
{
    auto dist = inform::distribution(2);
    ASSERT_FALSE(dist.is_valid());

    ASSERT_EQ(uint64_t{2}, dist.set(0,2));
    ASSERT_EQ(uint64_t{2}, dist.count());
    ASSERT_TRUE(dist.is_valid());

    ASSERT_EQ(uint64_t{0}, dist.set(0,0));
    ASSERT_EQ(uint64_t{0}, dist.count());
    ASSERT_FALSE(dist.is_valid());

    ASSERT_EQ(uint64_t{4}, dist.set(0,4));
    ASSERT_EQ(uint64_t{2}, dist.set(1,2));
    ASSERT_EQ(uint64_t{6}, dist.count());
    ASSERT_EQ(uint64_t{0}, dist.set(0,0));
    ASSERT_EQ(uint64_t{2}, dist.count());
    ASSERT_TRUE(dist.is_valid());

    ASSERT_THROW(dist.set(-1,5), std::out_of_range);
    ASSERT_THROW(dist.set(dist.size(),5), std::out_of_range);
}

TEST(Distribution, At)
{
    auto dist = inform::distribution(2);
    ASSERT_TRUE(std::isnan(dist.at(0)));
    ASSERT_TRUE(std::isnan(dist.at(1)));

    dist.tic(0);
    ASSERT_DOUBLE_EQ(1.0, dist.at(0));
    ASSERT_DOUBLE_EQ(0.0, dist.at(1));

    dist.tic(1);
    ASSERT_DOUBLE_EQ(0.5, dist.at(0));
    ASSERT_DOUBLE_EQ(0.5, dist.at(1));

    dist.tic(1);
    ASSERT_DOUBLE_EQ(1.0/3.0, dist.at(0));
    ASSERT_DOUBLE_EQ(2.0/3.0, dist.at(1));

    dist.set(0,4);
    dist.set(1,2);
    ASSERT_DOUBLE_EQ(2.0/3.0, dist.at(0));
    ASSERT_DOUBLE_EQ(1.0/3.0, dist.at(1));

    ASSERT_THROW(dist.at(-1), std::out_of_range);
    ASSERT_THROW(dist.at(dist.size()), std::out_of_range);
}


TEST(Distribution, IndexOperator)
{
    auto dist = inform::distribution(2);
    ASSERT_TRUE(std::isnan(dist[0]));
    ASSERT_TRUE(std::isnan(dist[1]));

    dist.tic(0);
    ASSERT_DOUBLE_EQ(1.0, dist[0]);
    ASSERT_DOUBLE_EQ(0.0, dist[1]);

    dist.tic(1);
    ASSERT_DOUBLE_EQ(0.5, dist[0]);
    ASSERT_DOUBLE_EQ(0.5, dist[1]);

    dist.tic(1);
    ASSERT_DOUBLE_EQ(1.0/3.0, dist[0]);
    ASSERT_DOUBLE_EQ(2.0/3.0, dist[1]);

    dist.set(0,4);
    dist.set(1,2);
    ASSERT_DOUBLE_EQ(2.0/3.0, dist[0]);
    ASSERT_DOUBLE_EQ(1.0/3.0, dist[1]);
}
