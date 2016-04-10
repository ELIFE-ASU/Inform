// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include "network.h"

#include <fstream>
#include <inform/state_encoding.h>
#include <iostream>
#include <regex>

network::network(std::string const& nodesfile, std::string const& edgesfile)
{
    std::tie(names, thresholds) = read_nodes(nodesfile);
    weights = read_edges(edgesfile, names);
    states = std::vector<bool>(names.size());
}

auto network::initialize(uint64_t node_states) -> void
{
    initialize(inform::decode_state<>(node_states, states.size()));
}

auto network::initialize(std::vector<bool> const& node_states) -> void
{
    using namespace std;
    if (node_states.size() != states.size())
    {
        throw std::invalid_argument("network::initialize: incorrect number of node states provided");
    }
    copy(begin(node_states), end(node_states), begin(states));
}

auto network::node_states() const -> std::vector<bool> const&
{
    return states;
}

auto network::update() -> std::vector<bool> const&
{
    auto const floating_states = step();
    for (size_t i = 0; i < states.size(); ++i)
    {
        auto const n = floating_states[i];
        auto const t = thresholds[i];
        if (n < t)
        {
            states[i] = 0;
        }
        else if (n > t)
        {
            states[i] = 1;
        }
    }
    return states;
}

auto network::step() const -> std::vector<double>
{
    std::vector<double> floating_steps(states.size(), 0.0);
    for (size_t i = 0; i < states.size(); ++i)
    {
        for (size_t j = 0; j < states.size(); ++j)
        {
            floating_steps[i] += weights[i][j] * states[j];
        }
    }
    return floating_steps;
}

auto network::read_nodes(std::string const& filename) -> std::tuple<std::vector<std::string>, std::vector<double>>
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        if (file.good())
        {
            return read_nodes(file);
        }
        throw std::runtime_error("network::read_nodes: error reading file: " + filename);
    }
    throw std::runtime_error("network::read_nodes: cannot open file: " + filename);
}

auto network::read_nodes(std::istream& stream) -> std::tuple<std::vector<std::string>, std::vector<double>>
{
    using namespace std;

    static auto const comment_regex = regex("^#.*$");
    static auto const node_regex = regex("^\\s*(\\S+)\\s+(\\S+)\\s*$");

    vector<string> names;
    vector<double> thresholds;

    auto line_number = 0;
    for (string line; getline(stream, line);)
    {
        ++line_number;
        smatch match;
        if (!regex_match(line, match, comment_regex))
        {
            if (regex_match(line, match, node_regex))
            {
                names.push_back(match[1]);
                thresholds.push_back(stod(match[2]));
            }
            else
            {
                std::cerr << ("network::read_nodes: ill-formed line - " + std::to_string(line_number))
                    << std::endl;
            }
        }
    }
    return make_tuple(names, thresholds);
}

auto network::read_edges(std::string const& filename, std::vector<std::string> const& names)
    -> std::vector<std::vector<double>>
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        if (file.good())
        {
            return read_edges(file, names);
        }
        throw std::runtime_error("network::read_edges: error reading file: " + filename);
    }
    throw std::runtime_error("network::read_edges: cannot open file: " + filename);
}

auto network::read_edges(std::istream& stream, std::vector<std::string> const& names)
    -> std::vector<std::vector<double>>
{
    using namespace std;

    static auto const comment_regex = regex("^#.*$");
    static auto const edge_regex = regex("^\\s*(\\S+)\\s+(\\S+)\\s+(\\S+)\\s*$");

    auto weights = vector<vector<double>>(names.size(), vector<double>(names.size(), 0.0));

    auto line_number = 0;
    for (string line; getline(stream, line);)
    {
        ++line_number;
        smatch match;
        if (!regex_match(line, match, comment_regex))
        {
            if (regex_match(line, match, edge_regex))
            {
                auto const i = distance(begin(names), find(begin(names), end(names), match[2]));
                auto const j = distance(begin(names), find(begin(names), end(names), match[1]));
                weights.at(i).at(j) = stod(match[3]);
            }
            else
            {
                std::cerr << ("network::read_edges: ill-formed line - " + std::to_string(line_number))
                    << std::endl;
            }
        }
    }
    return weights;
}
