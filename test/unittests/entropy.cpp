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
    ASSERT_THROW(inform::shannon_entropy(dist,10), std::invalid_argument);
}

TEST(ShannonEntropy, InvalidBase)
{
    using namespace inform;

    ASSERT_THROW(shannon_entropy(distribution({1,1,1,1,1}),  1), std::invalid_argument);
    ASSERT_THROW(shannon_entropy(distribution({1,1,1,1,1}),  0), std::invalid_argument);
    ASSERT_THROW(shannon_entropy(distribution({1,1,1,1,1}), -1), std::invalid_argument);
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
    ASSERT_DOUBLE_EQ(1.0, shannon_entropy(distribution{1,1,0}));
    ASSERT_DOUBLE_EQ(1.5219280948873621, shannon_entropy(distribution{2,2,1}));
}

TEST(ShannonEntropy, BaseTen)
{
    using namespace inform;
    auto const base = 10;
    ASSERT_DOUBLE_EQ(0.27643459094367495, shannon_entropy(distribution{2,1}, base));
    ASSERT_DOUBLE_EQ(0.30102999566398114, shannon_entropy(distribution{1,1,0}, base));
    ASSERT_DOUBLE_EQ(0.4581460078048337, shannon_entropy(distribution{2,2,1}, base));
}
