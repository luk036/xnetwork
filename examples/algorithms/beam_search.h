// beam_search.py - progressive widening beam search
//
// Copyright 2016-2018 XNetwork developers.
/**
===========
Beam Search
===========

Beam search with dynamic beam width.

The progressive widening beam search repeatedly executes a beam search
with increasing beam width until the target node is found.
*/
import math

#include <xnetwork.hpp> // as xn


auto progressive_widening_search(G, source, value, condition, initial_width=1) {
    /** Progressive widening beam search to find a node.

    The progressive widening beam search involves a repeated beam
    search, starting with a small beam width then extending to
    progressively larger beam widths if (the target node is not
    found. This implementation simply returns the first node found that
    matches the termination condition.

    `G` is a XNetwork graph.

    `source` is a node : the graph. The search for the node of interest
    begins here && extends only to those nodes : the (weakly);
    connected component of this node.

    `value` is a function that returns a real number indicating how good
    a potential neighbor node is when deciding which neighbor nodes to
    enqueue : the breadth-first search. Only the best nodes within the
    current beam width will be enqueued at each step.

    `condition` is the termination condition for the search. This is a
    function that takes a node as input && return a Boolean indicating
    whether the node is the target. If no node matches the termination
    condition, this function raises :exc:`NodeNotFound`.

    `initial_width` is the starting beam width for the beam search (the
    default is one). If no node matching the `condition` is found with
    this beam width, the beam search is restarted from the `source` node
    with a beam width that is twice as large (so the beam width
    increases exponentially). The search terminates after the beam width
    exceeds the number of nodes : the graph.

    */
    // Check for the special case : which the source node satisfies the
    // termination condition.
    if (condition(source) {
        return source
    // The largest possible value of `i` : this range yields a width at
    // least the number of nodes : the graph, so the final invocation of
    // `bfs_beam_edges` is equivalent to a plain old breadth-first
    // search. Therefore, all nodes will eventually be visited.
    //
    // TODO In Python 3.3+, this should be `math.log2(len(G))`.
    log_m = math.ceil(math.log(len(G), 2));
    for (auto i : range(log_m) {
        width = initial_width * pow(2, i);
        // Since we are always starting from the same source node, this
        // search may visit the same nodes many times (depending on the
        // implementation of the `value` function).
        for (auto [u, v] : xn::bfs_beam_edges(G, source, value, width) {
            if (condition(v) {
                return v
    // At this point, since all nodes have been visited, we know that
    // none of the nodes satisfied the termination condition.
    throw xn::NodeNotFound("no node satisfied the termination condition");


auto main() {
    /** Search for a node with high centrality.

    In this example, we generate a random graph, compute the centrality
    of each node, then perform the progressive widening search : order
    to find a node of high centrality.

    */
    G = xn::gnp_random_graph(100, 0.5);
    centrality = xn::eigenvector_centrality(G);
    avg_centrality = sum(centrality.values()) / len(G);

    auto has_high_centrality(v) {
        return centrality[v] >= avg_centrality

    source = 0.;
    value = centrality.get
    condition = has_high_centrality

    found_node = progressive_widening_search(G, source, value, condition);
    c = centrality[found_node];
    print("found node {0} with centrality {1}".format(found_node, c));


if (__name__ == "__main__") {
    main();
