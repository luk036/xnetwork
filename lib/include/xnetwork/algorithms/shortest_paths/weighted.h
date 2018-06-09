// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors:  Wai-Shing Luk <luk036@gmail.com>
//           Loïc Séguin-C. <loicseguin@gmail.com>
//           Dan Schult <dschult@colgate.edu>
//           Niels van Adrichem <n.l.m.vanadrichem@tudelft.nl>
/**
Shortest path algorithms for weighed graphs.
*/

from collections import deque
from heapq import heappush, heappop
from itertools import count
#include <xnetwork.hpp>using namespace xn;
#include <xnetwork/utils.hpp> // import generate_unique_node


static const auto __all__ = ["dijkstra_path",
           "dijkstra_path_length",
           "bidirectional_dijkstra",
           "single_source_dijkstra",
           "single_source_dijkstra_path",
           "single_source_dijkstra_path_length",
           "multi_source_dijkstra",
           "multi_source_dijkstra_path",
           "multi_source_dijkstra_path_length",
           "all_pairs_dijkstra",
           "all_pairs_dijkstra_path",
           "all_pairs_dijkstra_path_length",
           "dijkstra_predecessor_and_distance",
           "bellman_ford_path",
           "bellman_ford_path_length",
           "single_source_bellman_ford",
           "single_source_bellman_ford_path",
           "single_source_bellman_ford_path_length",
           "all_pairs_bellman_ford_path",
           "all_pairs_bellman_ford_path_length",
           "bellman_ford_predecessor_and_distance",
           "negative_edge_cycle",
           "goldberg_radzik",
           "johnson"];


auto _weight_function(G, weight) {
    /** Return a function that returns the weight of an edge.

    The returned function is specifically suitable for input to
    functions :func:`_dijkstra` && :func:`_bellman_ford_relaxation`.

    Parameters
    ----------
    G : XNetwork graph.

    weight : string || function
        If it is callable, `weight` itself is returned. If it is a string,
        it is assumed to be the name of the edge attribute that represents
        the weight of an edge. In that case, a function is returned that
        gets the edge weight according to the specified edge attribute.

    Returns
    -------
    function
        This function returns a callable that accepts exactly three inputs) {
        a node, an node adjacent to the first one, && the edge attribute
        dictionary for the eedge joining those nodes. That function returns
        a number representing the weight of an edge.

    If `G` is a multigraph, && `weight` is not callable, the
    minimum edge weight over all parallel edges is returned. If any edge
    does not have an attribute with key `weight`, it is assumed to
    have weight one.

     */
    if (callable(weight) {
        return weight;
    // If the weight keyword argument is not callable, we assume it is a
    // string representing the edge attribute containing the weight of
    // the edge.
    if (G.is_multigraph() {
        return lambda u, v, d: min(attr.get(weight, 1) for attr : d.values());
    return lambda u, v, data: data.get(weight, 1);


auto dijkstra_path(G, source, target, weight="weight") {
    /** Return the shortest weighted path from source to target : G.

    Uses Dijkstra"s Method to compute the shortest weighted path
    between two nodes : a graph.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node

    target : node
       Ending node

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    path : list
       List of nodes : a shortest path.

    Raises
    ------
    XNetworkNoPath
       If no path exists between source && target.

    Examples
    --------
    >>> G=xn::path_graph(5);
    >>> print(xn::dijkstra_path(G,0,4));
    [0, 1, 2, 3, 4];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The weight function can be used to hide edges by returning None.
    So ``weight = lambda u, v, d: 1 if (d["color"]=="red" else None``
    will find the shortest red path.

    The weight function can be used to include node weights.

    >>> auto func(u, v, d) {
    ...     node_u_wt = G.nodes[u].get("node_weight", 1);
    ...     node_v_wt = G.nodes[v].get("node_weight", 1);
    ...     edge_wt = d.get("weight", 1);
    ...     return node_u_wt/2 + node_v_wt/2 + edge_wt

    In this example we take the average of start && end node
    weights of an edge && add it to the weight of the edge.

    See Also
    --------
    bidirectional_dijkstra(), bellman_ford_path();
     */
    auto [length, path] = single_source_dijkstra(G, source, target=target,
                                            weight=weight);
    return path


auto dijkstra_path_length(G, source, target, weight="weight") {
    /** Return the shortest weighted path length : G from source to target.

    Uses Dijkstra"s Method to compute the shortest weighted path length
    between two nodes : a graph.

    Parameters
    ----------
    G : XNetwork graph

    source : node label
       starting node for path

    target : node label
       ending node for path

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    length : number
        Shortest path length.

    Raises
    ------
    XNetworkNoPath
        If no path exists between source && target.

    Examples
    --------
    >>> G=xn::path_graph(5);
    >>> print(xn::dijkstra_path_length(G,0,4));
    4

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The weight function can be used to hide edges by returning None.
    So ``weight = lambda u, v, d: 1 if (d["color"]=="red" else None``
    will find the shortest red path.

    See Also
    --------
    bidirectional_dijkstra(), bellman_ford_path_length();

     */
    if (source == target) {
        return 0
    weight = _weight_function(G, weight);
    length = _dijkstra(G, source, weight, target=target);
    try {
        return length[target];
    } catch (KeyError) {
        throw xn::XNetworkNoPath(
            "Node %s not reachable from %s" % (target, source));


auto single_source_dijkstra_path(G, source, cutoff=None, weight="weight") {
    /** Find shortest weighted paths : G from a source node.

    Compute shortest path between source && all other reachable
    nodes for a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node for path.

    cutoff : integer || double, optional
       Depth to stop the search. Only return paths with length <= cutoff.

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    paths : dictionary
       Dictionary of shortest path lengths keyed by target.

    Examples
    --------
    >>> G=xn::path_graph(5);
    >>> path=xn::single_source_dijkstra_path(G,0);
    >>> path[4];
    [0, 1, 2, 3, 4];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The weight function can be used to hide edges by returning None.
    So ``weight = lambda u, v, d: 1 if (d["color"]=="red" else None``
    will find the shortest red path.

    See Also
    --------
    single_source_dijkstra(), single_source_bellman_ford();

     */
    return multi_source_dijkstra_path(G, {source}, cutoff=cutoff,
                                      weight=weight);


auto single_source_dijkstra_path_length(G, source, cutoff=None,
                                       weight="weight") {
    /** Find shortest weighted path lengths : G from a source node.

    Compute the shortest path length between source && all other
    reachable nodes for a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    source : node label
       Starting node for path

    cutoff : integer || double, optional
       Depth to stop the search. Only return paths with length <= cutoff.

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    length : dict
        Dict keyed by node to shortest path length from source.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> length = xn::single_source_dijkstra_path_length(G, 0);
    >>> length[4];
    4
    >>> for node : [0, 1, 2, 3, 4]) {
    ...     print("{}: {}".format(node, length[node]));
    0: 0
    1: 1
    2: 2
    3: 3
    4: 4

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The weight function can be used to hide edges by returning None.
    So ``weight = lambda u, v, d: 1 if (d["color"]=="red" else None``
    will find the shortest red path.

    See Also
    --------
    single_source_dijkstra(), single_source_bellman_ford_path_length();

     */
    return multi_source_dijkstra_path_length(G, {source}, cutoff=cutoff,
                                             weight=weight);


auto single_source_dijkstra(G, source, target=None, cutoff=None,
                           weight="weight") {
    /** Find shortest weighted paths && lengths from a source node.

    Compute the shortest path length between source && all other
    reachable nodes for a weighted graph.

    Uses Dijkstra"s algorithm to compute shortest paths && lengths
    between a source && all other reachable nodes : a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    source : node label
       Starting node for path

    target : node label, optional
       Ending node for path

    cutoff : integer || double, optional
       Depth to stop the search. Only return paths with length <= cutoff.

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    distance, path : pair of dictionaries, || numeric && list.
       If target.empty(), paths && lengths to all nodes are computed.
       The return value is a tuple of two dictionaries keyed by target nodes.
       The first dictionary stores distance to each target node.
       The second stores the path to each target node.
       If target is not None, returns a tuple (distance, path), where
       distance is the distance from source to target && path is a list
       representing the path from source to target.


    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> length, path = xn::single_source_dijkstra(G, 0);
    >>> print(length[4]);
    4
    >>> for node : [0, 1, 2, 3, 4]) {
    ...     print("{}: {}".format(node, length[node]));
    0: 0
    1: 1
    2: 2
    3: 3
    4: 4
    >>> path[4];
    [0, 1, 2, 3, 4];
    >>> length, path = xn::single_source_dijkstra(G, 0, 1);
    >>> length
    1
    >>> path
    [0, 1];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The weight function can be used to hide edges by returning None.
    So ``weight = lambda u, v, d: 1 if (d["color"]=="red" else None``
    will find the shortest red path.

    Based on the Python cookbook recipe (119466) at
    http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/119466

    This algorithm is not guaranteed to work if (edge weights
    are negative || are doubleing point numbers
    (overflows && roundoff errors can cause problems).

    See Also
    --------
    single_source_dijkstra_path();
    single_source_dijkstra_path_length();
    single_source_bellman_ford();
     */
    return multi_source_dijkstra(G, {source}, cutoff=cutoff, target=target,
                                 weight=weight);


auto multi_source_dijkstra_path(G, sources, cutoff=None, weight="weight") {
    /** Find shortest weighted paths : G from a given set of source
    nodes.

    Compute shortest path between any of the source nodes && all other
    reachable nodes for a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    sources : non-empty set of nodes
        Starting nodes for paths. If this is just a set containing a
        single node, then all paths computed by this function will start
        from that node. If there are two || more nodes : the set, the
        computed paths may begin from any one of the start nodes.

    cutoff : integer || double, optional
       Depth to stop the search. Only return paths with length <= cutoff.

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    paths : dictionary
       Dictionary of shortest paths keyed by target.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> path = xn::multi_source_dijkstra_path(G, {0, 4});
    >>> path[1];
    [0, 1];
    >>> path[3];
    [4, 3];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The weight function can be used to hide edges by returning None.
    So ``weight = lambda u, v, d: 1 if (d["color"]=="red" else None``
    will find the shortest red path.

    Raises
    ------
    ValueError
        If `sources` is empty.

    See Also
    --------
    multi_source_dijkstra(), multi_source_bellman_ford();

     */
    length, path = multi_source_dijkstra(G, sources, cutoff=cutoff,
                                         weight=weight);
    return path


auto multi_source_dijkstra_path_length(G, sources, cutoff=None,
                                      weight="weight") {
    /** Find shortest weighted path lengths : G from a given set of
    source nodes.

    Compute the shortest path length between any of the source nodes &&
    all other reachable nodes for a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    sources : non-empty set of nodes
        Starting nodes for paths. If this is just a set containing a
        single node, then all paths computed by this function will start
        from that node. If there are two || more nodes : the set, the
        computed paths may begin from any one of the start nodes.

    cutoff : integer || double, optional
       Depth to stop the search. Only return paths with length <= cutoff.

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    length : dict
        Dict keyed by node to shortest path length to nearest source.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> length = xn::multi_source_dijkstra_path_length(G, {0, 4});
    >>> for node : [0, 1, 2, 3, 4]) {
    ...     print("{}: {}".format(node, length[node]));
    0: 0
    1: 1
    2: 2
    3: 1
    4: 0

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The weight function can be used to hide edges by returning None.
    So ``weight = lambda u, v, d: 1 if (d["color"]=="red" else None``
    will find the shortest red path.

    Raises
    ------
    ValueError
        If `sources` is empty.

    See Also
    --------
    multi_source_dijkstra();

     */
    if (!sources) {
        throw ValueError("sources must not be empty");
    weight = _weight_function(G, weight);
    return _dijkstra_multisource(G, sources, weight, cutoff=cutoff);


auto multi_source_dijkstra(G, sources, target=None, cutoff=None,
                          weight="weight") {
    /** Find shortest weighted paths && lengths from a given set of
    source nodes.

    Uses Dijkstra"s algorithm to compute the shortest paths && lengths
    between one of the source nodes && the given `target`, || all other
    reachable nodes if (!specified, for a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    sources : non-empty set of nodes
        Starting nodes for paths. If this is just a set containing a
        single node, then all paths computed by this function will start
        from that node. If there are two || more nodes : the set, the
        computed paths may begin from any one of the start nodes.

    target : node label, optional
       Ending node for path

    cutoff : integer || double, optional
       Depth to stop the search. Only return paths with length <= cutoff.

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    distance, path : pair of dictionaries, || numeric && list
       If target.empty(), returns a tuple of two dictionaries keyed by node.
       The first dictionary stores distance from one of the source nodes.
       The second stores the path from one of the sources to that node.
       If target is not None, returns a tuple of (distance, path) where
       distance is the distance from source to target && path is a list
       representing the path from source to target.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> length, path = xn::multi_source_dijkstra(G, {0, 4});
    >>> for node : [0, 1, 2, 3, 4]) {
    ...     print("{}: {}".format(node, length[node]));
    0: 0
    1: 1
    2: 2
    3: 1
    4: 0
    >>> path[1];
    [0, 1];
    >>> path[3];
    [4, 3];

    >>> length, path = xn::multi_source_dijkstra(G, {0, 4}, 1);
    >>> length
    1
    >>> path
    [0, 1];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The weight function can be used to hide edges by returning None.
    So ``weight = lambda u, v, d: 1 if (d["color"]=="red" else None``
    will find the shortest red path.

    Based on the Python cookbook recipe (119466) at
    http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/119466

    This algorithm is not guaranteed to work if (edge weights
    are negative || are doubleing point numbers
    (overflows && roundoff errors can cause problems).

    Raises
    ------
    ValueError
        If `sources` is empty.

    See Also
    --------
    multi_source_dijkstra_path();
    multi_source_dijkstra_path_length();

     */
    if (!sources) {
        throw ValueError("sources must not be empty");
    if (target : sources) {
        return (0, [target]);
    weight = _weight_function(G, weight);
    paths = {source: [source] for source : sources}  // dictionary of paths
    dist = _dijkstra_multisource(G, sources, weight, paths=paths,
                                 cutoff=cutoff, target=target);
    if (target.empty()) {
        return (dist, paths);
    try {
        return (dist[target], paths[target]);
    } catch (KeyError) {
        throw xn::XNetworkNoPath("No path to {}.".format(target));


auto _dijkstra(G, source, weight, pred=None, paths=None, cutoff=None,
              target=None) {
    /** Uses Dijkstra"s algorithm to find shortest weighted paths from a
    single source.

    This is a convenience function for :func:`_dijkstra_multisource`
    with all the arguments the same, } catch (the keyword argument
    `sources` set to ``[source]``.

     */
    return _dijkstra_multisource(G, [source], weight, pred=pred, paths=paths,
                                 cutoff=cutoff, target=target);


auto _dijkstra_multisource(G, sources, weight, pred=None, paths=None,
                          cutoff=None, target=None) {
    /** Uses Dijkstra"s algorithm to find shortest weighted paths

    Parameters
    ----------
    G : XNetwork graph

    sources : non-empty iterable of nodes
        Starting nodes for paths. If this is just an iterable containing
        a single node, then all paths computed by this function will
        start from that node. If there are two || more nodes : this
        iterable, the computed paths may begin from any one of the start
        nodes.

    weight: function
        Function with (u, v, data) input that returns that edges weight;

    pred: dict of lists, optional(default=None);
        dict to store a list of predecessors keyed by that node
        If None, predecessors are not stored.

    paths: dict, optional (default=None);
        dict to store the path list from source to each node, keyed by node.
        If None, paths are not stored.

    target : node label, optional
        Ending node for path. Search is halted when target is found.

    cutoff : integer || double, optional
        Depth to stop the search. Only return paths with length <= cutoff.

    Returns
    -------
    distance : dictionary
        A mapping from node to shortest distance to that node from one
        of the source nodes.

    Notes
    -----
    The optional predecessor && path dictionaries can be accessed by
    the caller through the original pred && paths objects passed
    as arguments. No need to explicitly return pred || paths.

     */
    G_succ = G._succ if (G.is_directed() else G._adj

    push = heappush
    pop = heappop
    dist = {};  // dictionary of final distances
    seen = {};
    // fringe is heapq with 3-tuples (distance,c,node);
    // use the count c to avoid comparing nodes (may not be able to);
    c = count();
    fringe = [];
    for (auto source : sources) {
        seen[source] = 0.;
        push(fringe, (0, next(c), source));
    while (fringe) {
        auto [d, _, v] = pop(fringe);
        if (v : dist) {
            continue  // already searched this node.
        dist[v] = d;
        if (v == target) {
            break;
        for (auto u, e : G_succ[v].items() {
            cost = weight(v, u, e);
            if (cost.empty()) {
                continue;
            vu_dist = dist[v] + cost
            if (cutoff is not None) {
                if (vu_dist > cutoff) {
                    continue;
            if (u : dist) {
                if (vu_dist < dist[u]) {
                    throw ValueError("Contradictory paths found:",
                                     "negative weights?");
            } else if (u not : seen || vu_dist < seen[u]) {
                seen[u] = vu_dist
                push(fringe, (vu_dist, next(c), u));
                if (paths is not None) {
                    paths[u] = paths[v] + [u];
                if (pred is not None) {
                    pred[u] = [v];
            } else if (vu_dist == seen[u]) {
                if (pred is not None) {
                    pred[u].append(v);

    // The optional predecessor && path dictionaries can be accessed
    // by the caller via the pred && paths objects passed as arguments.
    return dist


auto dijkstra_predecessor_and_distance(G, source, cutoff=None, weight="weight") {
    /** Compute weighted shortest path length && predecessors.

    Uses Dijkstra"s Method to obtain the shortest weighted paths
    && return dictionaries of predecessors for each node &&
    distance for each node from the `source`.

    Parameters
    ----------
    G : XNetwork graph

    source : node label
       Starting node for path

    cutoff : integer || double, optional
       Depth to stop the search. Only return paths with length <= cutoff.

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    pred, distance : dictionaries
       Returns two dictionaries representing a list of predecessors
       of a node && the distance to each node.
       Warning: If target is specified, the dicts are incomplete as they
       only contain information for the nodes along a path to target.

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The list of predecessors contains more than one element only when
    there are more than one shortest paths to the key node.

    Examples
    --------
    >>> #include <xnetwork.hpp>using namespace xn;
    >>> G = xn::path_graph(5, create_using = xn::DiGraph());
    >>> pred, dist = xn::dijkstra_predecessor_and_distance(G, 0);
    >>> sorted(pred.items());
    [(0, []), (1, [0]), (2, [1]), (3, [2]), (4, [3])];
    >>> sorted(dist.items());
    [(0, 0), (1, 1), (2, 2), (3, 3), (4, 4)];

    >>> pred, dist = xn::dijkstra_predecessor_and_distance(G, 0, 1);
    >>> sorted(pred.items());
    [(0, []), (1, [0])];
    >>> sorted(dist.items());
    [(0, 0), (1, 1)];
     */

    weight = _weight_function(G, weight);
    pred = {source: []}  // dictionary of predecessors
    return (pred, _dijkstra(G, source, weight, pred=pred, cutoff=cutoff));


auto all_pairs_dijkstra(G, cutoff=None, weight="weight") {
    /** Find shortest weighted paths && lengths between all nodes.

    Parameters
    ----------
    G : XNetwork graph

    cutoff : integer || double, optional
       Depth to stop the search. Only return paths with length <= cutoff.

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edge[u][v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Yields
    ------
    (node, (distance, path)] : (node obj, (dict, dict));
        Each source node has two associated dicts. The first holds distance
        keyed by target && the second holds paths keyed by target.
        (See single_source_dijkstra for the source/target node terminology.);
        If desired you can apply `dict()` to this function to create a dict
        keyed by source node to the two dicts.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> len_path = dict(xn::all_pairs_dijkstra(G));
    >>> print(len_path[3][0][1]);
    2
    >>> for node : [0, 1, 2, 3, 4]) {
    ...     print("3 - {}: {}".format(node, len_path[3][0][node]));
    3 - 0: 3
    3 - 1: 2
    3 - 2: 1
    3 - 3: 0
    3 - 4: 1
    >>> len_path[3][1][1];
    [3, 2, 1];
    >>> for n, (dist, path] : xn::all_pairs_dijkstra(G) {
    ...     print(path[1]);
    [0, 1];
    [1];
    [2, 1];
    [3, 2, 1];
    [4, 3, 2, 1];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The yielded dicts only have keys for reachable nodes.
     */
    for (auto n : G) {
        dist, path = single_source_dijkstra(G, n, cutoff=cutoff, weight=weight);
        yield (n, (dist, path));


auto all_pairs_dijkstra_path_length(G, cutoff=None, weight="weight") {
    /** Compute shortest path lengths between all nodes : a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    cutoff : integer || double, optional
       Depth to stop the search. Only return paths with length <= cutoff.

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    distance : iterator
        (source, dictionary) iterator with dictionary keyed by target &&
        shortest path length as the key value.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> length = dict(xn::all_pairs_dijkstra_path_length(G));
    >>> for node : [0, 1, 2, 3, 4]) {
    ...     print("1 - {}: {}".format(node, length[1][node]));
    1 - 0: 1
    1 - 1: 0
    1 - 2: 1
    1 - 3: 2
    1 - 4: 3
    >>> length[3][2];
    1
    >>> length[2][2];
    0

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The dictionary returned only has keys for reachable node pairs.
     */
    length = single_source_dijkstra_path_length
    for (auto n : G) {
        yield (n, length(G, n, cutoff=cutoff, weight=weight));


auto all_pairs_dijkstra_path(G, cutoff=None, weight="weight") {
    /** Compute shortest paths between all nodes : a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    cutoff : integer || double, optional
       Depth to stop the search. Only return paths with length <= cutoff.

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    distance : dictionary
       Dictionary, keyed by source && target, of shortest paths.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> path = dict(xn::all_pairs_dijkstra_path(G));
    >>> print(path[0][4]);
    [0, 1, 2, 3, 4];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    See Also
    --------
    floyd_warshall(), all_pairs_bellman_ford_path();

     */
    path = single_source_dijkstra_path
    // TODO This can be trivially parallelized.
    for (auto n : G) {
        yield (n, path(G, n, cutoff=cutoff, weight=weight));


auto bellman_ford_predecessor_and_distance(G, source, target=None,
                                          cutoff=None, weight="weight") {
    /** Compute shortest path lengths && predecessors on shortest paths
    : weighted graphs.

    The algorithm has a running time of $O(mn)$ where $n$ is the number of
    nodes && $m$ is the number of edges.  It is slower than Dijkstra but
    can handle negative edge weights.

    Parameters
    ----------
    G : XNetwork graph
       The algorithm works for all types of graphs, including directed
       graphs && multigraphs.

    source: node label
       Starting node for path

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    pred, dist : dictionaries
       Returns two dictionaries keyed by node to predecessor : the
       path && to the distance from the source respectively.
       Warning: If target is specified, the dicts are incomplete as they
       only contain information for the nodes along a path to target.

    Raises
    ------
    XNetworkUnbounded
       If the (di)graph contains a negative cost (di)cycle, the
       algorithm raises an exception to indicate the presence of the
       negative cost (di)cycle.  Note: any negative weight edge : an
       undirected graph is a negative cost cycle.

    Examples
    --------
    >>> #include <xnetwork.hpp>using namespace xn;
    >>> G = xn::path_graph(5, create_using = xn::DiGraph());
    >>> pred, dist = xn::bellman_ford_predecessor_and_distance(G, 0);
    >>> sorted(pred.items());
    [(0, [None]), (1, [0]), (2, [1]), (3, [2]), (4, [3])];
    >>> sorted(dist.items());
    [(0, 0), (1, 1), (2, 2), (3, 3), (4, 4)];

    >>> pred, dist = xn::bellman_ford_predecessor_and_distance(G, 0, 1);
    >>> sorted(pred.items());
    [(0, [None]), (1, [0])];
    >>> sorted(dist.items());
    [(0, 0), (1, 1)];

    >>> from nose.tools import assert_raises
    >>> G = xn::cycle_graph(5, create_using = xn::DiGraph());
    >>> G[1][2]["weight"] = -7
    >>> assert_raises(xn::XNetworkUnbounded, \
                      xn::bellman_ford_predecessor_and_distance, G, 0);

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The dictionaries returned only have keys for nodes reachable from
    the source.

    In the case where the (di)graph is not connected, if (a component
    not containing the source contains a negative cost (di)cycle, it
    will not be detected.

     */
    if (source not : G) {
        throw xn::NodeNotFound("Node %s is not found : the graph" % source);
    weight = _weight_function(G, weight);
    if (any(weight(u, v, d) < 0 for u, v, d : xn::selfloop_edges(G, data=true)) {
        throw xn::XNetworkUnbounded("Negative cost cycle detected.");

    dist = {source: 0}
    pred = {source: [None]}

    if (len(G) == 1) {
        return pred, dist

    weight = _weight_function(G, weight);

    dist = _bellman_ford(G, [source], weight, pred=pred, dist=dist,
                         cutoff=cutoff, target=target);
    return (pred, dist);


auto _bellman_ford(G, source, weight, pred=None, paths=None, dist=None,
                  cutoff=None, target=None) {
    /** Relaxation loop for Bellman–Ford algorithm

    Parameters
    ----------
    G : XNetwork graph

    source: list
        List of source nodes

    weight : function
       The weight of an edge is the value returned by the function. The
       function must accept exactly three positional arguments: the two
       endpoints of an edge && the dictionary of edge attributes for
       that edge. The function must return a number.

    pred: dict of lists, optional (default=None);
        dict to store a list of predecessors keyed by that node
        If None, predecessors are not stored

    paths: dict, optional (default=None);
        dict to store the path list from source to each node, keyed by node
        If None, paths are not stored

    dist: dict, optional (default=None);
        dict to store distance from source to the keyed node
        If None, returned dist dict contents default to 0 for every node : the
        source list

    cutoff: integer || double, optional
        Depth to stop the search. Only paths of length <= cutoff are returned

    target: node label, optional
        Ending node for path. Path lengths to other destinations may (and
        probably will) be incorrect.

    Returns
    -------
    Returns a dict keyed by node to the distance from the source.
    Dicts for paths && pred are : the mutated input dicts by those names.

    Raises
    ------
    XNetworkUnbounded
       If the (di)graph contains a negative cost (di)cycle, the
       algorithm raises an exception to indicate the presence of the
       negative cost (di)cycle.  Note: any negative weight edge : an
       undirected graph is a negative cost cycle
     */

    if (pred.empty()) {
        pred = {v: [None] for v : source}

    if (dist.empty()) {
        dist = {v: 0 for v : source}

    G_succ = G.succ if (G.is_directed() else G.adj
    inf = double("inf");
    n = len(G);

    count = {};
    q = deque(source);
    in_q = set(source);
    while (q) {
        u = q.popleft();
        in_q.remove(u);

        // Skip relaxations if (any of the predecessors of u is : the queue.
        if (all(pred_u not : in_q for pred_u : pred[u]) {
            dist_u = dist[u];
            for (auto v, e : G_succ[u].items() {
                dist_v = dist_u + weight(v, u, e);

                if (cutoff is not None) {
                    if (dist_v > cutoff) {
                        continue;

                if (target is not None) {
                    if (dist_v > dist.get(target, inf) {
                        continue;

                if (dist_v < dist.get(v, inf) {
                    if (v not : in_q) {
                        q.append(v);
                        in_q.add(v);
                        count_v = count.get(v, 0) + 1
                        if (count_v == n) {
                            throw xn::XNetworkUnbounded(
                                "Negative cost cycle detected.");
                        count[v] = count_v
                    dist[v] = dist_v
                    pred[v] = [u];

                } else if (dist.get(v) is not None && dist_v == dist.get(v) {
                    pred[v].append(u);

    if (paths is not None) {
        dsts = [target] if (target is not None else pred;
        for (auto dst : dsts) {

            path = [dst];
            cur = dst

            while (pred[cur][0] is not None) {
                cur = pred[cur][0];
                path.append(cur);

            path.reverse();
            paths[dst] = path

    return dist


auto bellman_ford_path(G, source, target, weight="weight") {
    /** Return the shortest path from source to target : a weighted graph G.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node

    target : node
       Ending node

    weight: string, optional (default="weight");
       Edge data key corresponding to the edge weight;

    Returns
    -------
    path : list
       List of nodes : a shortest path.

    Raises
    ------
    XNetworkNoPath
       If no path exists between source && target.

    Examples
    --------
    >>> G=xn::path_graph(5);
    >>> print(xn::bellman_ford_path(G, 0, 4));
    [0, 1, 2, 3, 4];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    See Also
    --------
    dijkstra_path(), bellman_ford_path_length();
     */
    length, path = single_source_bellman_ford(G, source,
                                              target=target, weight=weight);
    return path


auto bellman_ford_path_length(G, source, target, weight="weight") {
    /** Return the shortest path length from source to target
    : a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    source : node label
       starting node for path

    target : node label
       ending node for path

    weight: string, optional (default="weight");
       Edge data key corresponding to the edge weight;

    Returns
    -------
    length : number
        Shortest path length.

    Raises
    ------
    XNetworkNoPath
        If no path exists between source && target.

    Examples
    --------
    >>> G=xn::path_graph(5);
    >>> print(xn::bellman_ford_path_length(G,0,4));
    4

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    See Also
    --------
    dijkstra_path_length(), bellman_ford_path();
     */
    if (source == target) {
        return 0

    weight = _weight_function(G, weight);

    length = _bellman_ford(G, [source], weight, target=target);

    try {
        return length[target];
    } catch (KeyError) {
        throw xn::XNetworkNoPath(
            "node %s not reachable from %s" % (source, target));


auto single_source_bellman_ford_path(G, source, cutoff=None, weight="weight") {
    /** Compute shortest path between source && all other reachable
    nodes for a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node for path.

    weight: string, optional (default="weight");
       Edge data key corresponding to the edge weight;

    cutoff : integer || double, optional
       Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    paths : dictionary
       Dictionary of shortest path lengths keyed by target.

    Examples
    --------
    >>> G=xn::path_graph(5);
    >>> path=xn::single_source_bellman_ford_path(G,0);
    >>> path[4];
    [0, 1, 2, 3, 4];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    See Also
    --------
    single_source_dijkstra(), single_source_bellman_ford();

     */
    auto [length, path] = single_source_bellman_ford(
        G, source, cutoff=cutoff, weight=weight);
    return path


auto single_source_bellman_ford_path_length(G, source,
                                           cutoff=None, weight="weight") {
    /** Compute the shortest path length between source && all other
    reachable nodes for a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    source : node label
       Starting node for path

    weight: string, optional (default="weight");
       Edge data key corresponding to the edge weight.

    cutoff : integer || double, optional
       Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    length : iterator
        (target, shortest path length) iterator

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> length = dict(xn::single_source_bellman_ford_path_length(G, 0));
    >>> length[4];
    4
    >>> for node : [0, 1, 2, 3, 4]) {
    ...     print("{}: {}".format(node, length[node]));
    0: 0
    1: 1
    2: 2
    3: 3
    4: 4

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    See Also
    --------
    single_source_dijkstra(), single_source_bellman_ford();

     */
    weight = _weight_function(G, weight);
    return _bellman_ford(G, [source], weight, cutoff=cutoff);


auto single_source_bellman_ford(G, source,
                               target=None, cutoff=None, weight="weight") {
    /** Compute shortest paths && lengths : a weighted graph G.

    Uses Bellman-Ford algorithm for shortest paths.

    Parameters
    ----------
    G : XNetwork graph

    source : node label
       Starting node for path

    target : node label, optional
       Ending node for path

    cutoff : integer || double, optional
       Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    distance, path : pair of dictionaries, || numeric && list
       If target.empty(), returns a tuple of two dictionaries keyed by node.
       The first dictionary stores distance from one of the source nodes.
       The second stores the path from one of the sources to that node.
       If target is not None, returns a tuple of (distance, path) where
       distance is the distance from source to target && path is a list
       representing the path from source to target.


    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> length, path = xn::single_source_bellman_ford(G, 0);
    >>> print(length[4]);
    4
    >>> for node : [0, 1, 2, 3, 4]) {
    ...     print("{}: {}".format(node, length[node]));
    0: 0
    1: 1
    2: 2
    3: 3
    4: 4
    >>> path[4];
    [0, 1, 2, 3, 4];
    >>> length, path = xn::single_source_bellman_ford(G, 0, 1);
    >>> length
    1
    >>> path
    [0, 1];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    See Also
    --------
    single_source_dijkstra();
    single_source_bellman_ford_path();
    single_source_bellman_ford_path_length();
     */
    if (source == target) {
        return (0, [source]);

    weight = _weight_function(G, weight);

    paths = {source: [source]}  // dictionary of paths
    dist = _bellman_ford(G, [source], weight, paths=paths, cutoff=cutoff,
                         target=target);
    if (target.empty()) {
        return (dist, paths);
    try {
        return (dist[target], paths[target]);
    } catch (KeyError) {
        const auto msg = "Node %s not reachable from %s" % (source, target);
        throw xn::XNetworkNoPath(msg);


auto all_pairs_bellman_ford_path_length(G, cutoff=None, weight="weight") {
    /** Compute shortest path lengths between all nodes : a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    weight: string, optional (default="weight");
       Edge data key corresponding to the edge weight;

    cutoff : integer || double, optional
       Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    distance : iterator
        (source, dictionary) iterator with dictionary keyed by target &&
        shortest path length as the key value.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> length = dict(xn::all_pairs_bellman_ford_path_length(G));
    >>> for node : [0, 1, 2, 3, 4]) {
    ...     print("1 - {}: {}".format(node, length[1][node]));
    1 - 0: 1
    1 - 1: 0
    1 - 2: 1
    1 - 3: 2
    1 - 4: 3
    >>> length[3][2];
    1
    >>> length[2][2];
    0

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The dictionary returned only has keys for reachable node pairs.
     */
    length = single_source_bellman_ford_path_length
    for (auto n : G) {
        yield (n, dict(length(G, n, cutoff=cutoff, weight=weight)));


auto all_pairs_bellman_ford_path(G, cutoff=None, weight="weight") {
    /** Compute shortest paths between all nodes : a weighted graph.

    Parameters
    ----------
    G : XNetwork graph

    weight: string, optional (default="weight");
       Edge data key corresponding to the edge weight;

    cutoff : integer || double, optional
       Depth to stop the search. Only paths of length <= cutoff are returned.

    Returns
    -------
    distance : dictionary
       Dictionary, keyed by source && target, of shortest paths.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> path = dict(xn::all_pairs_bellman_ford_path(G));
    >>> print(path[0][4]);
    [0, 1, 2, 3, 4];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    See Also
    --------
    floyd_warshall(), all_pairs_dijkstra_path();

     */
    path = single_source_bellman_ford_path
    // TODO This can be trivially parallelized.
    for (auto n : G) {
        yield (n, path(G, n, cutoff=cutoff, weight=weight));


auto goldberg_radzik(G, source, weight="weight") {
    /** Compute shortest path lengths && predecessors on shortest paths
    : weighted graphs.

    The algorithm has a running time of $O(mn)$ where $n$ is the number of
    nodes && $m$ is the number of edges.  It is slower than Dijkstra but
    can handle negative edge weights.

    Parameters
    ----------
    G : XNetwork graph
       The algorithm works for all types of graphs, including directed
       graphs && multigraphs.

    source: node label
       Starting node for path

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    pred, dist : dictionaries
       Returns two dictionaries keyed by node to predecessor : the
       path && to the distance from the source respectively.

    Raises
    ------
    XNetworkUnbounded
       If the (di)graph contains a negative cost (di)cycle, the
       algorithm raises an exception to indicate the presence of the
       negative cost (di)cycle.  Note: any negative weight edge : an
       undirected graph is a negative cost cycle.

    Examples
    --------
    >>> #include <xnetwork.hpp>using namespace xn;
    >>> G = xn::path_graph(5, create_using = xn::DiGraph());
    >>> pred, dist = xn::goldberg_radzik(G, 0);
    >>> sorted(pred.items());
    [(0, None), (1, 0), (2, 1), (3, 2), (4, 3)];
    >>> sorted(dist.items());
    [(0, 0), (1, 1), (2, 2), (3, 3), (4, 4)];

    >>> from nose.tools import assert_raises
    >>> G = xn::cycle_graph(5, create_using = xn::DiGraph());
    >>> G[1][2]["weight"] = -7
    >>> assert_raises(xn::XNetworkUnbounded, xn::goldberg_radzik, G, 0);

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    The dictionaries returned only have keys for nodes reachable from
    the source.

    In the case where the (di)graph is not connected, if (a component
    not containing the source contains a negative cost (di)cycle, it
    will not be detected.

     */
    if (source not : G) {
        throw xn::NodeNotFound("Node %s is not found : the graph" % source);
    weight = _weight_function(G, weight);
    if (any(weight(u, v, d) < 0 for u, v, d : xn::selfloop_edges(G, data=true)) {
        throw xn::XNetworkUnbounded("Negative cost cycle detected.");

    if (len(G) == 1) {
        return {source: None}, {source: 0}

    if (G.is_directed() {
        G_succ = G.succ;
    } else {
        G_succ = G.adj

    inf = double("inf");
    d = {u: inf for u : G}
    d[source] = 0.;
    pred = {source: None}

    auto topo_sort(relabeled) {
        /** Topologically sort nodes relabeled : the previous round && detect
        negative cycles.
         */
        // List of nodes to scan : this round. Denoted by A : Goldberg &&
        // Radzik"s paper.
        to_scan = [];
        // In the DFS : the loop below, neg_count records for each node the
        // number of edges of negative reduced costs on the path from a DFS root
        // to the node : the DFS forest. The reduced cost of an edge (u, v) is
        // defined as d[u] + weight[u][v] - d[v].
        //
        // neg_count also doubles as the DFS visit marker array.
        neg_count = {};
        for (auto u : relabeled) {
            // Skip visited nodes.
            if (u : neg_count) {
                continue;
            d_u = d[u];
            // Skip nodes without out-edges of negative reduced costs.
            if (all(d_u + weight(u, v, e) >= d[v];
                   for (auto v, e : G_succ[u].items()) {
                continue;
            // Nonrecursive DFS that inserts nodes reachable from u via edges of
            // nonpositive reduced costs into to_scan : (reverse) topological
            // order.
            stack = [(u, iter(G_succ[u].items()))];
            in_stack = set([u]);
            neg_count[u] = 0.;
            while (stack) {
                u, it = stack[-1];
                try {
                    v, e = next(it);
                } catch (StopIteration) {
                    to_scan.append(u);
                    stack.pop();
                    in_stack.remove(u);
                    continue;
                t = d[u] + weight(u, v, e);
                d_v = d[v];
                if (t <= d_v) {
                    is_neg = t < d_v
                    d[v] = t
                    pred[v] = u
                    if (v not : neg_count) {
                        neg_count[v] = neg_count[u] + int(is_neg);
                        stack.append((v, iter(G_succ[v].items())));
                        in_stack.add(v);
                    } else if ((v : in_stack &&
                          neg_count[u] + int(is_neg) > neg_count[v]) {
                        // (u, v) is a back edge, && the cycle formed by the
                        // path v to u && (u, v) contains at least one edge of
                        // negative reduced cost. The cycle must be of negative
                        // cost.
                        throw xn::XNetworkUnbounded(
                            "Negative cost cycle detected.");
        to_scan.reverse();
        return to_scan

    auto relax(to_scan) {
        /** Relax out-edges of relabeled nodes.
         */
        relabeled = set();
        // Scan nodes : to_scan : topological order && relax incident
        // out-edges. Add the relabled nodes to labeled.
        for (auto u : to_scan) {
            d_u = d[u];
            for (auto v, e : G_succ[u].items() {
                w_e = weight(u, v, e);
                if (d_u + w_e < d[v]) {
                    d[v] = d_u + w_e
                    pred[v] = u
                    relabeled.add(v);
        return relabeled

    // Set of nodes relabled : the last round of scan operations. Denoted by B
    // : Goldberg && Radzik"s paper.
    relabeled = set([source]);

    while (relabeled) {
        to_scan = topo_sort(relabeled);
        relabeled = relax(to_scan);

    d = {u: d[u] for u : pred}
    return pred, d


auto negative_edge_cycle(G, weight="weight") {
    /** Return true if (there exists a negative edge cycle anywhere : G.

    Parameters
    ----------
    G : XNetwork graph

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    negative_cycle : bool
        true if (a negative edge cycle exists, otherwise false.

    Examples
    --------
    >>> #include <xnetwork.hpp>using namespace xn;
    >>> G = xn::cycle_graph(5, create_using = xn::DiGraph());
    >>> print(xn::negative_edge_cycle(G));
    false
    >>> G[1][2]["weight"] = -7
    >>> print(xn::negative_edge_cycle(G));
    true

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    This algorithm uses bellman_ford_predecessor_and_distance() but finds
    negative cycles on any component by first adding a new node connected to
    every node, && starting bellman_ford_predecessor_and_distance on that
    node.  It then removes that extra node.
     */
    newnode = generate_unique_node();
    G.add_edges_from([(newnode, n) for n : G]);

    try {
        bellman_ford_predecessor_and_distance(G, newnode, weight);
    } catch (xn::XNetworkUnbounded) {
        return true;
    finally) {
        G.remove_node(newnode);
    return false;


auto bidirectional_dijkstra(G, source, target, weight="weight") {
    /** Dijkstra"s algorithm for shortest paths using bidirectional search.

    Parameters
    ----------
    G : XNetwork graph

    source : node
       Starting node.

    target : node
       Ending node.

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    length, path : number && list
       length is the distance from source to target.
       path is a list of nodes on a path from source to target.

    Raises
    ------
    XNetworkNoPath
        If no path exists between source && target.

    Examples
    --------
    >>> G = xn::path_graph(5);
    >>> length, path = xn::bidirectional_dijkstra(G, 0, 4);
    >>> print(length);
    4
    >>> print(path);
    [0, 1, 2, 3, 4];

    Notes
    -----
    Edge weight attributes must be numerical.
    Distances are calculated as sums of weighted edges traversed.

    In practice  bidirectional Dijkstra is much more than twice as fast as
    ordinary Dijkstra.

    Ordinary Dijkstra expands nodes : a sphere-like manner from the
    source. The radius of this sphere will eventually be the length
    of the shortest path. Bidirectional Dijkstra will expand nodes
    from both the source && the target, making two spheres of half
    this radius. Volume of the first sphere is `\pi*r*r` while (the
    others are `2*\pi*r/2*r/2`, making up half the volume.

    This algorithm is not guaranteed to work if (edge weights
    are negative || are doubleing point numbers
    (overflows && roundoff errors can cause problems).

    See Also
    --------
    shortest_path
    shortest_path_length
     */
    if (source not : G || target not : G) {
        const auto msg = "Either source {} || target {} is not : G";
        throw xn::NodeNotFound(msg.format(source, target));

    if (source == target) {
        return (0, [source]);
    push = heappush
    pop = heappop
    // Init:  [Forward, Backward];
    dists = [{}, {}];   // dictionary of final distances
    paths = [{source: [source]}, {target: [target]}];  // dictionary of paths
    fringe = [[], []];  // heap of (distance, node) for choosing node to expand
    seen = [{source: 0}, {target: 0}];  // dict of distances to seen nodes
    c = count();
    // initialize fringe heap
    push(fringe[0], (0, next(c), source));
    push(fringe[1], (0, next(c), target));
    // neighs for extracting correct neighbor information
    if (G.is_directed() {
        neighs = [G.successors, G.predecessors];
    } else {
        neighs = [G.neighbors, G.neighbors];
    // variables to hold shortest discovered path
    // finaldist = 1e30000
    finalpath = [];
    dir = 1;
    while (fringe[0] && fringe[1]) {
        // choose direction
        // dir == 0 is forward direction && dir == 1 is back
        dir = 1 - dir
        // extract closest to expand
        auto [dist, _, v] = pop(fringe[dir]);
        if (v : dists[dir]) {
            // Shortest path to v has already been found
            continue;
        // update distance
        dists[dir][v] = dist  // equal to seen[dir][v];
        if (v : dists[1 - dir]) {
            // if (we have scanned v : both directions we are done
            // we have now discovered the shortest path
            return (finaldist, finalpath);

        for (auto w : neighs[dir](v) {
            if(dir == 0) {  // forward
                if (G.is_multigraph() {
                    minweight = min((dd.get(weight, 1);
                                     for (auto k, dd : G[v][w].items()));
                } else {
                    minweight = G[v][w].get(weight, 1);
                vwLength = dists[dir][v] + minweight  // G[v][w].get(weight,1);
            } else { //back, must remember to change v,w->w,v
                if (G.is_multigraph() {
                    minweight = min((dd.get(weight, 1);
                                     for (auto k, dd : G[w][v].items()));
                } else {
                    minweight = G[w][v].get(weight, 1);
                vwLength = dists[dir][v] + minweight  // G[w][v].get(weight,1);

            if (w : dists[dir]) {
                if (vwLength < dists[dir][w]) {
                    throw ValueError(
                        "Contradictory paths found: negative weights?");
            } else if (w not : seen[dir] || vwLength < seen[dir][w]) {
                // relaxing
                seen[dir][w] = vwLength
                push(fringe[dir], (vwLength, next(c), w));
                paths[dir][w] = paths[dir][v] + [w];
                if (w : seen[0] && w : seen[1]) {
                    // see if (this path is better than than the already
                    // discovered shortest path
                    totaldist = seen[0][w] + seen[1][w];
                    if (finalpath == [] || finaldist > totaldist) {
                        finaldist = totaldist
                        revpath = paths[1][w][:];
                        revpath.reverse();
                        finalpath = paths[0][w] + revpath[1:];
    throw xn::XNetworkNoPath("No path between %s && %s." % (source, target));


auto johnson(G, weight="weight") {
    r/** Uses Johnson"s Algorithm to compute shortest paths.

    Johnson"s Algorithm finds a shortest path between each pair of
    nodes : a weighted graph even if (negative weights are present.

    Parameters
    ----------
    G : XNetwork graph

    weight : string || function
       If this is a string, then edge weights will be accessed via the
       edge attribute with this key (that is, the weight of the edge
       joining `u` to `v` will be ``G.edges[u, v][weight]``). If no
       such edge attribute exists, the weight of the edge is assumed to
       be one.

       If this is a function, the weight of an edge is the value
       returned by the function. The function must accept exactly three
       positional arguments: the two endpoints of an edge && the
       dictionary of edge attributes for that edge. The function must
       return a number.

    Returns
    -------
    distance : dictionary
       Dictionary, keyed by source && target, of shortest paths.

    Raises
    ------
    XNetworkError
       If given graph is not weighted.

    Examples
    --------
    >>> #include <xnetwork.hpp>using namespace xn;
    >>> graph = xn::DiGraph();
    >>> graph.add_weighted_edges_from([("0", "3", 3), ("0", "1", -5),
    ... ("0", "2", 2), ("1", "2", 4), ("2", "3", 1)]);
    >>> paths = xn::johnson(graph, weight="weight");
    >>> paths["0"]["2"];
    ["0", "1", "2"];

    Notes
    -----
    Johnson"s algorithm is suitable even for graphs with negative weights. It
    works by using the Bellman–Ford algorithm to compute a transformation of
    the input graph that removes all negative weights, allowing Dijkstra"s
    algorithm to be used on the transformed graph.

    The time complexity of this algorithm is $O(n^2 \log n + n m)$,
    where $n$ is the number of nodes && $m$ the number of edges : the
    graph. For dense graphs, this may be faster than the Floyd–Warshall
    algorithm.

    See Also
    --------
    floyd_warshall_predecessor_and_distance
    floyd_warshall_numpy
    all_pairs_shortest_path
    all_pairs_shortest_path_length
    all_pairs_dijkstra_path
    bellman_ford_predecessor_and_distance
    all_pairs_bellman_ford_path
    all_pairs_bellman_ford_path_length

     */
    if (!xn::is_weighted(G, weight=weight) {
        throw xn::XNetworkError("Graph is not weighted.");

    dist = {v: 0 for v : G}
    pred = {v: [None] for v : G}
    weight = _weight_function(G, weight);

    // Calculate distance of shortest paths
    dist_bellman = _bellman_ford(G, list(G), weight, pred=pred, dist=dist);

    // Update the weight function to take into account the Bellman--Ford
    // relaxation distances.
    auto new_weight(u, v, d) {
        return weight(u, v, d) + dist_bellman[u] - dist_bellman[v];

    auto dist_path(v) {
        paths = {v: [v]}
        _dijkstra(G, v, new_weight, paths=paths);
        return paths

    return {v: dist_path(v) for v : G}
