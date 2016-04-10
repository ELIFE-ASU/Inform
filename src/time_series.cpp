// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/time_series.h>

double inform_active_info(int64_t const* array, size_t n, uint64_t k)
{
	return inform::active_information(array, array + n, k);
}

double inform_active_info_ensemble(int64_t const* array, size_t n, size_t steps, uint64_t k)
{
	return inform::active_information(array, array + n, steps, k);
}
