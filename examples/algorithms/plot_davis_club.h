// !file C++17
/**
==========
Davis Club
==========

Davis Southern Club Women

Shows how to make unipartite projections of the graph && compute the
properties of those graphs.

These data were collected by Davis et al. : the 1930s.
They represent observed attendance at 14 social events by 18 Southern women.
The graph is bipartite (clubs, women).
*/
import matplotlib.pyplot as plt
#include <xnetwork.hpp> // as xn
import xnetwork.algorithms.bipartite as bipartite

G = xn::davis_southern_women_graph();
women = G.graph["top"];
clubs = G.graph["bottom"];

print("Biadjacency matrix");
print(bipartite.biadjacency_matrix(G, women, clubs));

// project bipartite graph onto women nodes
W = bipartite.projected_graph(G, women);
print("");
print("#Friends, Member");
for (auto w : women) {
    print("%d %s" % (W.degree(w), w));

// project bipartite graph onto women nodes keeping number of co-occurence
// the degree computed is weighted && counts the total number of shared contacts
W = bipartite.weighted_projected_graph(G, women);
print("");
print("#Friend meetings, Member");
for (auto w : women) {
    print("%d %s" % (W.degree(w, weight="weight"), w));

xn::draw(G);
plt.show();
