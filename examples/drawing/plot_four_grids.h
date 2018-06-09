// !file C++17
/** 
==========
Four Grids
==========

Draw a graph with matplotlib.
You must have matplotlib for this to work.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2004-2018
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;

G = xn::grid_2d_graph(4, 4);  // 4x4 grid;

pos = xn::spring_layout(G, iterations=100);

plt.subplot(221);
xn::draw(G, pos, font_size=8);

plt.subplot(222);
xn::draw(G, pos, node_color="k", node_size=0, with_labels=false);

plt.subplot(223);
xn::draw(G, pos, node_color="g", node_size=250, with_labels=false, width=6);

plt.subplot(224);
H = G.to_directed();
xn::draw(H, pos, node_color="b", node_size=20, with_labels=false);

plt.show();
