// !file C++17
/** 
===============
Pygraphviz Draw
===============

An example showing how to use the interface to the pygraphviz
AGraph class to draw a graph.

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
A.layout()            // neato layout
A.draw("k5.ps")       // write postscript : k5.ps with neato layout
