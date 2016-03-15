// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include <inform/state_encoding.h>

TEST(EncodeState, EmptyState)
{
    using namespace inform;
    ASSERT_THROW(encode_state({}), std::invalid_argument);
}

TEST(EncodeState, LengthOne)
{
    using namespace inform;
    ASSERT_EQ(uint64_t{0}, encode_state({0}));
    ASSERT_EQ(uint64_t{1}, encode_state({1}));
}

TEST(EncodeState, LengthTwo)
{
    using namespace inform;
    ASSERT_EQ(uint64_t{0}, encode_state({0,0}));
    ASSERT_EQ(uint64_t{1}, encode_state({1,0}));
    ASSERT_EQ(uint64_t{2}, encode_state({0,1}));
    ASSERT_EQ(uint64_t{3}, encode_state({1,1}));
}


TEST(EncodeState, LengthThree)
{
    using namespace inform;
    ASSERT_EQ(uint64_t{0}, encode_state({0,0,0}));
    ASSERT_EQ(uint64_t{1}, encode_state({1,0,0}));
    ASSERT_EQ(uint64_t{2}, encode_state({0,1,0}));
    ASSERT_EQ(uint64_t{3}, encode_state({1,1,0}));
    ASSERT_EQ(uint64_t{4}, encode_state({0,0,1}));
    ASSERT_EQ(uint64_t{5}, encode_state({1,0,1}));
    ASSERT_EQ(uint64_t{6}, encode_state({0,1,1}));
    ASSERT_EQ(uint64_t{7}, encode_state({1,1,1}));
}

TEST(EncodeState, StateTooLong)
{
    using namespace inform;
    ASSERT_NO_THROW(encode_state(std::vector<bool>(64)));
    ASSERT_THROW(encode_state(std::vector<bool>(65)), std::invalid_argument);
}

TEST(DecodeState, ReverseEncoding)
{
    using namespace inform;
    auto const M = 10;
    for (uint64_t i = 0; i < (1 << M); ++i)
    {
        ASSERT_EQ(i, encode_state(decode_state<>(i, M)));
    }
}

TEST(DecodeState, TooLong)
{
    using namespace std::placeholders;
    using namespace inform;
    ASSERT_NO_THROW(decode_state<>(~uint64_t{0}, 64));
    ASSERT_THROW(decode_state<>(~uint64_t{0}, 65), std::invalid_argument);
}