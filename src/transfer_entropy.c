// Copyright 2016-2018 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/shannon.h>
#include <inform/transfer_entropy.h>
#include <string.h>

static void accumulate_observations(int const *src, int const *dst,
    int const *back, size_t l, size_t n, size_t m, int b, size_t k,
    inform_dist *states, inform_dist *histories, inform_dist *sources,
    inform_dist *predicates)
{
    for (size_t i = 0; i < n; ++i, src += m, dst += m)
    {
        uint64_t src_state, future, state, source, predicate, back_state;
        uint64_t history = 0, q = 1;
        for (size_t j = 0; j < k; ++j)
        {
            q *= b;
            history *= b;
            history += dst[j];
        }
        for (size_t j = k; j < m; ++j)
        {
            back_state = 0;
            for (size_t u = 0; u < l; ++u)
            {
                back_state = b * back_state + back[j+n*(i+m*u)-1];
            }
            history += back_state * q;

            src_state = src[j-1];
            future    = dst[j];
            source    = history * b + src_state;
            predicate = history * b + future;
            state     = predicate * b + src_state;

            states->histogram[state]++;
            histories->histogram[history]++;
            sources->histogram[source]++;
            predicates->histogram[predicate]++;

            history = predicate - (dst[j - k] + back_state * b) * q;
        }
    }
}

static void accumulate_local_observations(int const *src, int const *dst,
    int const *back, size_t l, size_t n, size_t m, int b, size_t k,
    inform_dist *states, inform_dist *histories, inform_dist *sources,
    inform_dist *predicates, uint64_t *state, uint64_t *history, uint64_t *source,
    uint64_t *predicate)
{
    for (size_t i = 0; i < n; ++i)
    {
        history[0] = 0;
        uint64_t q = 1;
        for (size_t j = 0; j < k; ++j)
        {
            q *= b;
            history[0] *= b;
            history[0] += dst[j];
        }
        for (size_t j = k; j < m; ++j)
        {
            size_t z = j - k;
            uint64_t back_state = 0;
            for (size_t u = 0; u < l; ++u)
            {
                back_state = b * back_state + back[j+n*(i+m*u)-1];
            }
            history[z] += back_state * q;
            uint64_t src_state = src[j-1];
            uint64_t future    = dst[j];
            predicate[z]       = history[z] * b + future;
            state[z]           = predicate[z] * b + src_state;
            source[z]          = history[z] * b + src_state;

            states->histogram[state[z]]++;
            histories->histogram[history[z]]++;
            sources->histogram[source[z]]++;
            predicates->histogram[predicate[z]]++;

            if (j + 1 != m)
            {
                history[z + 1] = predicate[z] - (dst[z] + back_state * b) * q;
            }
        }
        src += m;
        dst += m;
        state += (m - k);
        history += (m - k);
        source += (m - k);
        predicate += (m - k);
    }
}

static bool check_arguments(int const *src, int const *dst, int const *back, 
    size_t l, size_t n, size_t m, int b, size_t k, inform_error *err)
{
    if (src == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ETIMESERIES, true);
    }
    else if (dst == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ETIMESERIES, true);
    }
    else if (back == NULL && l != 0)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOSOURCES, true);
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
    else if (m <= k)
    {
        INFORM_ERROR_RETURN(err, INFORM_EKLONG, true);
    }
    else if (k == 0)
    {
        INFORM_ERROR_RETURN(err, INFORM_EKZERO, true);
    }
    for (size_t i = 0; i < n * m; ++i)
    {
        if (b <= src[i] || b <= dst[i])
        {
            INFORM_ERROR_RETURN(err, INFORM_EBADSTATE, true);
        }
        else if (src[i] < 0 || dst[i] < 0)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENEGSTATE, true);
        }
    }
    if (back != NULL)
    {
        for (size_t i = 0; i < l; ++i)
        {
            for (size_t j = 0; j < n * m; ++j)
            {
                if (b <= back[j + n*m*i])
                {
                    INFORM_ERROR_RETURN(err, INFORM_EBADSTATE, true);
                }
                else if (back[j + n*m*i] < 0)
                {
                    INFORM_ERROR_RETURN(err, INFORM_ENEGSTATE, true);
                }
            }
        }
    }
    return false;
}

double inform_transfer_entropy(int const *src, int const *dst, int const *back,
    size_t l, size_t n, size_t m, int b, size_t k, inform_error *err)
{
    if (check_arguments(src, dst, back, l, n, m, b, k, err)) return NAN;

    size_t const N = n * (m - k);

    size_t const q = (size_t) pow((double) b, (double) k);
    size_t const r = (size_t) pow((double) b, (double) l);
    size_t const states_size     = b*b*q*r;
    size_t const histories_size  = q*r;
    size_t const sources_size    = b*q*r;
    size_t const predicates_size = b*q*r;
    size_t const total_size = states_size + histories_size + sources_size + predicates_size;

    uint32_t *data = calloc(total_size, sizeof(uint32_t));
    if (data == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NAN);
    }

    inform_dist states     = { data, states_size, N };
    inform_dist histories  = { data + states_size, histories_size, N };
    inform_dist sources    = { data + states_size + histories_size, sources_size, N };
    inform_dist predicates = { data + states_size + histories_size + sources_size, predicates_size, N };

    accumulate_observations(src, dst, back, l, n, m, b, k, &states, &histories,
        &sources, &predicates);


    double te = 0.0;
    uint64_t predicate, source, state;
    double n_state, n_source, n_predicate, n_history;
    for (uint64_t history = 0; history < histories_size; ++history)
    {
        n_history = histories.histogram[history];
        if (n_history == 0)
        {
            continue;
        }
        for (uint64_t future = 0; future < (uint64_t) b; ++future)
        {
            predicate = history * b + future;
            n_predicate = predicates.histogram[predicate];
            if (n_predicate == 0)
            {
                continue;
            }
            for (uint64_t src_state = 0; src_state < (uint64_t) b; ++src_state)
            {
                source = history * b + src_state;
                n_source = sources.histogram[source];
                if (n_source == 0)
                {
                    continue;
                }
                state = predicate * b + src_state;
                n_state = states.histogram[state];
                if (n_state == 0)
                {
                    continue;
                }
                te += n_state * log2((n_state * n_history) / (n_source * n_predicate));
            }
        }
    }

    free(data);

    return te / N;
}

double *inform_local_transfer_entropy(int const *src, int const *dst,
    int const *back, size_t l, size_t n, size_t m, int b, size_t k, double *te,
    inform_error *err)
{
    if (check_arguments(src, dst, back, l, n, m, b, k, err)) return NULL;

    size_t const N = n * (m - k);

    bool allocate = (te == NULL);
    if (allocate)
    {
        te = malloc(N * sizeof(double));
        if (te == NULL)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
        }
    }

    size_t const q = (size_t) pow((double) b, (double) k);
    size_t const r = (size_t) pow((double) b, (double) l);
    size_t const states_size     = b*b*q*r;
    size_t const histories_size  = q*r;
    size_t const sources_size    = b*q*r;
    size_t const predicates_size = b*q*r;
    size_t const total_size = states_size + histories_size + sources_size + predicates_size;

    uint32_t *histogram_data = calloc(total_size, sizeof(uint32_t));
    if (histogram_data == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
    }

    inform_dist states     = { histogram_data, states_size, N };
    inform_dist histories  = { histogram_data + states_size, histories_size, N };
    inform_dist sources    = { histogram_data + states_size + histories_size, sources_size, N };
    inform_dist predicates = { histogram_data + states_size + histories_size + sources_size, predicates_size, N };

    uint64_t *state_data = malloc(4 * N * sizeof(uint64_t));
    if (state_data == NULL)
    {
        if (allocate) free(te);
        free(histogram_data);
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
    }
    uint64_t *state     = state_data;
    uint64_t *history   = state + N;
    uint64_t *source    = history + N;
    uint64_t *predicate = source + N;

    accumulate_local_observations(src, dst, back, l, n, m, b, k, &states,
        &histories, &sources, &predicates, state, history, source, predicate);

    double s, t, u, v;
    for (size_t i = 0; i < N; ++i)
    {
        s = states.histogram[state[i]];
        t = sources.histogram[source[i]];
        u = predicates.histogram[predicate[i]];
        v = histories.histogram[history[i]];
        te[i] = log2((s*v)/(t*u));
    }

    free(state_data);
    free(histogram_data);

    return te;
}
