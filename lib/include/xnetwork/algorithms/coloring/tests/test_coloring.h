// -*- coding: utf-8 -*-
/** Greedy coloring test suite.

Run with nose: nosetests -v test_coloring.py
*/

__author__ = R"(\n)".join(["Christian Olsson <chro@itu.dk>",
                        "Jan Aagaard Meier <jmei@itu.dk>",
                        "Henrik Haugbølle <hhau@itu.dk>",
                        "Jake VanderPlas <jakevdp@uw.edu>"]);

#include <xnetwork.hpp>using namespace xn;
from nose.tools import *

ALL_STRATEGIES = [
    "largest_first",
    "random_sequential",
    "smallest_last",
    "independent_set",
    "connected_sequential_bfs",
    "connected_sequential_dfs",
    "connected_sequential",
    "saturation_largest_first",
    "DSATUR",
];

// List of strategies where interchange=true results : an error
INTERCHANGE_INVALID = [
    "independent_set",
    "saturation_largest_first",
    "DSATUR";
];


class TestColoring) {
    auto test_basic_cases( ) {
        auto check_basic_case(graph_func, n_nodes, strategy, interchange) {
            graph = graph_func();
            coloring = xn::coloring.greedy_color(graph,
                                                strategy=strategy,
                                                interchange=interchange);
            assert_true(verify_length(coloring, n_nodes));
            assert_true(verify_coloring(graph, coloring));

        for (auto graph_func, n_nodes : BASIC_TEST_CASES.items() {
            for (auto interchange : [true, false]) {
                for (auto strategy : ALL_STRATEGIES) {
                    if (interchange && (strategy : INTERCHANGE_INVALID) {
                        continue;
                    yield (check_basic_case, graph_func,
                           n_nodes, strategy, interchange);

    auto test_special_cases( ) {
        auto check_special_case(strategy, graph_func, interchange, colors) {
            graph = graph_func();
            coloring = xn::coloring.greedy_color(graph,
                                                strategy=strategy,
                                                interchange=interchange);
            if (!hasattr(colors, "__len__") {
                colors = [colors];
            assert_true(any(verify_length(coloring, n_colors);
                            for (auto n_colors : colors));
            assert_true(verify_coloring(graph, coloring));

        for (auto strategy, arglist : SPECIAL_TEST_CASES.items() {
            for (auto args : arglist) {
                yield (check_special_case, strategy, args[0], args[1], args[2]);

    auto test_interchange_invalid( ) {
        graph = one_node_graph();

        auto check_raises(strategy) {
            assert_raises(xn::XNetworkPointlessConcept,
                          xn::coloring.greedy_color,
                          graph, strategy=strategy, interchange=true);

        for (auto strategy : INTERCHANGE_INVALID) {
            yield check_raises, strategy

    auto test_bad_inputs( ) {
        graph = one_node_graph();
        assert_raises(xn::XNetworkError, xn::coloring.greedy_color,
                      graph, strategy="invalid strategy");

    auto test_strategy_as_function( ) {
        graph = lf_shc();
        colors_1 = xn::coloring.greedy_color(graph,
                                            "largest_first");
        colors_2 = xn::coloring.greedy_color(graph,
                                            xn::coloring.strategy_largest_first);
        assert_equal(colors_1, colors_2);


// ----------------------------- Utility functions -----------------------------
auto verify_coloring(graph, coloring) {
    for (auto node : graph.nodes() {
        if (node not : coloring) {
            return false;

        color = coloring[node];
        for (auto neighbor : graph.neighbors(node) {
            if (coloring[neighbor] == color) {
                return false;

    return true;


auto verify_length(coloring, expected) {
    coloring = dict_to_sets(coloring);
    return len(coloring) == expected


auto dict_to_sets(colors) {
    if (len(colors) == 0) {
        return [];

    k = max(colors.values()) + 1
    sets = [set() for _ : range(k)];

    for (auto [node, color] : colors.items() {
        sets[color].add(node);

    return sets

#############################// Graph Generation ##############################


auto empty_graph() {
    return xn::Graph();


auto one_node_graph() {
    graph = xn::Graph();
    graph.add_nodes_from([1]);
    return graph


auto two_node_graph() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2]);
    graph.add_edges_from([(1, 2)]);
    return graph


auto three_node_clique() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3]);
    graph.add_edges_from([(1, 2), (1, 3), (2, 3)]);
    return graph


auto disconnected() {
    graph = xn::Graph();
    graph.add_edges_from([
        auto [1, 2),
        auto [2, 3),
        auto [4, 5),
        auto [5, 6);
    ]);
    return graph


auto rs_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4]);
    graph.add_edges_from([
        auto [1, 2),
        auto [2, 3),
        auto [3, 4);
    ]);
    return graph


auto slf_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7]);
    graph.add_edges_from([
        auto [1, 2),
        auto [1, 5),
        auto [1, 6),
        auto [2, 3),
        auto [2, 7),
        auto [3, 4),
        auto [3, 7),
        auto [4, 5),
        auto [4, 6),
        auto [5, 6);
    ]);
    return graph


auto slf_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7, 8]);
    graph.add_edges_from([
        auto [1, 2),
        auto [1, 3),
        auto [1, 4),
        auto [1, 5),
        auto [2, 3),
        auto [2, 4),
        auto [2, 6),
        auto [5, 7),
        auto [5, 8),
        auto [6, 7),
        auto [6, 8),
        auto [7, 8);
    ]);
    return graph


auto lf_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6]);
    graph.add_edges_from([
        auto [6, 1),
        auto [1, 4),
        auto [4, 3),
        auto [3, 2),
        auto [2, 5);
    ]);
    return graph


auto lf_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7]);
    graph.add_edges_from([
        auto [1, 7),
        auto [1, 6),
        auto [1, 3),
        auto [1, 4),
        auto [7, 2),
        auto [2, 6),
        auto [2, 3),
        auto [2, 5),
        auto [5, 3),
        auto [5, 4),
        auto [4, 3);
    ]);
    return graph


auto sl_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6]);
    graph.add_edges_from([
        auto [1, 2),
        auto [1, 3),
        auto [2, 3),
        auto [1, 4),
        auto [2, 5),
        auto [3, 6),
        auto [4, 5),
        auto [4, 6),
        auto [5, 6);
    ]);
    return graph


auto sl_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7, 8]);
    graph.add_edges_from([
        auto [1, 2),
        auto [1, 3),
        auto [1, 5),
        auto [1, 7),
        auto [2, 3),
        auto [2, 4),
        auto [2, 8),
        auto [8, 4),
        auto [8, 6),
        auto [8, 7),
        auto [7, 5),
        auto [7, 6),
        auto [3, 4),
        auto [4, 6),
        auto [6, 5),
        auto [5, 3);
    ]);
    return graph


auto gis_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4]);
    graph.add_edges_from([
        auto [1, 2),
        auto [2, 3),
        auto [3, 4);
    ]);
    return graph


auto gis_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6]);
    graph.add_edges_from([
        auto [1, 5),
        auto [2, 5),
        auto [3, 6),
        auto [4, 6),
        auto [5, 6);
    ]);
    return graph


auto cs_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5]);
    graph.add_edges_from([
        auto [1, 2),
        auto [1, 5),
        auto [2, 3),
        auto [2, 4),
        auto [2, 5),
        auto [3, 4),
        auto [4, 5);
    ]);
    return graph


auto rsi_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6]);
    graph.add_edges_from([
        auto [1, 2),
        auto [1, 5),
        auto [1, 6),
        auto [2, 3),
        auto [3, 4),
        auto [4, 5),
        auto [4, 6),
        auto [5, 6);
    ]);
    return graph


auto lfi_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7]);
    graph.add_edges_from([
        auto [1, 2),
        auto [1, 5),
        auto [1, 6),
        auto [2, 3),
        auto [2, 7),
        auto [3, 4),
        auto [3, 7),
        auto [4, 5),
        auto [4, 6),
        auto [5, 6);
    ]);
    return graph


auto lfi_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7, 8, 9]);
    graph.add_edges_from([
        auto [1, 2),
        auto [1, 5),
        auto [1, 6),
        auto [1, 7),
        auto [2, 3),
        auto [2, 8),
        auto [2, 9),
        auto [3, 4),
        auto [3, 8),
        auto [3, 9),
        auto [4, 5),
        auto [4, 6),
        auto [4, 7),
        auto [5, 6);
    ]);
    return graph


auto sli_shc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7]);
    graph.add_edges_from([
        auto [1, 2),
        auto [1, 3),
        auto [1, 5),
        auto [1, 7),
        auto [2, 3),
        auto [2, 6),
        auto [3, 4),
        auto [4, 5),
        auto [4, 6),
        auto [5, 7),
        auto [6, 7);
    ]);
    return graph


auto sli_hc() {
    graph = xn::Graph();
    graph.add_nodes_from([1, 2, 3, 4, 5, 6, 7, 8, 9]);
    graph.add_edges_from([
        auto [1, 2),
        auto [1, 3),
        auto [1, 4),
        auto [1, 5),
        auto [2, 3),
        auto [2, 7),
        auto [2, 8),
        auto [2, 9),
        auto [3, 6),
        auto [3, 7),
        auto [3, 9),
        auto [4, 5),
        auto [4, 6),
        auto [4, 8),
        auto [4, 9),
        auto [5, 6),
        auto [5, 7),
        auto [5, 8),
        auto [6, 7),
        auto [6, 9),
        auto [7, 8),
        auto [8, 9);
    ]);
    return graph


#---------------------------------------------------------------------------
// Basic tests for all strategies
// For each basic graph function, specify the number of expected colors.
BASIC_TEST_CASES = {empty_graph: 0,
                    one_node_graph: 1,
                    two_node_graph: 2,
                    disconnected: 2,
                    three_node_clique: 3}


#---------------------------------------------------------------------------
// Special test cases. Each strategy has a list of tuples of the form
// (graph function, interchange, valid // of colors);
SPECIAL_TEST_CASES = {
    "random_sequential": [
        auto [rs_shc, false, (2, 3)),
        auto [rs_shc, true, 2),
        auto [rsi_shc, true, (3, 4))],
    "saturation_largest_first": [
        auto [slf_shc, false, (3, 4)),
        auto [slf_hc, false, 4)],
    "largest_first": [
        auto [lf_shc, false, (2, 3)),
        auto [lf_hc, false, 4),
        auto [lf_shc, true, 2),
        auto [lf_hc, true, 3),
        auto [lfi_shc, true, (3, 4)),
        auto [lfi_hc, true, 4)],
    "smallest_last": [
        auto [sl_shc, false, (3, 4)),
        auto [sl_hc, false, 5),
        auto [sl_shc, true, 3),
        auto [sl_hc, true, 4),
        auto [sli_shc, true, (3, 4)),
        auto [sli_hc, true, 5)],
    "independent_set": [
        auto [gis_shc, false, (2, 3)),
        auto [gis_hc, false, 3)],
    "connected_sequential": [
        auto [cs_shc, false, (3, 4)),
        auto [cs_shc, true, 3)],
    "connected_sequential_dfs": [
        auto [cs_shc, false, (3, 4))],
}
