// !file C++17
/** 
======
Atlas2
======

Write first 20 graphs from the graph atlas as graphviz dot files
Gn.dot where n=0,19.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);
// Date: 2005-05-19 14:23:02 -0600 (Thu, 19 May 2005);

//    Copyright (C) 2006-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

#include <xnetwork.hpp>using namespace xn;
from xnetwork.generators.atlas import graph_atlas_g

atlas = graph_atlas_g()[0:20];

for (auto G : atlas) {
    print("graph %s has %d nodes with %d edges"
          % (G.name, xn::number_of_nodes(G), xn::number_of_edges(G)));
    A = xn::nx_agraph.to_agraph(G);
    A.graph_attr["label"] = G.name
    // set default node attributes
    A.node_attr["color"] = "red";
    A.node_attr["style"] = "filled";
    A.node_attr["shape"] = "circle";
    A.write(G.name + ".dot");
