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

TEST(TsallisEntropy, InvalidDistribution)
{
    auto const dist = inform::distribution(5);
    ASSERT_THROW(inform::tsallis_entropy(dist, 2.0), std::invalid_argument);
}

TEST(TsallisEntropy, Uniform)
{
    auto const dist = inform::distribution({1,1,1,1,1});
    ASSERT_DOUBLE_EQ(41.33333333333333,  inform::tsallis_entropy(dist, -2.0));
    ASSERT_DOUBLE_EQ(21.960679774997892, inform::tsallis_entropy(dist, -1.5));
    ASSERT_DOUBLE_EQ(12.0,               inform::tsallis_entropy(dist, -1.0));
    ASSERT_DOUBLE_EQ(6.786893258332633,  inform::tsallis_entropy(dist, -0.5));
    ASSERT_DOUBLE_EQ(4.0,                inform::tsallis_entropy(dist,  0.0));
    ASSERT_DOUBLE_EQ(2.4721359549995796, inform::tsallis_entropy(dist,  0.5));
    ASSERT_DOUBLE_EQ(1.6094379124341003, inform::tsallis_entropy(dist,  1.0));
    ASSERT_DOUBLE_EQ(1.105572809000084,  inform::tsallis_entropy(dist,  1.5));
    ASSERT_DOUBLE_EQ(0.7999999999999999, inform::tsallis_entropy(dist,  2.0));
}

TEST(TsallisEntropy, DeltaFunction)
{
    auto const dist = inform::distribution({0,1,0,0,0});
    ASSERT_TRUE(std::isinf(inform::tsallis_entropy(dist, -2.0)));
    ASSERT_TRUE(std::isinf(inform::tsallis_entropy(dist, -1.5)));
    ASSERT_TRUE(std::isinf(inform::tsallis_entropy(dist, -1.0)));
    ASSERT_TRUE(std::isinf(inform::tsallis_entropy(dist, -0.5)));
    ASSERT_DOUBLE_EQ(4.0, inform::tsallis_entropy(dist,  0.0));
    ASSERT_DOUBLE_EQ(0.0, inform::tsallis_entropy(dist,  0.5));
    ASSERT_DOUBLE_EQ(0.0, inform::tsallis_entropy(dist,  1.0));
    ASSERT_DOUBLE_EQ(0.0, inform::tsallis_entropy(dist,  1.5));
    ASSERT_DOUBLE_EQ(0.0, inform::tsallis_entropy(dist,  2.0));
}

TEST(TsallisEntropy, NonUniform)
{
    using namespace inform;
    ASSERT_DOUBLE_EQ(1.0, tsallis_entropy(distribution{2,1}, 0.0));
    ASSERT_DOUBLE_EQ(3.4166666666666665, tsallis_entropy(distribution{2,1}, -2.0));
    ASSERT_DOUBLE_EQ(2.4133078919176065, tsallis_entropy(distribution{2,1}, -1.5));
    ASSERT_DOUBLE_EQ(1.75,               tsallis_entropy(distribution{2,1}, -1.0));
    ASSERT_DOUBLE_EQ(1.304530452640311,  tsallis_entropy(distribution{2,1}, -0.5));
    ASSERT_DOUBLE_EQ(1.0,                tsallis_entropy(distribution{2,1},  0.0));
    ASSERT_DOUBLE_EQ(0.7876937002347035, tsallis_entropy(distribution{2,1},  0.5));
    ASSERT_DOUBLE_EQ(0.6365141682948129, tsallis_entropy(distribution{2,1},  1.0));
    ASSERT_DOUBLE_EQ(0.5264377126366149, tsallis_entropy(distribution{2,1},  1.5));
    ASSERT_DOUBLE_EQ(0.4444444444444444, tsallis_entropy(distribution{2,1},  2.0));

    ASSERT_TRUE(std::isinf(tsallis_entropy(distribution{1,1,0}, -2.0)));
    ASSERT_TRUE(std::isinf(tsallis_entropy(distribution{1,1,0}, -1.5)));
    ASSERT_TRUE(std::isinf(tsallis_entropy(distribution{1,1,0}, -1.0)));
    ASSERT_TRUE(std::isinf(tsallis_entropy(distribution{1,1,0}, -0.5)));
    ASSERT_DOUBLE_EQ(2.0,                tsallis_entropy(distribution{1,1,0},  0.0));
    ASSERT_DOUBLE_EQ(0.8284271247461903, tsallis_entropy(distribution{1,1,0},  0.5));
    ASSERT_DOUBLE_EQ(0.6931471805599453, tsallis_entropy(distribution{1,1,0},  1.0));
    ASSERT_DOUBLE_EQ(0.5857864376269051, tsallis_entropy(distribution{1,1,0},  1.5));
    ASSERT_DOUBLE_EQ(0.5,                tsallis_entropy(distribution{1,1,0},  2.0));

    ASSERT_DOUBLE_EQ(12.166666666666664, tsallis_entropy(distribution{2,2,1}, -2.0));
    ASSERT_DOUBLE_EQ(7.2344136151679574, tsallis_entropy(distribution{2,2,1}, -1.5));
    ASSERT_DOUBLE_EQ(4.5,                tsallis_entropy(distribution{2,2,1}, -1.0));
    ASSERT_DOUBLE_EQ(2.9322304251121127, tsallis_entropy(distribution{2,2,1}, -0.5));
    ASSERT_DOUBLE_EQ(2.0,                tsallis_entropy(distribution{2,2,1},  0.0));
    ASSERT_DOUBLE_EQ(1.4242493191346193, tsallis_entropy(distribution{2,2,1},  0.5));
    ASSERT_DOUBLE_EQ(1.054920167986144, tsallis_entropy(distribution{2,2,1},  1.0));
    ASSERT_DOUBLE_EQ(0.8091857105461353, tsallis_entropy(distribution{2,2,1},  1.5));
    ASSERT_DOUBLE_EQ(0.6399999999999999, tsallis_entropy(distribution{2,2,1},  2.0));
}

TEST(ConditionalEntropy, SelfCondition)
{
    using namespace inform;

    auto const uniform = distribution{1,1,1,1,1};
    ASSERT_DOUBLE_EQ(0.0, conditional_entropy(uniform, uniform));

    auto const delta = distribution{0,1,0};
    ASSERT_DOUBLE_EQ(0.0, conditional_entropy(delta, delta));

    auto const unfair_coin = distribution{2,1};
    ASSERT_DOUBLE_EQ(0.0, conditional_entropy(unfair_coin, unfair_coin));
}

TEST(ConditionalEntropy, Uniform)
{
    using namespace inform;

    distribution const joint     = {50,32,18,0,28,12,31,29,12,30,30,28,17,18,21,44};
    distribution const condition = {1,1,1,1};
    ASSERT_NEAR(1.797915, conditional_entropy(joint, condition), 1e-6);
}

TEST(ConditionalEntropy, NonUniform)
{
    using namespace inform;

    distribution const joint     = {100,64,36,0,28,12,31,29,12,30,30,28,17,18,21,44};
    distribution const condition = {2,1,1,1};
    ASSERT_NEAR(1.732600, conditional_entropy(joint, condition), 1e-6);
}
