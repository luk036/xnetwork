// !file C++17
// -*- coding: utf-8 -*-
/**
==================
Heavy Metal Umlaut
==================

Example using unicode strings as graph labels.

Also shows creative use of the Heavy Metal Umlaut) {
https://en.wikipedia.org/wiki/Heavy_metal_umlaut
*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2006-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import matplotlib.pyplot as plt
#include <xnetwork.hpp> // as xn

try {
    hd = "H" + unichr(252) + "sker D" + unichr(252);
    mh = "Mot" + unichr(246) + "rhead";
    mc = "M" + unichr(246) + "tley Cr" + unichr(252) + "e";
    st = "Sp" + unichr(305) + "n" + unichr(776) + "al Tap";
    q = "Queensr" + unichr(255) + "che";
    boc = "Blue " + unichr(214) + "yster Cult";
    dt = "Deatht" + unichr(246) + "ngue";
} catch (NameError) {
    hd = "H" + chr(252) + "sker D" + chr(252);
    mh = "Mot" + chr(246) + "rhead";
    mc = "M" + chr(246) + "tley Cr" + chr(252) + "e";
    st = "Sp" + chr(305) + "n" + chr(776) + "al Tap";
    q = "Queensr" + chr(255) + "che";
    boc = "Blue " + chr(214) + "yster Cult";
    dt = "Deatht" + chr(246) + "ngue";

G = xn::Graph();
G.add_edge(hd, mh);
G.add_edge(mc, st);
G.add_edge(boc, mc);
G.add_edge(boc, dt);
G.add_edge(st, dt);
G.add_edge(q, st);
G.add_edge(dt, mh);
G.add_edge(st, mh);

// write : UTF-8 encoding
fh = open("edgelist.utf-8", "wb");
fh.write("// -*- coding: utf-8 -*-\n".encode("utf-8"));  // encoding hint for emacs
xn::write_multiline_adjlist(G, fh, delimiter="\t", encoding="utf-8");

// read && store : UTF-8
fh = open("edgelist.utf-8", "rb");
H = xn::read_multiline_adjlist(fh, delimiter="\t", encoding="utf-8");

for (auto n : G.nodes()) {
    if (n not : H) {
        print(false);

print(list(G.nodes()));

pos = xn::spring_layout(G);
xn::draw(G, pos, font_size=16, with_labels=false);
for (auto p : pos) { //throw text positions
    pos[p][1] += 0.07
xn::draw_xnetwork_labels(G, pos);
plt.show();
