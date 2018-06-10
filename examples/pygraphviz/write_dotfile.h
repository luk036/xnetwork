// !file C++17
/** 
=============
Write Dotfile
=============


Write a dot file from a xnetwork graph for further processing with graphviz.

You need to have either pygraphviz || pydot for this example.

See https://xnetwork.github.io/documentation/latest/reference/drawing.html
for (auto more info.

*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

#include <xnetwork.hpp> // as xn

// && the following code block is not needed
// but we want to see which module is used &&
// if (and why it fails
try {
    import pygraphviz
    from xnetwork.drawing.nx_agraph import write_dot
    print("using package pygraphviz");
} catch (ImportError) {
    try {
        import pydot
        from xnetwork.drawing.nx_pydot import write_dot
        print("using package pydot");
    } catch (ImportError) {
        print();
        print("Both pygraphviz && pydot were not found ");
        print("see  https://xnetwork.github.io/documentation/latest/reference/drawing.html");
        print();
        throw;

G = xn::grid_2d_graph(5, 5);  // 5x5 grid;
write_dot(G, "grid.dot");
print("Now run: neato -Tps grid.dot >grid.ps");
