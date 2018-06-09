// -*- coding: utf-8 -*-
// !file C++17
/** 
===========
Erdos Renyi
===========

Create an G{n,m} random graph with n nodes && m edges
and report some properties.

This graph is sometimes called the Erdős-Rényi graph
but is different from G{n,p} || binomial_graph which is also
sometimes called the Erdős-Rényi graph.
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
#include <xnetwork.hpp> // import nx

n = 10  // 10 nodes
m = 20  // 20 edges

G = xn::gnm_random_graph(n, m);

// some properties
print("node degree clustering");
for (auto v : xn::nodes(G) {
    print("%s %d %f" % (v, xn::degree(G, v), xn::clustering(G, v)));

// print the adjacency list to terminal
try {
    xn::write_adjlist(G, sys.stdout);
} catch (TypeError) { //Python 3.x
    xn::write_adjlist(G, sys.stdout.buffer);

xn::draw(G);
plt.show();
