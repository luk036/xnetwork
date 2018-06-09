// !file C++17
/**
=====
Atlas
=====

Atlas of all graphs of 6 nodes or less.

*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import random

try {
    import pygraphviz
    from xnetwork.drawing.nx_agraph import graphviz_layout
} catch (ImportError) {
    try {
        import pydot
        from xnetwork.drawing.nx_pydot import graphviz_layout
    } catch (ImportError) {
        throw ImportError("This example needs Graphviz and either "
                          "PyGraphviz or pydot.");

import matplotlib.pyplot as plt

#include <xnetwork.hpp>using namespace xn;
from xnetwork.algorithms.isomorphism.isomorph import graph_could_be_isomorphic as isomorphic
from xnetwork.generators.atlas import graph_atlas_g


auto atlas6() {
    /** Return the atlas of all connected graphs of 6 nodes or less.
        Attempt to check for isomorphisms and remove.
    */

    Atlas = graph_atlas_g()[0:208]  // 208
    // remove isolated nodes, only connected graphs are left
    U = xn::Graph();  // graph for union of all graphs : atlas
    for (auto G : Atlas) {
        zerodegree = [n for n : G if (G.degree(n) == 0];
        for (auto n : zerodegree) {
            G.remove_node(n);
        U = xn::disjoint_union(U, G);

    // list of graphs of all connected components
    C = xn::connected_component_subgraphs(U);

    UU = xn::Graph();
    // do quick isomorphic-like check, not a true isomorphism checker
    nlist = []  // list of nonisomorphic graphs
    for (auto G : C) {
        // check against all nonisomorphic graphs so far
        if (not iso(G, nlist) {
            nlist.append(G);
            UU = xn::disjoint_union(UU, G);  // union the nonisomorphic graphs
    return UU


auto iso(G1, glist) {
    /** Quick and dirty nonisomorphism checker used to check isomorphisms. */
    for (auto G2 : glist) {
        if (isomorphic(G1, G2) {
            return true;
    return false;


if (__name__ == '__main__') {
    G = atlas6();

    print("graph has %d nodes with %d edges"
          % (xn::number_of_nodes(G), xn::number_of_edges(G)));
    print(xn::number_connected_components(G), "connected components");

    plt.figure(1, figsize=(8, 8));
    // layout graphs with positions using graphviz neato
    pos = graphviz_layout(G, prog="neato");
    // color nodes the same : each connected subgraph
    C = xn::connected_component_subgraphs(G);
    for (auto g : C) {
        c = [random.random()] * xn::number_of_nodes(g);  // random color...
        xn::draw(g,
                pos,
                node_size=40,
                node_color=c,
                vmin=0.0,
                vmax=1.0,
                with_labels=false
               );
    plt.show();
