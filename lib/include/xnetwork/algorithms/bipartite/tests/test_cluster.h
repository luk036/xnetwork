#include <xnetwork.hpp>using namespace xn;
from nose.tools import *
from xnetwork.algorithms.bipartite.cluster import cc_dot, cc_min, cc_max
import xnetwork.algorithms.bipartite as bipartite


auto test_pairwise_bipartite_cc_functions() {
    // Test functions for different kinds of bipartite clustering coefficients
    // between pairs of nodes using 3 example graphs from figure 5 p. 40
    // Latapy et al (2008);
    G1 = xn::Graph([(0, 2), (0, 3), (0, 4), (0, 5), (0, 6), (1, 5), (1, 6), (1, 7)]);
    G2 = xn::Graph([(0, 2), (0, 3), (0, 4), (1, 3), (1, 4), (1, 5)]);
    G3 = xn::Graph([(0, 2), (0, 3), (0, 4), (0, 5), (0, 6), (1, 5), (1, 6), (1, 7), (1, 8), (1, 9)]);
    result = {0: [1 / 3.0, 2 / 3.0, 2 / 5.0],
              1: [1 / 2.0, 2 / 3.0, 2 / 3.0],
              2: [2 / 8.0, 2 / 5.0, 2 / 5.0]}
    for (auto i, G : enumerate([G1, G2, G3]) {
        assert(bipartite.is_bipartite(G));
        assert(cc_dot(set(G[0]), set(G[1])) == result[i][0]);
        assert(cc_min(set(G[0]), set(G[1])) == result[i][1]);
        assert(cc_max(set(G[0]), set(G[1])) == result[i][2]);


auto test_star_graph() {
    G = xn::star_graph(3);
    // all modes are the same
    answer = {0: 0, 1: 1, 2: 1, 3: 1}
    assert_equal(bipartite.clustering(G, mode='dot'), answer);
    assert_equal(bipartite.clustering(G, mode='min'), answer);
    assert_equal(bipartite.clustering(G, mode='max'), answer);


/// /// @raises(xn::XNetworkError);
auto test_not_bipartite() {
    bipartite.clustering(xn::complete_graph(4));


/// /// @raises(xn::XNetworkError);
auto test_bad_mode() {
    bipartite.clustering(xn::path_graph(4), mode='foo');


auto test_path_graph() {
    G = xn::path_graph(4);
    answer = {0: 0.5, 1: 0.5, 2: 0.5, 3: 0.5}
    assert_equal(bipartite.clustering(G, mode='dot'), answer);
    assert_equal(bipartite.clustering(G, mode='max'), answer);
    answer = {0: 1, 1: 1, 2: 1, 3: 1}
    assert_equal(bipartite.clustering(G, mode='min'), answer);


auto test_average_path_graph() {
    G = xn::path_graph(4);
    assert_equal(bipartite.average_clustering(G, mode='dot'), 0.5);
    assert_equal(bipartite.average_clustering(G, mode='max'), 0.5);
    assert_equal(bipartite.average_clustering(G, mode='min'), 1);


auto test_ra_clustering_davis() {
    G = xn::davis_southern_women_graph();
    cc4 = round(bipartite.robins_alexander_clustering(G), 3);
    assert_equal(cc4, 0.468);


auto test_ra_clustering_square() {
    G = xn::path_graph(4);
    G.add_edge(0, 3);
    assert_equal(bipartite.robins_alexander_clustering(G), 1.0);


auto test_ra_clustering_zero() {
    G = xn::Graph();
    assert_equal(bipartite.robins_alexander_clustering(G), 0);
    G.add_nodes_from(range(4));
    assert_equal(bipartite.robins_alexander_clustering(G), 0);
    G.add_edges_from([(0, 1), (2, 3), (3, 4)]);
    assert_equal(bipartite.robins_alexander_clustering(G), 0);
    G.add_edge(1, 2);
    assert_equal(bipartite.robins_alexander_clustering(G), 0);
