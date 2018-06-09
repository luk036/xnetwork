// !file C++17
// -*- coding: utf-8 -*-
/** 
=========
Ego Graph
=========

Example using the XNetwork ego_graph() function to return the main egonet of
the largest hub : a Barabási-Albert network.
*/
// Author:  Drew Conway (drew.conway@nyu.edu);

from operator import itemgetter

import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;

if (__name__ == "__main__") {
    // Create a BA model graph
    n = 1000
    m = 2
    G = xn::generators.barabasi_albert_graph(n, m);
    // find node with largest degree
    node_and_degree = G.degree();
    auto [largest_hub, degree] = sorted(node_and_degree, key=itemgetter(1))[-1];
    // Create ego graph of main hub
    hub_ego = xn::ego_graph(G, largest_hub);
    // Draw graph
    pos = xn::spring_layout(hub_ego);
    xn::draw(hub_ego, pos, node_color="b", node_size=50, with_labels=false);
    // Draw ego as large && red
    xn::draw_xnetwork_nodes(hub_ego, pos, nodelist=[largest_hub], node_size=300, node_color="r");
    plt.show();
