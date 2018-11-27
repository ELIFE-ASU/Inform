// Copyright 2016-2018 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/entropy_rate.h>
#include <inform/shannon.h>

static void accumulate_observations(int const* series, size_t n, size_t m,
    int b, size_t k, inform_dist *states, inform_dist *histories)
{
    for (size_t i = 0; i < n; ++i, series += m)
    {
        uint64_t history = 0, q = 1, state, future;
        for (size_t j = 0; j < k; ++j)
        {
            q *= b;
            history *= b;
            history += series[j];
        }
        for (size_t j = k; j < m; ++j)
        {
            future = series[j];
            state  = history * b + future;

            states->histogram[state]++;
            histories->histogram[history]++;

            history = state - series[j - k]*q;
        }
    }
}

static void accumulate_local_observations(int const* series, size_t n, size_t m,
    int b, size_t k, inform_dist *states, inform_dist *histories,
    uint64_t *state, uint64_t *history)
{
    for (size_t i = 0; i < n; ++i)
    {
        uint64_t q = 1;
        history[0] = 0;
        for (size_t j = 0; j < k; ++j)
        {
            q *= b;
            history[0] *= b;
            history[0] += series[j];
        }
        for (size_t j = k; j < m; ++j)
        {
            size_t l = j - k;
            state[l]  = history[l] * b + series[j];

            states->histogram[state[l]]++;
            histories->histogram[history[l]]++;

            if (j + 1 != m)
            {
                history[l + 1] = state[l] - series[l]*q;
            }
        }
        series += m;
        state += (m - k);
        history += (m - k);
    }
}

static bool check_arguments(int const *series, size_t n, size_t m, int b, size_t k, inform_error *err)
{
    if (series == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ETIMESERIES, true);
    }
    else if (n < 1)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOINITS, true);
    }
    else if (m < 2)
    {
        INFORM_ERROR_RETURN(err, INFORM_ESHORTSERIES, true);
    }
    else if (b < 2)
    {
        INFORM_ERROR_RETURN(err, INFORM_EBASE, true);
    }
    else if (k == 0)
    {
        INFORM_ERROR_RETURN(err, INFORM_EKZERO, true);
    }
    else if (m <= k)
    {
        INFORM_ERROR_RETURN(err, INFORM_EKLONG, true);
    }
    for (size_t i = 0; i < n * m; ++i)
    {
        if (series[i] < 0)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENEGSTATE, true);
        }
        else if (b <= series[i])
        {
            INFORM_ERROR_RETURN(err, INFORM_EBADSTATE, true);
        }
    }
    return false;
}

double inform_entropy_rate(int const *series, size_t n, size_t m, int b,
    size_t k, inform_error *err)
{
    if (check_arguments(series, n, m, b, k, err)) return NAN;

    size_t const N = n * (m - k);

    size_t const states_size = (size_t) (b * pow((double) b, (double) k));
    size_t const histories_size = states_size / b;
    size_t const total_size = states_size + histories_size;

    uint32_t *data = calloc(total_size, sizeof(uint32_t));
    if (data == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NAN);
    }

    inform_dist states    = { data, states_size, N };
    inform_dist histories = { data + states_size, histories_size, N };

    accumulate_observations(series, n, m, b, k, &states, &histories);

    double er = inform_shannon_ce(&states, &histories, 2.0);

    free(data);

    return er;
}

double *inform_local_entropy_rate(int const *series, size_t n, size_t m, int b,
    size_t k, double *er, inform_error *err)
{
    if (check_arguments(series, n, m, b, k, err)) return NULL;

    size_t const N = n * (m - k);

    bool allocate_er = (er == NULL);
    if (allocate_er)
    {
        er = malloc(N * sizeof(double));
        if (er == NULL)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
        }
    }

    size_t const states_size = (size_t) (b * pow((double) b, (double) k));
    size_t const histories_size = states_size / b;
    size_t const total_size = states_size + histories_size;

    uint32_t *histogram_data = calloc(total_size, sizeof(uint32_t));
    if (histogram_data == NULL)
    {
        if (allocate_er) free(er);
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
    }

    inform_dist states    = { histogram_data, states_size, N };
    inform_dist histories = { histogram_data + states_size, histories_size, N };


    uint64_t *state_data = malloc(2 * N * sizeof(uint64_t));
    if (state_data == NULL)
    {
        if (allocate_er) free(er);
        free(histogram_data);
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
    }
    uint64_t *state = state_data;
    uint64_t *history = state + N;

    accumulate_local_observations(series, n, m, b, k, &states, &histories,
        state, history);

    double s, h;
    for (size_t i = 0; i < N; ++i)
    {
        s = states.histogram[state[i]];
        h = histories.histogram[history[i]];
        er[i] = log2(h/s);
    }

    free(state_data);
    free(histogram_data);

    return er;
}
