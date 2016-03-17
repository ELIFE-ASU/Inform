// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include "network.h"

#include <inform/time_series.h>
#include <iostream>

auto main() -> int
{
    auto net = network("examples/assets/fission-yeast-nodes.txt", "examples/assets/fission-yeast-edges.txt");

    auto const N = 9;
    auto const M = 1 << N;
    auto const steps = 20;
    bool time_series[N][M][steps];

    for (auto init = 0; init < M; ++init)
    {
        net.initialize(init);

        for (size_t j = 0; j < steps; ++j)
        {
            auto const& state = net.node_states();
            for (size_t i = 0; i < N; ++i)
            {
                time_series[i][init][j] = state.at(i);
            }

            net.update();
        }
    }

    std::cout << "Active Information:" << std::endl;
    for (size_t i = 0; i < N; ++i)
    {
        auto const ai = inform::active_information(*std::begin(time_series[i]), *std::end(time_series[i]), steps, 5);
        std::printf("\tNode %d\t=> %f\n", static_cast<int>(i), ai);
    }

    return 0;
}
