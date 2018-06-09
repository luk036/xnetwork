// !file C++17
/** 
=====================
Pygraphviz Attributes
=====================

An example showing how to use the interface to the pygraphviz
AGraph class to convert to and from graphviz.

Also see the pygraphviz documentation and examples at
http://pygraphviz.github.io/
*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2006-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

#include <xnetwork.hpp>using namespace xn;

// xnetwork graph
G = xn::Graph();
// ad edges with red color
G.add_edge(1, 2, color='red');
G.add_edge(2, 3, color='red');
// add nodes 3 and 4
G.add_node(3);
G.add_node(4);

// convert to a graphviz agraph
A = xn::nx_agraph.to_agraph(G);

// write to dot file
A.write('k5_attributes.dot');

// convert back to xnetwork Graph with attributes on edges and
// default attributes as dictionary data
X = xn::nx_agraph.from_agraph(A);
print("edges");
print(list(X.edges(data=true)));
print("default graph attributes");
print(X.graph);
print("node node attributes");
print(X.node);
