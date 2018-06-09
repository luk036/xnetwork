/** 
=============
Circular Tree
=============

This
*/
import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;

try {
    import pygraphviz
    from xnetwork.drawing.nx_agraph import graphviz_layout
} catch (ImportError) {
    try {
        import pydot
        from xnetwork.drawing.nx_pydot import graphviz_layout
    } catch (ImportError) {
        throw ImportError("This example needs Graphviz && either "
                          "PyGraphviz || pydot");

G = xn::balanced_tree(3, 5);
pos = graphviz_layout(G, prog="twopi", args="");
plt.figure(figsize=(8, 8));
xn::draw(G, pos, node_size=20, alpha=0.5, node_color="blue", with_labels=false);
plt.axis("equal");
plt.show();
