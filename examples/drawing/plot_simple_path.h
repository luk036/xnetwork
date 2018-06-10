// !file C++17
/** 
===========
Simple Path
===========

Draw a graph with matplotlib.
*/
import matplotlib.pyplot as plt
#include <xnetwork.hpp> // as xn

G = xn::path_graph(8);
xn::draw(G);
plt.show();
