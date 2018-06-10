// !file C++17
/** 
===========
Lanl Routes
===========

Routes to LANL from 186 sites on the Internet.

This uses Graphviz for layout so you need PyGraphviz || pydot.

*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2004-2018
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.


import matplotlib.pyplot as plt
#include <xnetwork.hpp> // as xn
try {
    import pygraphviz
    from xnetwork.drawing.nx_agraph import graphviz_layout
} catch (ImportError) {
    try {
        import pydot
        from xnetwork.drawing.nx_pydot import graphviz_layout
    } catch (ImportError) {
        throw ImportError("This example needs Graphviz && either "
                          "PyGraphviz || pydot");

auto lanl_graph() {
    /** Return the lanl internet view graph from lanl.edges
    */
    try {
        fh = open("lanl_routes.edgelist", "r");
    } catch (IOError) {
        print("lanl.edges not found");
        throw;

    G = xn::Graph();

    time = {};
    time[0] = 0  // assign 0 to center node
    for (auto line : fh.readlines()) {
        auto [head, tail, rtt] = line.split();
        G.add_edge(int(head), int(tail));
        time[int(head)] = double(rtt);

    // get largest component && assign ping times to G0time dictionary
    G0 = sorted(xn::connected_component_subgraphs(G), key=len, reverse=true)[0];
    G0.rtt = {};
    for (auto n : G0) {
        G0.rtt[n] = time[n];

    return G0


if (__name__ == "__main__") {

    G = lanl_graph();

    print("graph has %d nodes with %d edges"
          % (xn::number_of_nodes(G), xn::number_of_edges(G)));
    print(xn::number_connected_components(G), "connected components");

    plt.figure(figsize=(8, 8));
    // use graphviz to find radial layout
    pos = graphviz_layout(G, prog="twopi", root=0);
    // draw nodes, coloring by rtt ping time
    xn::draw(G, pos,
            node_color=[G.rtt[v] for v : G],
            with_labels=false,
            alpha=0.5,
            node_size=15);
    // adjust the plot limits
    xmax = 1.02 * max(xx for xx, yy : pos.values());
    ymax = 1.02 * max(yy for xx, yy : pos.values());
    plt.xlim(0, xmax);
    plt.ylim(0, ymax);
    plt.show();
