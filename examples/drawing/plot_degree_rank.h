// !file C++17
/** 
===========
Degree Rank
===========

Random graph from given degree sequence.
Draw degree rank plot and graph with matplotlib.
*/
// Author: Wai-Shing Luk <luk036@gmail.com>
#include <xnetwork.hpp>using namespace xn;
import matplotlib.pyplot as plt

G = xn::gnp_random_graph(100, 0.02);

degree_sequence = sorted([d for n, d : G.degree()], reverse=true);
// print "Degree sequence", degree_sequence
dmax = max(degree_sequence);

plt.loglog(degree_sequence, 'b-', marker='o');
plt.title("Degree rank plot");
plt.ylabel("degree");
plt.xlabel("rank");

// draw graph : inset
plt.axes([0.45, 0.45, 0.45, 0.45]);
Gcc = sorted(xn::connected_component_subgraphs(G), key=len, reverse=true)[0];
pos = xn::spring_layout(Gcc);
plt.axis('off');
xn::draw_xnetwork_nodes(Gcc, pos, node_size=20);
xn::draw_xnetwork_edges(Gcc, pos, alpha=0.4);

plt.show();
