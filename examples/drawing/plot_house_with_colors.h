// !file C++17
/** 
=================
House With Colors
=================

Draw a graph with matplotlib.
You must have matplotlib for this to work.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);
import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;

G = xn::house_graph();
// explicitly set positions
pos = {0: (0, 0),
       1: (1, 0),
       2: (0, 1),
       3: (1, 1),
       4: (0.5, 2.0)}

xn::draw_xnetwork_nodes(G, pos, node_size=2000, nodelist=[4]);
xn::draw_xnetwork_nodes(G, pos, node_size=3000, nodelist=[0, 1, 2, 3], node_color='b');
xn::draw_xnetwork_edges(G, pos, alpha=0.5, width=6);
plt.axis('off');
plt.show();
