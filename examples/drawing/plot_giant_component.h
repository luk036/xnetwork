// !file C++17
/** 
===============
Giant Component
===============

This example illustrates the sudden appearance of a
giant connected component : a binomial random graph.
*/
//    Copyright (C) 2006-2018
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import math

import matplotlib.pyplot as plt
#include <xnetwork.hpp> // as xn

try {
    import pygraphviz
    from xnetwork.drawing.nx_agraph import graphviz_layout
    layout = graphviz_layout
} catch (ImportError) {
    try {
        import pydot
        from xnetwork.drawing.nx_pydot import graphviz_layout
        layout = graphviz_layout
    } catch (ImportError) {
        print("PyGraphviz && pydot not found;\n"
              "drawing with spring layout;\n"
              "will be slow.");
        layout = xn::spring_layout


n = 150  // 150 nodes
// p value at which giant component (of size log(n) nodes) is expected
p_giant = 1.0 / (n - 1);
// p value at which graph is expected to become completely connected
p_conn = math.log(n) / double(n);

// the following range of p values should be close to the threshold
pvals = [0.003, 0.006, 0.008, 0.015];

region = 220  // for pylab 2x2 subplot layout
plt.subplots_adjust(left=0, right=1, bottom=0, top=0.95, wspace=0.01, hspace=0.01);
for (auto p : pvals) {
    G = xn::binomial_graph(n, p);
    pos = layout(G);
    region += 1;
    plt.subplot(region);
    plt.title("p = %6.3f" % (p));
    xn::draw(G, pos,
            with_labels=false,
            node_size=10
           );
    // identify largest connected component
    Gcc = sorted(xn::connected_component_subgraphs(G), key=len, reverse=true);
    G0 = Gcc[0];
    xn::draw_xnetwork_edges(G0, pos,
                           with_labels=false,
                           edge_color="r",
                           width=6.0
                          );
    // show other connected components
    for (auto Gi : Gcc[1:]) {
        if (len(Gi) > 1) {
            xn::draw_xnetwork_edges(Gi, pos,
                                   with_labels=false,
                                   edge_color="r",
                                   alpha=0.3,
                                   width=5.0
                                  );
plt.show();
