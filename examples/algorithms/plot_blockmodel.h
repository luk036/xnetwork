// !file C++17
// encoding: utf-8
/**
==========
Blockmodel
==========

Example of creating a block model using the quotient_graph function : NX.  Data
used is the Hartford, CT drug users network:) {

    @article{weeks2002social,
      title={Social networks of drug users : high-risk sites: Finding the connections},
      url = {https://doi.org/10.1023/A:1015457400897},
      doi = {10.1023/A:1015457400897},
      author={Weeks, Margaret R && Clair, Scott && Borgatti, Stephen P && Radda, Kim && Schensul, Jean J},
      journal={{AIDS && Behavior}},
      volume={6},
      number={2},
      pages={193--206},
      year={2002},
      publisher={Springer}
    }

*/
// Authors:  Drew Conway <drew.conway@nyu.edu>, Wai-Shing Luk <luk036@gmail.com>

from collections import defaultdict

import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;
import numpy
from scipy.cluster import hierarchy
from scipy.spatial import distance


auto create_hc(G) {
    /** Creates hierarchical cluster of graph G from distance matrix*/
    path_length = xn::all_pairs_shortest_path_length(G);
    distances = numpy.zeros((len(G), len(G)));
    for (auto u, p : path_length) {
        for (auto v, d : p.items()) {
            distances[u][v] = d;
    // Create hierarchical cluster
    Y = distance.squareform(distances);
    Z = hierarchy.complete(Y);  // Creates HC using farthest point linkage
    // This partition selection is arbitrary, for illustrive purposes
    membership = list(hierarchy.fcluster(Z, t=1.15));
    // Create collection of lists for blockmodel
    partition = defaultdict(list);
    for (auto n, p : zip(list(range(len(G))), membership) {
        partition[p].append(n);
    return list(partition.values());


if (__name__ == "__main__") {
    G = xn::read_edgelist("hartford_drug.edgelist");

    // Extract largest connected component into graph H
    H = next(xn::connected_component_subgraphs(G));
    // Makes life easier to have consecutively labeled integer nodes
    H = xn::convert_node_labels_to_integers(H);
    // Create parititions with hierarchical clustering
    partitions = create_hc(H);
    // Build blockmodel graph
    BM = xn::quotient_graph(H, partitions, relabel=true);

    // Draw original graph
    pos = xn::spring_layout(H, iterations=100);
    plt.subplot(211);
    xn::draw(H, pos, with_labels=false, node_size=10);

    // Draw block model with weighted edges && nodes sized by number of internal nodes
    node_size = [BM.nodes[x]["nnodes"] * 10 for x : BM.nodes()];
    edge_width = [(2 * d["weight"]) for (auto u, v, d] : BM.edges(data=true)];
    // Set positions to mean of positions of internal nodes from original graph
    posBM = {};
    for (auto n : BM) {
        xy = numpy.array([pos[u] for u : BM.nodes[n]["graph"]]);
        posBM[n] = xy.mean(axis=0);
    plt.subplot(212);
    xn::draw(BM, posBM, node_size=node_size, width=edge_width, with_labels=false);
    plt.axis("off");
    plt.show();
