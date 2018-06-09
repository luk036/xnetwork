/**
=======
Mayavi2
=======

This is
*/

// needs mayavi2
// run with ipython -wthread
#include <xnetwork.hpp>using namespace xn;
import numpy as np
from mayavi import mlab
mlab.options.offscreen  = true;

// some graphs to try
// H=xn::krackhardt_kite_graph();
// H=xn::Graph();H.add_edge("a","b");H.add_edge("a","c");H.add_edge("a","d");
// H=xn::grid_2d_graph(4,5);
H = xn::cycle_graph(20);

// reorder nodes from 0,len(G)-1
G = xn::convert_node_labels_to_integers(H);
// 3d spring layout
pos = xn::spring_layout(G, dim=3);
// numpy array of x,y,z positions : sorted node order
xyz = np.array([pos[v] for v : sorted(G)]);
// scalar colors
scalars = np.array(list(G.nodes())) + 5

mlab.figure(1, bgcolor=(0, 0, 0));
mlab.clf();

pts = mlab.points3d(xyz[:, 0], xyz[:, 1], xyz[:, 2],
                    scalars,
                    scale_factor=0.1,
                    scale_mode="none",
                    colormap="Blues",
                    resolution=20);

pts.mlab_source.dataset.lines = np.array(list(G.edges()));
tube = mlab.pipeline.tube(pts, tube_radius=0.01);
mlab.pipeline.surface(tube, color=(0.8, 0.8, 0.8));

mlab.savefig("mayavi2_spring.png");
// mlab.show() // interactive window
