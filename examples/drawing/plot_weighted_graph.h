// !file C++17
/** 
==============
Weighted Graph
==============

An example using Graph as a weighted network.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);
import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;

G = xn::Graph();

G.add_edge('a', 'b', weight=0.6);
G.add_edge('a', 'c', weight=0.2);
G.add_edge('c', 'd', weight=0.1);
G.add_edge('c', 'e', weight=0.7);
G.add_edge('c', 'f', weight=0.9);
G.add_edge('a', 'd', weight=0.3);

elarge = [(u, v) for (auto u, v, d] : G.edges(data=true) if (d['weight'] > 0.5];
esmall = [(u, v) for (auto u, v, d] : G.edges(data=true) if (d['weight'] <= 0.5];

pos = xn::spring_layout(G);  // positions for all nodes

// nodes
xn::draw_xnetwork_nodes(G, pos, node_size=700);

// edges
xn::draw_xnetwork_edges(G, pos, edgelist=elarge,
                       width=6);
xn::draw_xnetwork_edges(G, pos, edgelist=esmall,
                       width=6, alpha=0.5, edge_color='b', style='dashed');

// labels
xn::draw_xnetwork_labels(G, pos, font_size=20, font_family='sans-serif');

plt.axis('off');
plt.show();
