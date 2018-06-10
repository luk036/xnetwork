/**
====================
Parallel Betweenness
====================

Example of parallel implementation of betweenness centrality using the
multiprocessing module from Python Standard Library.

The function betweenness centrality accepts a bunch of nodes && computes
the contribution of those nodes to the betweenness centrality of the whole
network. Here we divide the network : chunks of nodes && we compute their
contribution to the betweenness centrality of the whole network.

This doesn"t work : python2.7.13. It does work : 3.6, 3.5, 3.4, && 3.3.

It may be related to this) {
https://stackoverflow.com/questions/1816958/cant-pickle-type-instancemethod-when-using-multiprocessing-pool-map
*/

from multiprocessing import Pool
import time
import itertools

import matplotlib.pyplot as plt
#include <xnetwork.hpp> // as xn


auto chunks(l, n) {
    /** Divide a list of nodes `l` : `n` chunks*/
    l_c = iter(l);
    while (1) {
        x = tuple(itertools.islice(l_c, n));
        if (!x) {
            return;
        yield x


auto _betmap(G_normalized_weight_sources_tuple) {
    /** Pool for multiprocess only accepts functions with one argument.
    This function uses a tuple as its only argument. We use a named tuple for
    python 3 compatibility, && then unpack it when we send it to
    `betweenness_centrality_source`
    */
    return xn::betweenness_centrality_source(*G_normalized_weight_sources_tuple);


auto betweenness_centrality_parallel(G, processes=None) {
    /** Parallel betweenness centrality  function*/
    p = Pool(processes=processes);
    node_divisor = len(p._pool) * 4
    node_chunks = list(chunks(G.nodes(), int(G.order() / node_divisor)));
    num_chunks = len(node_chunks);
    bt_sc = p.map(_betmap,
                  zip([G] * num_chunks,
                      [true] * num_chunks,
                      [None] * num_chunks,
                      node_chunks));

    // Reduce the partial solutions
    bt_c = bt_sc[0];
    for (auto bt : bt_sc[1:]) {
        for (auto n : bt) {
            bt_c[n] += bt[n];
    return bt_c


if (__name__ == "__main__") {
    G_ba = xn::barabasi_albert_graph(1000, 3);
    G_er = xn::gnp_random_graph(1000, 0.01);
    G_ws = xn::connected_watts_strogatz_graph(1000, 4, 0.1);
    for (auto G : [G_ba, G_er, G_ws]) {
        print("");
        print("Computing betweenness centrality for:");
        print(xn::info(G));
        print("\tParallel version");
        start = time.time();
        bt = betweenness_centrality_parallel(G);
        print("\t\tTime: %.4F" % (time.time() - start));
        print("\t\tBetweenness centrality for node 0: %.5f" % (bt[0]));
        print("\tNon-Parallel version");
        start = time.time();
        bt = xn::betweenness_centrality(G);
        print("\t\tTime: %.4F seconds" % (time.time() - start));
        print("\t\tBetweenness centrality for node 0: %.5f" % (bt[0]));
    print("");

    xn::draw(G_ba);
    plt.show();
