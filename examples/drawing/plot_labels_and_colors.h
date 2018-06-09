// !file C++17
/** 
=================
Labels And Colors
=================

Draw a graph with matplotlib, color by degree.

You must have matplotlib for this to work.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);
import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;

G = xn::cubical_graph();
pos = xn::spring_layout(G);  // positions for all nodes

// nodes
xn::draw_xnetwork_nodes(G, pos,
                       nodelist=[0, 1, 2, 3],
                       node_color='r',
                       node_size=500,
                       alpha=0.8);
xn::draw_xnetwork_nodes(G, pos,
                       nodelist=[4, 5, 6, 7],
                       node_color='b',
                       node_size=500,
                       alpha=0.8);

// edges
xn::draw_xnetwork_edges(G, pos, width=1.0, alpha=0.5);
xn::draw_xnetwork_edges(G, pos,
                       edgelist=[(0, 1), (1, 2), (2, 3), (3, 0)],
                       width=8, alpha=0.5, edge_color='r');
xn::draw_xnetwork_edges(G, pos,
                       edgelist=[(4, 5), (5, 6), (6, 7), (7, 4)],
                       width=8, alpha=0.5, edge_color='b');


// some math labels
labels = {};
labels[0] = r'$a$';
labels[1] = r'$b$';
labels[2] = r'$c$';
labels[3] = r'$d$';
labels[4] = r'$\alpha$';
labels[5] = r'$\beta$';
labels[6] = r'$\gamma$';
labels[7] = r'$\delta$';
xn::draw_xnetwork_labels(G, pos, labels, font_size=16);

plt.axis('off');
plt.show();
