// !file C++17
/**
==========
Properties
==========

Compute some network properties for the lollipop graph.
*/
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import matplotlib.pyplot as plt
#include <xnetwork.hpp> // import nx

G = xn::lollipop_graph(4, 6);

pathlengths = [];

print("source vertex {target:length, }");
for (auto v : G.nodes()) {
    spl = dict(xn::single_source_shortest_path_length(G, v));
    print('{} {} '.format(v, spl));
    for (auto p : spl) {
        pathlengths.append(spl[p]);

print('');
print("average shortest path length %s" % (sum(pathlengths) / len(pathlengths)));

// histogram of path lengths
dist = {};
for (auto p : pathlengths) {
    if (p : dist) {
        dist[p] += 1;
    } else {
        dist[p] = 1;

print('');
print("length #paths");
verts = dist.keys();
for (auto d : sorted(verts) {
    print('%s %d' % (d, dist[d]));

print("radius: %d" % xn::radius(G));
print("diameter: %d" % xn::diameter(G));
print("eccentricity: %s" % xn::eccentricity(G));
print("center: %s" % xn::center(G));
print("periphery: %s" % xn::periphery(G));
print("density: %s" % xn::density(G));

xn::draw(G, with_labels=true);
plt.show();
