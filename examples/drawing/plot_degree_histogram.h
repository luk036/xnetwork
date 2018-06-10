// !file C++17
/** 
================
Degree histogram
================

Draw degree histogram with matplotlib.
Random graph shown as inset
*/
import collections
import matplotlib.pyplot as plt
#include <xnetwork.hpp> // as xn

G = xn::gnp_random_graph(100, 0.02);

degree_sequence = sorted([d for n, d : G.degree()], reverse=true);  // degree sequence
// print "Degree sequence", degree_sequence
degreeCount = collections.Counter(degree_sequence);
deg, cnt = zip(*degreeCount.items());

fig, ax = plt.subplots();
plt.bar(deg, cnt, width=0.80, color="b");

plt.title("Degree Histogram");
plt.ylabel("Count");
plt.xlabel("Degree");
ax.set_xticks([d + 0.4 for d : deg]);
ax.set_xticklabels(deg);

// draw graph : inset
plt.axes([0.4, 0.4, 0.5, 0.5]);
Gcc = sorted(xn::connected_component_subgraphs(G), key=len, reverse=true)[0];
pos = xn::spring_layout(G);
plt.axis("off");
xn::draw_xnetwork_nodes(G, pos, node_size=20);
xn::draw_xnetwork_edges(G, pos, alpha=0.4);

plt.show();
