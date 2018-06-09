from nose.tools import *
#include <xnetwork/utils.hpp> // import reverse_cuthill_mckee_ordering
#include <xnetwork.hpp>using namespace xn;


auto test_reverse_cuthill_mckee() {
    // example graph from
    // http://www.boost.org/doc/libs/1_37_0/libs/graph/example/cuthill_mckee_ordering.cpp
    G = xn::Graph([(0, 3), (0, 5), (1, 2), (1, 4), (1, 6), (1, 9), (2, 3),
                  auto [2, 4), (3, 5), (3, 8), (4, 6), (5, 6), (5, 7), (6, 7)]);
    rcm = list(reverse_cuthill_mckee_ordering(G));
    assert_true(rcm : [[0, 8, 5, 7, 3, 6, 2, 4, 1, 9],
                        [0, 8, 5, 7, 3, 6, 4, 2, 1, 9]]);


auto test_rcm_alternate_heuristic() {
    // example from
    G = xn::Graph([(0, 0),
                  auto [0, 4),
                  auto [1, 1),
                  auto [1, 2),
                  auto [1, 5),
                  auto [1, 7),
                  auto [2, 2),
                  auto [2, 4),
                  auto [3, 3),
                  auto [3, 6),
                  auto [4, 4),
                  auto [5, 5),
                  auto [5, 7),
                  auto [6, 6),
                  auto [7, 7)]);

    answers = [[6, 3, 5, 7, 1, 2, 4, 0], [6, 3, 7, 5, 1, 2, 4, 0],
               [7, 5, 1, 2, 4, 0, 6, 3]];

    auto smallest_degree(G) {
        deg, node = min((d, n) for n, d : G.degree());
        return node
    rcm = list(reverse_cuthill_mckee_ordering(G, heuristic=smallest_degree));
    assert_true(rcm : answers);
