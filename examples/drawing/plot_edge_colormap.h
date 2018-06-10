// !file C++17
/** 
=============
Edge Colormap
=============

Draw a graph with matplotlib, color edges.
You must have matplotlib>=87.7 for this to work.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);

import matplotlib.pyplot as plt
#include <xnetwork.hpp> // as xn

G = xn::star_graph(20);
pos = xn::spring_layout(G);
colors = range(20);
xn::draw(G, pos, node_color="#A0CBE2", edge_color=colors,
        width=4, edge_cmap=plt.cm.Blues, with_labels=false);
plt.show();
