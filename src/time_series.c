// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/state_encoding.h>
#include <inform/time_series.h>

static void inform_active_info_dist(int const* series, size_t n,
                                    uint64_t k, inform_dist *states,
                                    inform_dist *histories,
                                    inform_dist *futures)
{
    int const *last   = series + n;
    int const *future = series + k;
    while (future != last)
    {
        uint64_t const history = inform_encode(series, k);
        uint64_t const state   = history ^ (*future << k);

        inform_dist_tick(states, state);
        inform_dist_tick(histories, history);
        inform_dist_tick(futures, *future);

        ++series;
        ++future;
    }
}

entropy inform_active_info(int const *series, size_t n, uint64_t k)
{
    if (n <= 1)
    {
        return nan("1");
    }
    else if (n <= k)
    {
        return nan("2");
    }

    inform_dist *states    = inform_dist_alloc(2 << k);
    inform_dist *histories = inform_dist_alloc(1 << k);
    inform_dist *futures   = inform_dist_alloc(2);

    inform_active_info_dist(series, n, k, states, histories, futures);
    entropy ai = inform_mutual_info(states, histories, futures);

    inform_dist_free(futures);
    inform_dist_free(histories);
    inform_dist_free(states);

    return ai;
}

entropy inform_active_info_ensemble(int const *series, size_t n, size_t m,
        uint64_t k)
{
    if (n <= 1)
    {
        return nan("1");
    }
    else if (n <= k)
    {
        return nan("2");
    }
    else if (n % m != 0)
    {
        return nan("3");
    }

    inform_dist *states    = inform_dist_alloc(2 << k);
    inform_dist *histories = inform_dist_alloc(1 << k);
    inform_dist *futures   = inform_dist_alloc(2);

    int const *last = series + n;
    while (series != last)
    {
        inform_active_info_dist(series, m, k, states, histories, futures);
        series += m;
    }
    entropy ai = inform_mutual_info(states, histories, futures);

    inform_dist_free(futures);
    inform_dist_free(histories);
    inform_dist_free(states);

    return ai;
}
