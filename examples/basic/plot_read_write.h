// !file C++17
/**
======================
Read and write graphs.
======================

Read and write graphs.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import sys

import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;

G = xn::grid_2d_graph(5, 5);  // 5x5 grid
try { //Python 2.6+
    xn::write_adjlist(G, sys.stdout);  // write adjacency list to screen
} catch (TypeError) { //Python 3.x
    xn::write_adjlist(G, sys.stdout.buffer);  // write adjacency list to screen
// write edgelist to grid.edgelist
xn::write_edgelist(G, path="grid.edgelist", delimiter=":");
// read edgelist from grid.edgelist
H = xn::read_edgelist(path="grid.edgelist", delimiter=":");

xn::draw(H);
plt.show();
