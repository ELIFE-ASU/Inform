// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include "network.h"

#include <inform/time_series.h>
#include <iostream>

auto main() -> int
{
    // Specify that there are 9 nodes
    auto const N = 9;
    // Specify that there are 2^N initial states
    auto const M = 1 << N;
    // We will run the network for 20 time steps for each initial condition
    auto const steps = 20;

    // Construct the array to store the network's time series
    bool time_series[N][M][steps];

    // Read the network from the nodes and edges files
    auto net = network("examples/assets/fission-yeast-nodes.txt", "examples/assets/fission-yeast-edges.txt");
    if (net.node_states().size() != N)
    {
        throw std::runtime_error("main: the network as read does not have the expected number of nodes");
    }

    // For each initial condition
    for (auto init = 0; init < M; ++init)
    {
        // Set the network's initial state
        net.initialize(init);

        // for each step in time
        for (size_t j = 0; j < steps; ++j)
        {
            // get the network state
            auto const& state = net.node_states();
            // add that state to the time series
            for (size_t i = 0; i < N; ++i)
            {
                time_series[i][init][j] = state.at(i);
            }
            // update the network
            net.update();
        }
    }

    // Go ahead and calculate the active information for each node
    std::cout << "Active Information:" << std::endl;
    for (size_t i = 0; i < N; ++i)
    {
        auto const ai = inform::active_information(*std::begin(time_series[i]), *std::end(time_series[i]), steps, 5);
        std::printf("\tNode %d\t=> %f\n", static_cast<int>(i), ai);
    }

    return 0;
}
