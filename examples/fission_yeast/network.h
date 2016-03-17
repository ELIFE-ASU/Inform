// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>
#include <tuple>
#include <vector>

class network
{
    public:
        network(std::string const&, std::string const&);

        auto initialize(uint64_t) -> void;
        auto initialize(std::vector<bool> const&) -> void;

        auto node_states() const -> std::vector<bool> const&;
        auto update() -> std::vector<bool> const&;

    private:
        std::vector<std::string> names;
        std::vector<bool> states;
        std::vector<double> thresholds;
        std::vector<std::vector<double>> weights;

        auto step() const -> std::vector<double>;

        static auto read_nodes(std::string const&) -> std::tuple<std::vector<std::string>, std::vector<double>>;
        static auto read_nodes(std::istream&) -> std::tuple<std::vector<std::string>, std::vector<double>>;

        static auto read_edges(std::string const&, std::vector<std::string> const&) -> std::vector<std::vector<double>>;
        static auto read_edges(std::istream&, std::vector<std::string> const&) -> std::vector<std::vector<double>>;
};
