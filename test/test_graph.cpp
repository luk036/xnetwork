// -*- coding: utf-8 -*-
#include <cinttypes>
#include <doctest/doctest.h>
#include <py2cpp/py2cpp.hpp>
#include <xnetwork/classes/graph.hpp>

inline auto create_test_case()
{
    using Edge = std::pair<std::string, std::string>;
    std::vector<std::string> nodes = {"A", "B", "C", "D", "E"};
    const auto edges = std::array<Edge, 5> {Edge {"A", "B"}, Edge {"B", "C"},
        Edge {"C", "D"}, Edge {"D", "E"}, Edge {"E", "A"}};
    // constexpr auto weights = std::array<int, 5> {-5, 1, 1, 1, 1};

    auto G = xn::Graph {nodes};
    G.add_edges_from(edges);
    return G;
}

/*!
 * @brief
 *
 */
TEST_CASE("Test xnetwork")
{
    // auto weights = std::array<int, 5> {-5, 1, 1, 1, 1};
    auto G = create_test_case();
    CHECK(G.size() == 5);
    // do_case(G);
    // CHECK(hasNeg);
}

/**
 * @brief Create a test netlist object
 *
 * @return Netlist
 */
TEST_CASE("Test xnetwork")
{
    constexpr auto num_nodes = 6;
    enum nodes
    {
        a1,
        a2,
        a3,
        n1,
        n2,
        n3
    };
    // const auto R = py::range<std::uint8_t>(0, num_nodes);
    auto G = xn::SimpleGraph {num_nodes};
    G.add_edge(a1, n1);
    G.add_edge(a1, n1);
    G.add_edge(a1, n2);
    G.add_edge(a2, n2);

    auto count = 0;
    for ([[maybe_unused]] auto _ : G)
    {
        ++count;
    }

    CHECK(G.number_of_nodes() == count);

    auto deg = 0;
    for ([[maybe_unused]] auto _ : G[a1])
    {
        ++deg;
    }

    CHECK(G.degree(a1) == deg);
}

