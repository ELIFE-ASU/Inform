// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <gtest/gtest.h>

#include <inform/time_series.h>

TEST(ActiveInformation, SingleSeries_TooShort)
{
    using namespace std;
    auto const series = vector<bool>{1,1,0,0,1,0,0,1};
    ASSERT_THROW(inform::active_information(begin(series), begin(series), 2), std::invalid_argument);
    ASSERT_THROW(inform::active_information(begin(series), begin(series)+1, 2), std::invalid_argument);
}

TEST(ActiveInformation, SingleSeries_InvalidHistory)
{
    using namespace std;
    auto const series = vector<bool>{1,1,0,0,1,0,0,1};
    ASSERT_THROW(inform::active_information(begin(series), begin(series)+2, 2), std::invalid_argument);
    ASSERT_THROW(inform::active_information(begin(series), begin(series)+3, 2), std::invalid_argument);
    ASSERT_NO_THROW(inform::active_information(begin(series), begin(series)+4, 2));
    ASSERT_THROW(inform::active_information(begin(series), begin(series)+3, 3), std::invalid_argument);
    ASSERT_THROW(inform::active_information(begin(series), begin(series)+4, 3), std::invalid_argument);
}

TEST(ActiveInformation, SingleSeries)
{
    using namespace std;
    auto const series = vector<bool>{1,1,0,0,1,0,0,1};
    ASSERT_NEAR(0.918296, inform::active_information(begin(series), end(series), 2), 1e-6);

    bool series_array[] = {1,1,0,0,1,0,0,1};
    ASSERT_NEAR(0.918296, inform::active_information(begin(series_array), end(series_array), 2), 1e-6);
}

TEST(ActiveInformation, EnsembleSeries)
{
    using namespace std;
    auto const series = vector<bool>{
        1,1,0,0,1,0,0,1,
        0,0,0,1,0,0,0,1,
    };

    ASSERT_NEAR(0.459148, inform::active_information(begin(series), end(series), 8, 2), 1e-6);
}
