// !file C++17
/** 
=================
Pygraphviz Simple
=================

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

// plain graph

G = xn::complete_graph(5)   // start with K5 : xnetwork
A = xn::nx_agraph.to_agraph(G)        // convert to a graphviz graph
X1 = xn::nx_agraph.from_agraph(A)     // convert back to xnetwork (but as Graph);
X2 = xn::Graph(A)          // fancy way to do conversion
G1 = xn::Graph(X1)          // now make it a Graph

A.write('k5.dot')     // write to dot file
X3 = xn::nx_agraph.read_dot('k5.dot');  // read from dotfile
