// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include <cmath>
#include <inform/entropy.h>

TEST(ShannonEntropy, InvalidDistribution)
{
    auto const dist = inform::distribution(5);
    ASSERT_THROW(inform::shannon_entropy(dist), std::invalid_argument);
}

TEST(ShannonEntropy, Uniform)
{
    auto const dist = inform::distribution({1,1,1,1,1});
    ASSERT_DOUBLE_EQ(2.321928094887362, inform::shannon_entropy(dist));
}

TEST(ShannonEntropy, DeltaFunction)
{
    auto const dist = inform::distribution({0,1,0,0,0});
    ASSERT_DOUBLE_EQ(0.0, inform::shannon_entropy(dist));
}

TEST(ShannonEntropy, NonUniform)
{
    using namespace inform;
    ASSERT_DOUBLE_EQ(0.9182958340544896, shannon_entropy(distribution{2,1}));
    ASSERT_DOUBLE_EQ(1.0,                shannon_entropy(distribution{1,1,0}));
    ASSERT_DOUBLE_EQ(1.5219280948873621, shannon_entropy(distribution{2,2,1}));
}

TEST(RenyiEntropy, InvalidDistribution)
{
    auto const dist = inform::distribution(5);
    ASSERT_THROW(inform::renyi_entropy(dist, 2.0), std::invalid_argument);
}

TEST(RenyiEntropy, InvalidOrder)
{
    auto const dist = inform::distribution{1,1,1};
    ASSERT_NO_THROW(inform::renyi_entropy(dist, 2));
    ASSERT_NO_THROW(inform::renyi_entropy(dist, 1.5));
    ASSERT_NO_THROW(inform::renyi_entropy(dist, 1));
    ASSERT_NO_THROW(inform::renyi_entropy(dist, 0.5));
    ASSERT_NO_THROW(inform::renyi_entropy(dist, 0));
    ASSERT_THROW(inform::renyi_entropy(dist, -0.5), std::invalid_argument);
    ASSERT_THROW(inform::renyi_entropy(dist, -1), std::invalid_argument);
}

TEST(RenyiEntropy, Uniform)
{
    auto const dist = inform::distribution({1,1,1,1,1});
    ASSERT_DOUBLE_EQ(2.321928094887362, inform::renyi_entropy(dist, 0.0));
    ASSERT_DOUBLE_EQ(2.321928094887362, inform::renyi_entropy(dist, 0.5));
    ASSERT_DOUBLE_EQ(2.321928094887362, inform::renyi_entropy(dist, 1.0));
    ASSERT_DOUBLE_EQ(2.321928094887362, inform::renyi_entropy(dist, 1.5));
    ASSERT_DOUBLE_EQ(2.321928094887362, inform::renyi_entropy(dist, 2.0));
}

TEST(RenyiEntropy, DeltaFunction)
{
    auto const dist = inform::distribution({0,1,0,0,0});
    ASSERT_DOUBLE_EQ(2.321928094887362, inform::renyi_entropy(dist, 0.0));
    ASSERT_DOUBLE_EQ(0.0, inform::renyi_entropy(dist, 0.5));
    ASSERT_DOUBLE_EQ(0.0, inform::renyi_entropy(dist, 1.5));
    ASSERT_DOUBLE_EQ(0.0, inform::renyi_entropy(dist, 2.0));
}

TEST(RenyiEntropy, NonUniform)
{
    using namespace inform;
    ASSERT_DOUBLE_EQ(1.0, renyi_entropy(distribution{2,1}, 0.0));
    ASSERT_DOUBLE_EQ(0.9581441056060678, renyi_entropy(distribution{2,1}, 0.5));
    ASSERT_DOUBLE_EQ(0.9182958340544896, renyi_entropy(distribution{2,1}, 1.0));
    ASSERT_DOUBLE_EQ(0.8813839112838207, renyi_entropy(distribution{2,1}, 1.5));
    ASSERT_DOUBLE_EQ(0.84799690655495,   renyi_entropy(distribution{2,1}, 2.0));

    ASSERT_DOUBLE_EQ(1.584962500721156, renyi_entropy(distribution{1,1,0}, 0.0));
    ASSERT_DOUBLE_EQ(1.0, renyi_entropy(distribution{1,1,0}, 0.5));
    ASSERT_DOUBLE_EQ(1.0, renyi_entropy(distribution{1,1,0}, 1.0));
    ASSERT_DOUBLE_EQ(1.0, renyi_entropy(distribution{1,1,0}, 1.5));
    ASSERT_DOUBLE_EQ(1.0, renyi_entropy(distribution{1,1,0}, 2.0));

    ASSERT_DOUBLE_EQ(1.584962500721156,   renyi_entropy(distribution{2,2,1}, 0.0));
    ASSERT_DOUBLE_EQ(1.5515754959922856,  renyi_entropy(distribution{2,2,1}, 0.5));
    ASSERT_DOUBLE_EQ(1.5219280948873621,  renyi_entropy(distribution{2,2,1}, 1.0));
    ASSERT_DOUBLE_EQ(1.49610312250368341, renyi_entropy(distribution{2,2,1}, 1.5));
    ASSERT_DOUBLE_EQ(1.4739311883324118,  renyi_entropy(distribution{2,2,1}, 2.0));
}
