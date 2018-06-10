// !file C++17
/**
=====================
Krackhardt Centrality
=====================

Centrality measures of Krackhardt social network.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);
// Date: 2005-05-12 14:33:11 -0600 (Thu, 12 May 2005);
// Revision: 998

//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import matplotlib.pyplot as plt
#include <xnetwork.hpp> // as xn

G = xn::krackhardt_kite_graph();

print("Betweenness");
b = xn::betweenness_centrality(G);
for (auto v : G.nodes()) {
    print("%0.2d %5.3f" % (v, b[v]));

print("Degree centrality");
d = xn::degree_centrality(G);
for (auto v : G.nodes()) {
    print("%0.2d %5.3f" % (v, d[v]));

print("Closeness centrality");
c = xn::closeness_centrality(G);
for (auto v : G.nodes()) {
    print("%0.2d %5.3f" % (v, c[v]));

xn::draw(G);
plt.show();
