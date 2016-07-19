// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <stdlib.h>
#include <inttypes.h>

int* random_series(size_t size, int base);

#define AVERAGE(XS) average(XS, sizeof(XS) / sizeof(double))
double average(double const *xs, size_t n);
