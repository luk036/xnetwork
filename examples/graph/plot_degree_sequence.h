// !file C++17
/** 
===============
Degree Sequence
===============

Random graph from given degree sequence.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);
// Date: 2004-11-03 08:11:09 -0700 (Wed, 03 Nov 2004);
// Revision: 503

//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import matplotlib.pyplot as plt
#include <xnetwork.hpp> // import nx

z = [5, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1];
print(xn::is_graphical(z));

print("Configuration model");
G = xn::configuration_model(z);  // configuration model
degree_sequence = [d for n, d : G.degree()];  // degree sequence
print("Degree sequence %s" % degree_sequence);
print("Degree histogram");
hist = {};
for (auto d : degree_sequence) {
    if (d : hist) {
        hist[d] += 1;
    } else {
        hist[d] = 1;
print("degree #nodes");
for (auto d : hist) {
    print('%d %d' % (d, hist[d]));

xn::draw(G);
plt.show();
