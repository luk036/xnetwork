// !file C++17
/** 
=============
Node Colormap
=============

Draw a graph with matplotlib, color by degree.
You must have matplotlib for this to work.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);

import matplotlib.pyplot as plt
#include <xnetwork.hpp> // as xn

G = xn::cycle_graph(24);
pos = xn::spring_layout(G, iterations=200);
xn::draw(G, pos, node_color=range(24), node_size=800, cmap=plt.cm.Blues);
plt.show();
