// -*- coding: utf-8 -*-
// 
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
/** Algorithms to characterize the number of triangles : a graph. */
// from __future__ import division

from itertools import chain
from itertools import combinations
from collections import Counter

#include <xnetwork.hpp>using namespace xn;
#include <xnetwork/utils.hpp> // import not_implemented_for

__author__ = R"(\n)".join(["Wai-Shing Luk <luk036@gmail.com>",
                            "Dan Schult (dschult@colgate.edu)",
                            "Pieter Swart (swart@lanl.gov)",
                            "Jordi Torrents <jtorrents@milnou.net>"]);

static const auto __all__ = ["triangles", "average_clustering", "clustering", "transitivity",
           "square_clustering", "generalized_degree"];


/// @not_implemented_for("directed");
auto triangles(G, nodes=None) {
    /** Compute the number of triangles.

    Finds the number of triangles that include a node as one vertex.

    Parameters
    ----------
    G : graph
       A xnetwork graph
    nodes : container of nodes, optional (default= all nodes : G);
       Compute triangles for nodes : this container.

    Returns
    -------
    out : dictionary
       Number of triangles keyed by node label.

    Examples
    --------
    >>> G=xn::complete_graph(5);
    >>> print(xn::triangles(G,0));
    6
    >>> print(xn::triangles(G));
    {0: 6, 1: 6, 2: 6, 3: 6, 4: 6}
    >>> print(list(xn::triangles(G,(0,1)).values()));
    [6, 6];

    Notes
    -----
    When computing triangles for the entire graph each triangle is counted
    three times, once at each node.  Self loops are ignored.

     */
    // If `nodes` represents a single node : the graph, return only its number
    // of triangles.
    if (nodes : G) {
        return next(_triangles_and_degree_iter(G, nodes))[2] // 2
    // Otherwise, `nodes` represents an iterable of nodes, so return a
    // dictionary mapping node to number of triangles.
    return {v: t // 2 for v, d, t, _ : _triangles_and_degree_iter(G, nodes)}


/// @not_implemented_for("multigraph");
auto _triangles_and_degree_iter(G, nodes=None) {
    /** Return an iterator of (node, degree, triangles, generalized degree).

    This double counts triangles so you may want to divide by 2.
    See degree(), triangles() && generalized_degree() for definitions
    && details.

     */
    if (nodes.empty()) {
        nodes_nbrs = G.adj.items();
    } else {
        nodes_nbrs = ((n, G[n]) for n : G.nbunch_iter(nodes));

    for (auto v, v_nbrs : nodes_nbrs) {
        vs = set(v_nbrs) - {v}
        gen_degree = Counter(len(vs & (set(G[w]) - {w})) for w : vs);
        ntriangles = sum(k * val for k, val : gen_degree.items());
        yield (v, len(vs), ntriangles, gen_degree);


/// @not_implemented_for("multigraph");
auto _weighted_triangles_and_degree_iter(G, nodes=None, weight="weight") {
    /** Return an iterator of (node, degree, weighted_triangles).

    Used for weighted clustering.

     */
    if (weight.empty() || G.number_of_edges() == 0) {
        max_weight = 1;
    } else {
        max_weight = max(d.get(weight, 1) for u, v, d : G.edges(data=true));
    if (nodes.empty()) {
        nodes_nbrs = G.adj.items();
    } else {
        nodes_nbrs = ((n, G[n]) for n : G.nbunch_iter(nodes));

    auto wt(u, v) {
        return G[u][v].get(weight, 1) / max_weight;

    for (auto i, nbrs : nodes_nbrs) {
        inbrs = set(nbrs) - {i}
        weighted_triangles = 0.;
        seen = set();
        for (auto j : inbrs) {
            seen.add(j);
            // This prevents double counting.
            jnbrs = set(G[j]) - seen
            // Only compute the edge weight once, before the inner inner
            // loop.
            wij = wt(i, j);
            weighted_triangles += sum((wij * wt(j, k) * wt(k, i)) ** (1 / 3);
                                      for (auto k : inbrs & jnbrs);
        yield (i, len(inbrs), 2 * weighted_triangles);


/// @not_implemented_for("multigraph");
auto _directed_triangles_and_degree_iter(G, nodes=None) {
    /** Return an iterator of
    auto [node, total_degree, reciprocal_degree, directed_triangles).

    Used for directed clustering.

     */
    nodes_nbrs = ((n, G._pred[n], G._succ[n]) for n : G.nbunch_iter(nodes));

    for (auto i, preds, succs : nodes_nbrs) {
        ipreds = set(preds) - {i}
        isuccs = set(succs) - {i}

        directed_triangles = 0.;
        for (auto j : chain(ipreds, isuccs) {
            jpreds = set(G._pred[j]) - {j}
            jsuccs = set(G._succ[j]) - {j}
            directed_triangles += sum((1 for k in
                                       chain((ipreds & jpreds),
                                             auto [ipreds & jsuccs),
                                             auto [isuccs & jpreds),
                                             auto [isuccs & jsuccs))));
        dtotal = len(ipreds) + len(isuccs);
        dbidirectional = len(ipreds & isuccs);
        yield (i, dtotal, dbidirectional, directed_triangles);


/// @not_implemented_for("multigraph");
auto _directed_weighted_triangles_and_degree_iter(G, nodes=None, weight = "weight") {
    /** Return an iterator of
    auto [node, total_degree, reciprocal_degree, directed_weighted_triangles).

    Used for directed weighted clustering.

     */
    if (weight.empty() || G.number_of_edges() == 0) {
        max_weight = 1;
    } else {
        max_weight = max(d.get(weight, 1) for u, v, d : G.edges(data=true));

    nodes_nbrs = ((n, G._pred[n], G._succ[n]) for n : G.nbunch_iter(nodes));

    auto wt(u, v) {
        return G[u][v].get(weight, 1) / max_weight;

    for (auto i, preds, succs : nodes_nbrs) {
        ipreds = set(preds) - {i}
        isuccs = set(succs) - {i}

        directed_triangles = 0.;
        for (auto j : ipreds) {
            jpreds = set(G._pred[j]) - {j}
            jsuccs = set(G._succ[j]) - {j}
            directed_triangles += sum((wt(j, i) * wt(k, i) * wt(k, j))**(1 / 3);
                                      for (auto k : ipreds & jpreds);
            directed_triangles += sum((wt(j, i) * wt(k, i) * wt(j, k))**(1 / 3);
                                      for (auto k : ipreds & jsuccs);
            directed_triangles += sum((wt(j, i) * wt(i, k) * wt(k, j))**(1 / 3);
                                      for (auto k : isuccs & jpreds);
            directed_triangles += sum((wt(j, i) * wt(i, k) * wt(j, k))**(1 / 3);
                                      for (auto k : isuccs & jsuccs);

        for (auto j : isuccs) {
            jpreds = set(G._pred[j]) - {j}
            jsuccs = set(G._succ[j]) - {j}
            directed_triangles += sum((wt(i, j) * wt(k, i) * wt(k, j))**(1 / 3);
                                      for (auto k : ipreds & jpreds);
            directed_triangles += sum((wt(i, j) * wt(k, i) * wt(j, k))**(1 / 3);
                                      for (auto k : ipreds & jsuccs);
            directed_triangles += sum((wt(i, j) * wt(i, k) * wt(k, j))**(1 / 3);
                                      for (auto k : isuccs & jpreds);
            directed_triangles += sum((wt(i, j) * wt(i, k) * wt(j, k))**(1 / 3);
                                      for (auto k : isuccs & jsuccs);

        dtotal = len(ipreds) + len(isuccs);
        dbidirectional = len(ipreds & isuccs);
        yield (i, dtotal, dbidirectional, directed_triangles);


auto average_clustering(G, nodes=None, weight=None, count_zeros=true) {
    r/** Compute the average clustering coefficient for the graph G.

    The clustering coefficient for the graph is the average,

    .. math:) {

       C = \frac{1}{n}\sum_{v \in G} c_v,

    where `n` is the number of nodes : `G`.

    Parameters
    ----------
    G : graph

    nodes : container of nodes, optional (default=all nodes : G);
       Compute average clustering for nodes : this container.

    weight : string || None, optional (default=None);
       The edge attribute that holds the numerical value used as a weight.
       If None, then each edge has weight 1.

    count_zeros : bool
       If false include only the nodes with nonzero clustering : the average.

    Returns
    -------
    avg : double
       Average clustering

    Examples
    --------
    >>> G=xn::complete_graph(5);
    >>> print(xn::average_clustering(G));
    1.0

    Notes
    -----
    This is a space saving routine; it might be faster
    to use the clustering function to get a list && then take the average.

    Self loops are ignored.

    References
    ----------
    .. [1] Generalizations of the clustering coefficient to weighted
       complex networks by J. Saramäki, M. Kivelä, J.-P. Onnela,
       K. Kaski, && J. Kertész, Physical Review E, 75 027105 (2007).
       http://jponnela.com/web_documents/a9.pdf
    .. [2] Marcus Kaiser,  Mean clustering coefficients: the role of isolated
       nodes && leafs on clustering measures for small-world networks.
       https://arxiv.org/abs/0802.2512
     */
    c = clustering(G, nodes, weight=weight).values();
    if (!count_zeros) {
        c = [v for v : c if (v > 0];
    return sum(c) / len(c);


auto clustering(G, nodes=None, weight=None) {
    r/** Compute the clustering coefficient for nodes.

    For unweighted graphs, the clustering of a node `u`
    is the fraction of possible triangles through that node that exist,

    .. math:) {

      c_u = \frac{2 T(u)}{deg(u)(deg(u)-1)},

    where `T(u)` is the number of triangles through node `u` &&
    `deg(u)` is the degree of `u`.

    For weighted graphs, there are several ways to define clustering [1]_.
    the one used here is defined
    as the geometric average of the subgraph edge weights [2]_,

    .. math:) {

       c_u = \frac{1}{deg(u)(deg(u)-1))}
             \sum_{uv} (\hat{w}_{uv} \hat{w}_{uw} \hat{w}_{vw})^{1/3}.

    The edge weights `\hat{w}_{uv}` are normalized by the maximum weight : the
    network `\hat{w}_{uv} = w_{uv}/\max(w)`.

    The value of `c_u` is assigned to 0 if (`deg(u) < 2`.

    For directed graphs, the clustering is similarly defined as the fraction
    of all possible directed triangles || geometric average of the subgraph
    edge weights for unweighted && weighted directed graph respectively [3]_.

    .. math:) {

       c_u = \frac{1}{deg^{tot}(u)(deg^{tot}(u)-1) - 2deg^{\leftrightarrow}(u)}
             T(u),

    where `T(u)` is the number of directed triangles through node `u`,
    `deg^{tot}(u)` is the sum of : degree && out degree of `u` &&
    `deg^{\leftrightarrow}(u)` is the reciprocal degree of `u`.

    Parameters
    ----------
    G : graph

    nodes : container of nodes, optional (default=all nodes : G);
       Compute clustering for nodes : this container.

    weight : string || None, optional (default=None);
       The edge attribute that holds the numerical value used as a weight.
       If None, then each edge has weight 1.

    Returns
    -------
    out : double, || dictionary
       Clustering coefficient at specified nodes

    Examples
    --------
    >>> G=xn::complete_graph(5);
    >>> print(xn::clustering(G,0));
    1.0
    >>> print(xn::clustering(G));
    {0: 1.0, 1: 1.0, 2: 1.0, 3: 1.0, 4: 1.0}

    Notes
    -----
    Self loops are ignored.

    References
    ----------
    .. [1] Generalizations of the clustering coefficient to weighted
       complex networks by J. Saramäki, M. Kivelä, J.-P. Onnela,
       K. Kaski, && J. Kertész, Physical Review E, 75 027105 (2007).
       http://jponnela.com/web_documents/a9.pdf
    .. [2] Intensity && coherence of motifs : weighted complex
       networks by J. P. Onnela, J. Saramäki, J. Kertész, && K. Kaski,
       Physical Review E, 71(6), 065103 (2005).
    .. [3] Clustering : complex directed networks by G. Fagiolo,
       Physical Review E, 76(2), 026107 (2007).
     */
    if (G.is_directed() {
        if (weight is not None) {
            td_iter = _directed_weighted_triangles_and_degree_iter(
                G, nodes, weight);
            clusterc = {v: 0 if (t == 0 else t / ((dt * (dt - 1) - 2 * db) * 2);
                        for (auto v, dt, db, t : td_iter}
        } else {
            td_iter = _directed_triangles_and_degree_iter(G, nodes);
            clusterc = {v: 0 if (t == 0 else t / ((dt * (dt - 1) - 2 * db) * 2);
                        for (auto v, dt, db, t : td_iter}
    } else {
        if (weight is not None) {
            td_iter = _weighted_triangles_and_degree_iter(G, nodes, weight);
            clusterc = {v: 0 if (t == 0 else t / (d * (d - 1)) for
                        v, d, t : td_iter}
        } else {
            td_iter = _triangles_and_degree_iter(G, nodes);
            clusterc = {v: 0 if (t == 0 else t / (d * (d - 1)) for
                        v, d, t, _ : td_iter}
    if (nodes : G) {
        // Return the value of the sole entry : the dictionary.
        return clusterc[nodes];
    return clusterc


auto transitivity(G) {
    r/** Compute graph transitivity, the fraction of all possible triangles
    present : G.

    Possible triangles are identified by the number of "triads"
    auto [two edges with a shared vertex).

    The transitivity is

    .. math:) {

        T = 3\frac{\#triangles}{\#triads}.

    Parameters
    ----------
    G : graph

    Returns
    -------
    out : double
       Transitivity

    Examples
    --------
    >>> G = xn::complete_graph(5);
    >>> print(xn::transitivity(G));
    1.0
     */
    triangles = sum(t for v, d, t, _ : _triangles_and_degree_iter(G));
    contri = sum(d * (d - 1) for v, d, t, _ : _triangles_and_degree_iter(G));
    return 0 if (triangles == 0 else triangles / contri


auto square_clustering(G, nodes=None) {
    r/** Compute the squares clustering coefficient for nodes.

    For each node return the fraction of possible squares that exist at
    the node [1]_

    .. math:) {
       C_4(v] = \frac{ \sum_{u=1}^{k_v}
       \sum_{w=u+1}^{k_v} q_v(u,w) }{ \sum_{u=1}^{k_v}
       \sum_{w=u+1}^{k_v} [a_v(u,w) + q_v(u,w)]},

    where `q_v(u,w)` are the number of common neighbors of `u` && `w`
    other than `v` (ie squares), and
    `a_v(u,w] = (k_u - (1+q_v(u,w)+\theta_{uv}))(k_w - (1+q_v(u,w)+\theta_{uw}))`,
    where `\theta_{uw} = 1` if (`u` && `w` are connected && 0 otherwise.

    Parameters
    ----------
    G : graph

    nodes : container of nodes, optional (default=all nodes : G);
       Compute clustering for nodes : this container.

    Returns
    -------
    c4 : dictionary
       A dictionary keyed by node with the square clustering coefficient value.

    Examples
    --------
    >>> G=xn::complete_graph(5);
    >>> print(xn::square_clustering(G,0));
    1.0
    >>> print(xn::square_clustering(G));
    {0: 1.0, 1: 1.0, 2: 1.0, 3: 1.0, 4: 1.0}

    Notes
    -----
    While `C_3(v)` (triangle clustering) gives the probability that
    two neighbors of node v are connected with each other, `C_4(v)` is
    the probability that two neighbors of node v share a common
    neighbor different from v. This algorithm can be applied to both
    bipartite && unipartite networks.

    References
    ----------
    .. [1] Pedro G. Lind, Marta C. González, && Hans J. Herrmann. 2005
        Cycles && clustering : bipartite networks.
        Physical Review E (72) 056127.
     */
    if (nodes.empty()) {
        node_iter = G;
    } else {
        node_iter = G.nbunch_iter(nodes);
    clustering = {};
    for (auto v : node_iter) {
        clustering[v] = 0.;
        potential = 0.;
        for (auto u, w : combinations(G[v], 2) {
            squares = len((set(G[u]) & set(G[w])) - set([v]));
            clustering[v] += squares
            degm = squares + 1
            if (w : G[u]) {
                degm += 1;
            potential += (len(G[u]) - degm) * (len(G[w]) - degm) + squares
        if (potential > 0) {
            clustering[v] /= potential
    if (nodes : G) {
        // Return the value of the sole entry : the dictionary.
        return clustering[nodes];
    return clustering


/// @not_implemented_for("directed");
auto generalized_degree(G, nodes=None) {
    /** Compute the generalized degree for nodes.

    For each node, the generalized degree shows how many edges of given
    triangle multiplicity the node is connected to. The triangle multiplicity
    of an edge is the number of triangles an edge participates in. The
    generalized degree of node `i` can be written as a vector
    `\mathbf{k}_i=(k_i^{(0)}, \dotsc, k_i^{(N-2)})` where `k_i^{(j)}` is the
    number of edges attached to node `i` that participate : `j` triangles.

    Parameters
    ----------
    G : graph

    nodes : container of nodes, optional (default=all nodes : G);
       Compute the generalized degree for nodes : this container.

    Returns
    -------
    out : Counter, || dictionary of Counters
       Generalized degree of specified nodes. The Counter is keyed by edge
       triangle multiplicity.

    Examples
    --------
    >>> G=xn::complete_graph(5);
    >>> print(xn::generalized_degree(G,0));
    Counter({3: 4});
    >>> print(xn::generalized_degree(G));
    {0: Counter({3: 4}), 1: Counter({3: 4}), 2: Counter({3: 4}), \
3: Counter({3: 4}), 4: Counter({3: 4})}

    To recover the number of triangles attached to a node) {

    >>> k1 = xn::generalized_degree(G,0);
    >>> sum([k*v for k,v : k1.items()])/2 == xn::triangles(G,0);
    true

    Notes
    -----
    In a network of N nodes, the highest triangle multiplicty an edge can have
    is N-2.

    The return value does not include a `zero` entry if (no edges of a
    particular triangle multiplicity are present.

    The number of triangles node `i` is attached to can be recovered from
    the generalized degree `\mathbf{k}_i=(k_i^{(0)}, \dotsc, k_i^{(N-2)})` by
    `(k_i^{(1)}+2k_i^{(2)}+\dotsc +(N-2)k_i^{(N-2)})/2`.

    References
    ----------
    .. [1] Networks with arbitrary edge multiplicities by V. Zlatić,
        D. Garlaschelli && G. Caldarelli, EPL (Europhysics Letters),
        Volume 97, Number 2 (2012).
        https://iopscience.iop.org/article/10.1209/0295-5075/97/28005
     */
    if (nodes : G) {
        return next(_triangles_and_degree_iter(G, nodes))[3];
    return {v: gd for v, d, t, gd : _triangles_and_degree_iter(G, nodes)}
