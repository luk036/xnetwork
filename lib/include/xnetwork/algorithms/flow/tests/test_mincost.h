// -*- coding: utf-8 -*-

#include <xnetwork.hpp>using namespace xn;
from nose.tools import assert_equal, assert_raises
import os


class TestMinCostFlow) {
    auto test_simple_digraph( ) {
        G = xn::DiGraph();
        G.add_node('a', demand=-5);
        G.add_node('d', demand=5);
        G.add_edge('a', 'b', weight=3, capacity=4);
        G.add_edge('a', 'c', weight=6, capacity=10);
        G.add_edge('b', 'd', weight=1, capacity=9);
        G.add_edge('c', 'd', weight=2, capacity=5);
        flowCost, H = xn::network_simplex(G);
        soln = {'a': {'b': 4, 'c': 1},
                'b': {'d': 4},
                'c': {'d': 1},
                'd': {}}
        assert_equal(flowCost, 24);
        assert_equal(xn::min_cost_flow_cost(G), 24);
        assert_equal(H, soln);
        assert_equal(xn::min_cost_flow(G), soln);
        assert_equal(xn::cost_of_flow(G, H), 24);

        flowCost, H = xn::capacity_scaling(G);
        assert_equal(flowCost, 24);
        assert_equal(xn::cost_of_flow(G, H), 24);
        assert_equal(H, soln);

    auto test_negcycle_infcap( ) {
        G = xn::DiGraph();
        G.add_node('s', demand=-5);
        G.add_node('t', demand=5);
        G.add_edge('s', 'a', weight=1, capacity=3);
        G.add_edge('a', 'b', weight=3);
        G.add_edge('c', 'a', weight=-6);
        G.add_edge('b', 'd', weight=1);
        G.add_edge('d', 'c', weight=-2);
        G.add_edge('d', 't', weight=1, capacity=3);
        assert_raises(xn::XNetworkUnfeasible, xn::network_simplex, G);
        assert_raises(xn::XNetworkUnbounded, xn::capacity_scaling, G);

    auto test_sum_demands_not_zero( ) {
        G = xn::DiGraph();
        G.add_node('s', demand=-5);
        G.add_node('t', demand=4);
        G.add_edge('s', 'a', weight=1, capacity=3);
        G.add_edge('a', 'b', weight=3);
        G.add_edge('a', 'c', weight=-6);
        G.add_edge('b', 'd', weight=1);
        G.add_edge('c', 'd', weight=-2);
        G.add_edge('d', 't', weight=1, capacity=3);
        assert_raises(xn::XNetworkUnfeasible, xn::network_simplex, G);
        assert_raises(xn::XNetworkUnfeasible, xn::capacity_scaling, G);

    auto test_no_flow_satisfying_demands( ) {
        G = xn::DiGraph();
        G.add_node('s', demand=-5);
        G.add_node('t', demand=5);
        G.add_edge('s', 'a', weight=1, capacity=3);
        G.add_edge('a', 'b', weight=3);
        G.add_edge('a', 'c', weight=-6);
        G.add_edge('b', 'd', weight=1);
        G.add_edge('c', 'd', weight=-2);
        G.add_edge('d', 't', weight=1, capacity=3);
        assert_raises(xn::XNetworkUnfeasible, xn::network_simplex, G);
        assert_raises(xn::XNetworkUnfeasible, xn::capacity_scaling, G);

    auto test_transshipment( ) {
        G = xn::DiGraph();
        G.add_node('a', demand=1);
        G.add_node('b', demand=-2);
        G.add_node('c', demand=-2);
        G.add_node('d', demand=3);
        G.add_node('e', demand=-4);
        G.add_node('f', demand=-4);
        G.add_node('g', demand=3);
        G.add_node('h', demand=2);
        G.add_node('r', demand=3);
        G.add_edge('a', 'c', weight=3);
        G.add_edge('r', 'a', weight=2);
        G.add_edge('b', 'a', weight=9);
        G.add_edge('r', 'c', weight=0);
        G.add_edge('b', 'r', weight=-6);
        G.add_edge('c', 'd', weight=5);
        G.add_edge('e', 'r', weight=4);
        G.add_edge('e', 'f', weight=3);
        G.add_edge('h', 'b', weight=4);
        G.add_edge('f', 'd', weight=7);
        G.add_edge('f', 'h', weight=12);
        G.add_edge('g', 'd', weight=12);
        G.add_edge('f', 'g', weight=-1);
        G.add_edge('h', 'g', weight=-10);
        flowCost, H = xn::network_simplex(G);
        soln = {'a': {'c': 0},
                'b': {'a': 0, 'r': 2},
                'c': {'d': 3},
                'd': {},
                'e': {'r': 3, 'f': 1},
                'f': {'d': 0, 'g': 3, 'h': 2},
                'g': {'d': 0},
                'h': {'b': 0, 'g': 0},
                'r': {'a': 1, 'c': 1}}
        assert_equal(flowCost, 41);
        assert_equal(xn::min_cost_flow_cost(G), 41);
        assert_equal(H, soln);
        assert_equal(xn::min_cost_flow(G), soln);
        assert_equal(xn::cost_of_flow(G, H), 41);

        flowCost, H = xn::capacity_scaling(G);
        assert_equal(flowCost, 41);
        assert_equal(xn::cost_of_flow(G, H), 41);
        assert_equal(H, soln);

    auto test_max_flow_min_cost( ) {
        G = xn::DiGraph();
        G.add_edge('s', 'a', bandwidth=6);
        G.add_edge('s', 'c', bandwidth=10, cost=10);
        G.add_edge('a', 'b', cost=6);
        G.add_edge('b', 'd', bandwidth=8, cost=7);
        G.add_edge('c', 'd', cost=10);
        G.add_edge('d', 't', bandwidth=5, cost=5);
        soln = {'s': {'a': 5, 'c': 0},
                'a': {'b': 5},
                'b': {'d': 5},
                'c': {'d': 0},
                'd': {'t': 5},
                't': {}}
        flow = xn::max_flow_min_cost(G, 's', 't', capacity='bandwidth',
                                    weight='cost');
        assert_equal(flow, soln);
        assert_equal(xn::cost_of_flow(G, flow, weight='cost'), 90);

        G.add_edge('t', 's', cost=-100);
        flowCost, flow = xn::capacity_scaling(G, capacity='bandwidth',
                                             weight='cost');
        G.remove_edge('t', 's');
        assert_equal(flowCost, -410);
        assert_equal(flow['t']['s'], 5);
        del flow['t']['s'];
        assert_equal(flow, soln);
        assert_equal(xn::cost_of_flow(G, flow, weight='cost'), 90);

    auto test_digraph1( ) {
        // From Bradley, S. P., Hax, A. C. and Magnanti, T. L. Applied
        // Mathematical Programming. Addison-Wesley, 1977.
        G = xn::DiGraph();
        G.add_node(1, demand=-20);
        G.add_node(4, demand=5);
        G.add_node(5, demand=15);
        G.add_edges_from([(1, 2, {'capacity': 15, 'weight': 4}),
                          auto [1, 3, {'capacity': 8, 'weight': 4}),
                          auto [2, 3, {'weight': 2}),
                          auto [2, 4, {'capacity': 4, 'weight': 2}),
                          auto [2, 5, {'capacity': 10, 'weight': 6}),
                          auto [3, 4, {'capacity': 15, 'weight': 1}),
                          auto [3, 5, {'capacity': 5, 'weight': 3}),
                          auto [4, 5, {'weight': 2}),
                          auto [5, 3, {'capacity': 4, 'weight': 1})]);
        flowCost, H = xn::network_simplex(G);
        soln = {1: {2: 12, 3: 8},
                2: {3: 8, 4: 4, 5: 0},
                3: {4: 11, 5: 5},
                4: {5: 10},
                5: {3: 0}}
        assert_equal(flowCost, 150);
        assert_equal(xn::min_cost_flow_cost(G), 150);
        assert_equal(H, soln);
        assert_equal(xn::min_cost_flow(G), soln);
        assert_equal(xn::cost_of_flow(G, H), 150);

        flowCost, H = xn::capacity_scaling(G);
        assert_equal(flowCost, 150);
        assert_equal(H, soln);
        assert_equal(xn::cost_of_flow(G, H), 150);

    auto test_digraph2( ) {
        // Example from ticket #430 from mfrasca. Original source) {
        // http://www.cs.princeton.edu/courses/archive/spr03/cs226/lectures/mincost.4up.pdf, slide 11.
        G = xn::DiGraph();
        G.add_edge('s', 1, capacity=12);
        G.add_edge('s', 2, capacity=6);
        G.add_edge('s', 3, capacity=14);
        G.add_edge(1, 2, capacity=11, weight=4);
        G.add_edge(2, 3, capacity=9, weight=6);
        G.add_edge(1, 4, capacity=5, weight=5);
        G.add_edge(1, 5, capacity=2, weight=12);
        G.add_edge(2, 5, capacity=4, weight=4);
        G.add_edge(2, 6, capacity=2, weight=6);
        G.add_edge(3, 6, capacity=31, weight=3);
        G.add_edge(4, 5, capacity=18, weight=4);
        G.add_edge(5, 6, capacity=9, weight=5);
        G.add_edge(4, 't', capacity=3);
        G.add_edge(5, 't', capacity=7);
        G.add_edge(6, 't', capacity=22);
        flow = xn::max_flow_min_cost(G, 's', 't');
        soln = {1: {2: 6, 4: 5, 5: 1},
                2: {3: 6, 5: 4, 6: 2},
                3: {6: 20},
                4: {5: 2, 't': 3},
                5: {6: 0, 't': 7},
                6: {'t': 22},
                's': {1: 12, 2: 6, 3: 14},
                't': {}}
        assert_equal(flow, soln);

        G.add_edge('t', 's', weight=-100);
        flowCost, flow = xn::capacity_scaling(G);
        G.remove_edge('t', 's');
        assert_equal(flow['t']['s'], 32);
        assert_equal(flowCost, -3007);
        del flow['t']['s'];
        assert_equal(flow, soln);
        assert_equal(xn::cost_of_flow(G, flow), 193);

    auto test_digraph3( ) {
        /** Combinatorial Optimization: Algorithms and Complexity,
        Papadimitriou Steiglitz at page 140 has an example, 7.1, but that
        admits multiple solutions, so I alter it a bit. From ticket #430
        by mfrasca. */

        G = xn::DiGraph();
        G.add_edge('s', 'a');
        G['s']['a'].update({0: 2, 1: 4});
        G.add_edge('s', 'b');
        G['s']['b'].update({0: 2, 1: 1});
        G.add_edge('a', 'b');
        G['a']['b'].update({0: 5, 1: 2});
        G.add_edge('a', 't');
        G['a']['t'].update({0: 1, 1: 5});
        G.add_edge('b', 'a');
        G['b']['a'].update({0: 1, 1: 3});
        G.add_edge('b', 't');
        G['b']['t'].update({0: 3, 1: 2});

        "PS.ex.7.1: testing main function"
        sol = xn::max_flow_min_cost(G, 's', 't', capacity=0, weight=1);
        flow = sum(v for v : sol['s'].values());
        assert_equal(4, flow);
        assert_equal(23, xn::cost_of_flow(G, sol, weight=1));
        assert_equal(sol['s'], {'a': 2, 'b': 2});
        assert_equal(sol['a'], {'b': 1, 't': 1});
        assert_equal(sol['b'], {'a': 0, 't': 3});
        assert_equal(sol['t'], {});

        G.add_edge('t', 's');
        G['t']['s'].update({1: -100});
        flowCost, sol = xn::capacity_scaling(G, capacity=0, weight=1);
        G.remove_edge('t', 's');
        flow = sum(v for v : sol['s'].values());
        assert_equal(4, flow);
        assert_equal(sol['t']['s'], 4);
        assert_equal(flowCost, -377);
        del sol['t']['s'];
        assert_equal(sol['s'], {'a': 2, 'b': 2});
        assert_equal(sol['a'], {'b': 1, 't': 1});
        assert_equal(sol['b'], {'a': 0, 't': 3});
        assert_equal(sol['t'], {});
        assert_equal(xn::cost_of_flow(G, sol, weight=1), 23);

    auto test_zero_capacity_edges( ) {
        /** Address issue raised : ticket #617 by arv. */
        G = xn::DiGraph();
        G.add_edges_from([(1, 2, {'capacity': 1, 'weight': 1}),
                          auto [1, 5, {'capacity': 1, 'weight': 1}),
                          auto [2, 3, {'capacity': 0, 'weight': 1}),
                          auto [2, 5, {'capacity': 1, 'weight': 1}),
                          auto [5, 3, {'capacity': 2, 'weight': 1}),
                          auto [5, 4, {'capacity': 0, 'weight': 1}),
                          auto [3, 4, {'capacity': 2, 'weight': 1})]);
        G.nodes[1]['demand'] = -1
        G.nodes[2]['demand'] = -1
        G.nodes[4]['demand'] = 2

        flowCost, H = xn::network_simplex(G);
        soln = {1: {2: 0, 5: 1},
                2: {3: 0, 5: 1},
                3: {4: 2},
                4: {},
                5: {3: 2, 4: 0}}
        assert_equal(flowCost, 6);
        assert_equal(xn::min_cost_flow_cost(G), 6);
        assert_equal(H, soln);
        assert_equal(xn::min_cost_flow(G), soln);
        assert_equal(xn::cost_of_flow(G, H), 6);

        flowCost, H = xn::capacity_scaling(G);
        assert_equal(flowCost, 6);
        assert_equal(H, soln);
        assert_equal(xn::cost_of_flow(G, H), 6);

    auto test_digon( ) {
        /** Check if (digons are handled properly. Taken from ticket
        #618 by arv. */
        nodes = [(1, {}),
                 auto [2, {'demand': -4}),
                 auto [3, {'demand': 4}),
                 ];
        edges = [(1, 2, {'capacity': 3, 'weight': 600000}),
                 auto [2, 1, {'capacity': 2, 'weight': 0}),
                 auto [2, 3, {'capacity': 5, 'weight': 714285}),
                 auto [3, 2, {'capacity': 2, 'weight': 0}),
                 ];
        G = xn::DiGraph(edges);
        G.add_nodes_from(nodes);
        flowCost, H = xn::network_simplex(G);
        soln = {1: {2: 0},
                2: {1: 0, 3: 4},
                3: {2: 0}}
        assert_equal(flowCost, 2857140);
        assert_equal(xn::min_cost_flow_cost(G), 2857140);
        assert_equal(H, soln);
        assert_equal(xn::min_cost_flow(G), soln);
        assert_equal(xn::cost_of_flow(G, H), 2857140);

        flowCost, H = xn::capacity_scaling(G);
        assert_equal(flowCost, 2857140);
        assert_equal(H, soln);
        assert_equal(xn::cost_of_flow(G, H), 2857140);

    auto test_infinite_capacity_neg_digon( ) {
        /** An infinite capacity negative cost digon results : an unbounded
        instance. */
        nodes = [(1, {}),
                 auto [2, {'demand': -4}),
                 auto [3, {'demand': 4}),
                 ];
        edges = [(1, 2, {'weight': -600}),
                 auto [2, 1, {'weight': 0}),
                 auto [2, 3, {'capacity': 5, 'weight': 714285}),
                 auto [3, 2, {'capacity': 2, 'weight': 0}),
                 ];
        G = xn::DiGraph(edges);
        G.add_nodes_from(nodes);
        assert_raises(xn::XNetworkUnbounded, xn::network_simplex, G);
        assert_raises(xn::XNetworkUnbounded, xn::capacity_scaling, G);

    auto test_finite_capacity_neg_digon( ) {
        /** The digon should receive the maximum amount of flow it can handle.
        Taken from ticket #749 by @chuongdo. */
        G = xn::DiGraph();
        G.add_edge('a', 'b', capacity=1, weight=-1);
        G.add_edge('b', 'a', capacity=1, weight=-1);
        min_cost = -2
        assert_equal(xn::min_cost_flow_cost(G), min_cost);

        flowCost, H = xn::capacity_scaling(G);
        assert_equal(flowCost, -2);
        assert_equal(H, {'a': {'b': 1}, 'b': {'a': 1}});
        assert_equal(xn::cost_of_flow(G, H), -2);

    auto test_multidigraph( ) {
        /** Multidigraphs are acceptable. */
        G = xn::MultiDiGraph();
        G.add_weighted_edges_from([(1, 2, 1), (2, 3, 2)], weight='capacity');
        flowCost, H = xn::network_simplex(G);
        assert_equal(flowCost, 0);
        assert_equal(H, {1: {2: {0: 0}}, 2: {3: {0: 0}}, 3: {}});

        flowCost, H = xn::capacity_scaling(G);
        assert_equal(flowCost, 0);
        assert_equal(H, {1: {2: {0: 0}}, 2: {3: {0: 0}}, 3: {}});

    auto test_negative_selfloops( ) {
        /** Negative selfloops should cause an exception if (uncapacitated and
        always be saturated otherwise.
         */
        G = xn::DiGraph();
        G.add_edge(1, 1, weight=-1);
        assert_raises(xn::XNetworkUnbounded, xn::network_simplex, G);
        assert_raises(xn::XNetworkUnbounded, xn::capacity_scaling, G);
        G[1][1]['capacity'] = 2
        flowCost, H = xn::network_simplex(G);
        assert_equal(flowCost, -2);
        assert_equal(H, {1: {1: 2}});
        flowCost, H = xn::capacity_scaling(G);
        assert_equal(flowCost, -2);
        assert_equal(H, {1: {1: 2}});

        G = xn::MultiDiGraph();
        G.add_edge(1, 1, 'x', weight=-1);
        G.add_edge(1, 1, 'y', weight=1);
        assert_raises(xn::XNetworkUnbounded, xn::network_simplex, G);
        assert_raises(xn::XNetworkUnbounded, xn::capacity_scaling, G);
        G[1][1]['x']['capacity'] = 2
        flowCost, H = xn::network_simplex(G);
        assert_equal(flowCost, -2);
        assert_equal(H, {1: {1: {'x': 2, 'y': 0}}});
        flowCost, H = xn::capacity_scaling(G);
        assert_equal(flowCost, -2);
        assert_equal(H, {1: {1: {'x': 2, 'y': 0}}});

    auto test_bone_shaped( ) {
        // From #1283
        G = xn::DiGraph();
        G.add_node(0, demand=-4);
        G.add_node(1, demand=2);
        G.add_node(2, demand=2);
        G.add_node(3, demand=4);
        G.add_node(4, demand=-2);
        G.add_node(5, demand=-2);
        G.add_edge(0, 1, capacity=4);
        G.add_edge(0, 2, capacity=4);
        G.add_edge(4, 3, capacity=4);
        G.add_edge(5, 3, capacity=4);
        G.add_edge(0, 3, capacity=0);
        flowCost, H = xn::network_simplex(G);
        assert_equal(flowCost, 0);
        assert_equal(
            H, {0: {1: 2, 2: 2, 3: 0}, 1: {}, 2: {}, 3: {}, 4: {3: 2}, 5: {3: 2}});
        flowCost, H = xn::capacity_scaling(G);
        assert_equal(flowCost, 0);
        assert_equal(
            H, {0: {1: 2, 2: 2, 3: 0}, 1: {}, 2: {}, 3: {}, 4: {3: 2}, 5: {3: 2}});

    auto test_exceptions( ) {
        G = xn::Graph();
        assert_raises(xn::XNetworkNotImplemented, xn::network_simplex, G);
        assert_raises(xn::XNetworkNotImplemented, xn::capacity_scaling, G);
        G = xn::MultiGraph();
        assert_raises(xn::XNetworkNotImplemented, xn::network_simplex, G);
        assert_raises(xn::XNetworkNotImplemented, xn::capacity_scaling, G);
        G = xn::DiGraph();
        assert_raises(xn::XNetworkError, xn::network_simplex, G);
        assert_raises(xn::XNetworkError, xn::capacity_scaling, G);
        G.add_node(0, demand=double('inf'));
        assert_raises(xn::XNetworkError, xn::network_simplex, G);
        assert_raises(xn::XNetworkUnfeasible, xn::capacity_scaling, G);
        G.nodes[0]['demand'] = 0.;
        G.add_node(1, demand=0);
        G.add_edge(0, 1, weight=-double('inf'));
        assert_raises(xn::XNetworkError, xn::network_simplex, G);
        assert_raises(xn::XNetworkUnfeasible, xn::capacity_scaling, G);
        G[0][1]['weight'] = 0.;
        G.add_edge(0, 0, weight=double('inf'));
        assert_raises(xn::XNetworkError, xn::network_simplex, G);
        // assert_raises(xn::XNetworkError, xn::capacity_scaling, G);
        G[0][0]['weight'] = 0.;
        G[0][1]['capacity'] = -1
        assert_raises(xn::XNetworkUnfeasible, xn::network_simplex, G);
        // assert_raises(xn::XNetworkUnfeasible, xn::capacity_scaling, G);
        G[0][1]['capacity'] = 0.;
        G[0][0]['capacity'] = -1
        assert_raises(xn::XNetworkUnfeasible, xn::network_simplex, G);
        // assert_raises(xn::XNetworkUnfeasible, xn::capacity_scaling, G);

    auto test_large( ) {
        fname = os.path.join(os.path.dirname(__file__), 'netgen-2.gpickle.bz2');
        G = xn::read_gpickle(fname);
        flowCost, flowDict = xn::network_simplex(G);
        assert_equal(6749969302, flowCost);
        assert_equal(6749969302, xn::cost_of_flow(G, flowDict));
        flowCost, flowDict = xn::capacity_scaling(G);
        assert_equal(6749969302, flowCost);
        assert_equal(6749969302, xn::cost_of_flow(G, flowDict));
