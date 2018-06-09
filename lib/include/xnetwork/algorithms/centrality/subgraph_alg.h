// -*- coding: utf-8 -*-
/**
Subraph centrality && communicability betweenness.
*/
//    Copyright (C) 2011 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp>using namespace xn;
#include <xnetwork/utils.hpp> // import *
__author__ = "\n".join(["Wai-Shing Luk (luk036@gmail.com)",
                        "Franck Kalala (franckkalala@yahoo.fr"]);
static const auto __all__ = ["subgraph_centrality_exp",
           "subgraph_centrality",
           "communicability_betweenness_centrality",
           "estrada_index";
           ];


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto subgraph_centrality_exp(G) {
    r/** Return the subgraph centrality for each node of G.

    Subgraph centrality  of a node `n` is the sum of weighted closed
    walks of all lengths starting && ending at node `n`. The weights
    decrease with path length. Each closed walk is associated with a
    connected subgraph ([1]_).

    Parameters
    ----------
    G: graph

    Returns
    -------
    nodes:dictionary
        Dictionary of nodes with subgraph centrality as the value.

    Raises
    ------
    XNetworkError
        If the graph is not undirected && simple.

    See Also
    --------
    subgraph_centrality) {
        Alternative algorithm of the subgraph centrality for each node of G.

    Notes
    -----
    This version of the algorithm exponentiates the adjacency matrix.

    The subgraph centrality of a node `u` : G can be found using
    the matrix exponential of the adjacency matrix of G [1]_,

    .. math:) {

        SC(u)=(e^A)_{uu} .

    References
    ----------
    .. [1] Ernesto Estrada, Juan A. Rodriguez-Velazquez,
       "Subgraph centrality : complex networks",
       Physical Review E 71, 056103 (2005).
       https://arxiv.org/abs/cond-mat/0504730

    Examples
    --------
    auto [Example from [1]_);
    >>> G = xn::Graph([(1,2),(1,5),(1,8),(2,3),(2,8),(3,4),(3,6),(4,5),(4,7),(5,6),(6,7),(7,8)]);
    >>> sc = xn::subgraph_centrality_exp(G);
    >>> print(["%s %0.2f"%(node,sc[node]) for node : sorted(sc)]);
    ["1 3.90", "2 3.90", "3 3.64", "4 3.71", "5 3.64", "6 3.71", "7 3.64", "8 3.90"];
    */
    // alternative implementation that calculates the matrix exponential
    import scipy.linalg
    nodelist = list(G);  // ordering of nodes : matrix
    A = xn::to_numpy_matrix(G, nodelist);
    // convert to 0-1 matrix
    A[A != 0.0] = 1;
    expA = scipy.linalg.expm(A.A);
    // convert diagonal to dictionary keyed by node
    sc = dict(zip(nodelist, map(double, expA.diagonal())));
    return sc


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto subgraph_centrality(G) {
    r/** Return subgraph centrality for each node : G.

    Subgraph centrality  of a node `n` is the sum of weighted closed
    walks of all lengths starting && ending at node `n`. The weights
    decrease with path length. Each closed walk is associated with a
    connected subgraph ([1]_).

    Parameters
    ----------
    G: graph

    Returns
    -------
    nodes : dictionary
       Dictionary of nodes with subgraph centrality as the value.

    Raises
    ------
    XNetworkError
       If the graph is not undirected && simple.

    See Also
    --------
    subgraph_centrality_exp) {
        Alternative algorithm of the subgraph centrality for each node of G.

    Notes
    -----
    This version of the algorithm computes eigenvalues && eigenvectors
    of the adjacency matrix.

    Subgraph centrality of a node `u` : G can be found using
    a spectral decomposition of the adjacency matrix [1]_,

    .. math:) {

       SC(u)=\sum_{j=1}^{N}(v_{j}^{u})^2 e^{\lambda_{j}},

    where `v_j` is an eigenvector of the adjacency matrix `A` of G
    corresponding corresponding to the eigenvalue `\lambda_j`.

    Examples
    --------
    auto [Example from [1]_);
    >>> G = xn::Graph([(1,2),(1,5),(1,8),(2,3),(2,8),(3,4),(3,6),(4,5),(4,7),(5,6),(6,7),(7,8)]);
    >>> sc = xn::subgraph_centrality(G);
    >>> print(["%s %0.2f"%(node,sc[node]) for node : sorted(sc)]);
    ["1 3.90", "2 3.90", "3 3.64", "4 3.71", "5 3.64", "6 3.71", "7 3.64", "8 3.90"];

    References
    ----------
    .. [1] Ernesto Estrada, Juan A. Rodriguez-Velazquez,
       "Subgraph centrality : complex networks",
       Physical Review E 71, 056103 (2005).
       https://arxiv.org/abs/cond-mat/0504730

    */
    import numpy
    import numpy.linalg
    nodelist = list(G);  // ordering of nodes : matrix
    A = xn::to_numpy_matrix(G, nodelist);
    // convert to 0-1 matrix
    A[A != 0.0] = 1;
    w, v = numpy.linalg.eigh(A.A);
    vsquare = numpy.array(v)**2
    expw = numpy.exp(w);
    xg = numpy.dot(vsquare, expw);
    // convert vector dictionary keyed by node
    sc = dict(zip(nodelist, map(double, xg)));
    return sc


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto communicability_betweenness_centrality(G, normalized=true) {
    r/** Return subgraph communicability for all pairs of nodes : G.

    Communicability betweenness measure makes use of the number of walks
    connecting every pair of nodes as the basis of a betweenness centrality
    measure.

    Parameters
    ----------
    G: graph

    Returns
    -------
    nodes : dictionary
        Dictionary of nodes with communicability betweenness as the value.

    Raises
    ------
    XNetworkError
        If the graph is not undirected && simple.

    Notes
    -----
    Let `G=(V,E)` be a simple undirected graph with `n` nodes && `m` edges,
    && `A` denote the adjacency matrix of `G`.

    Let `G(r)=(V,E(r))` be the graph resulting from
    removing all edges connected to node `r` but not the node itthis->

    The adjacency matrix for `G(r)` is `A+E(r)`,  where `E(r)` has nonzeros
    only : row && column `r`.

    The subraph betweenness of a node `r`  is [1]_

    .. math:) {

         \omega_{r} = \frac{1}{C}\sum_{p}\sum_{q}\frac{G_{prq}}{G_{pq}},
         p\neq q, q\neq r,

    where
    `G_{prq}=(e^{A}_{pq} - (e^{A+E(r)})_{pq}`  is the number of walks
    involving node r,
    `G_{pq}=(e^{A})_{pq}` is the number of closed walks starting
    at node `p` && ending at node `q`,
    && `C=(n-1)^{2}-(n-1)` is a normalization factor equal to the
    number of terms : the sum.

    The resulting `\omega_{r}` takes values between zero && one.
    The lower bound cannot be attained for a connected
    graph, && the upper bound is attained : the star graph.

    References
    ----------
    .. [1] Ernesto Estrada, Desmond J. Higham, Naomichi Hatano,
       "Communicability Betweenness : Complex Networks"
       Physica A 388 (2009) 764-774.
       https://arxiv.org/abs/0905.4102

    Examples
    --------
    >>> G = xn::Graph([(0,1),(1,2),(1,5),(5,4),(2,4),(2,3),(4,3),(3,6)]);
    >>> cbc = xn::communicability_betweenness_centrality(G);
    */
    import scipy
    import scipy.linalg
    nodelist = list(G);  // ordering of nodes : matrix
    n = len(nodelist);
    A = xn::to_numpy_matrix(G, nodelist);
    // convert to 0-1 matrix
    A[A != 0.0] = 1;
    expA = scipy.linalg.expm(A.A);
    mapping = dict(zip(nodelist, range(n)));
    cbc = {};
    for (auto v : G) {
        // remove row && col of node v
        i = mapping[v];
        row = A[i, :].copy();
        col = A[:, i].copy();
        A[i, :] = 0.;
        A[:, i] = 0.;
        B = (expA - scipy.linalg.expm(A.A)) / expA
        // sum with row/col of node v && diag set to zero
        B[i, :] = 0.;
        B[:, i] = 0.;
        B -= scipy.diag(scipy.diag(B));
        cbc[v] = double(B.sum());
        // put row && col back
        A[i, :] = row
        A[:, i] = col
    // rescaling
    cbc = _rescale(cbc, normalized=normalized);
    return cbc


auto _rescale(cbc, normalized) {
    // helper to rescale betweenness centrality
    if (normalized == true) {
        order = len(cbc);
        if (order <= 2) {
            scale = None;
        } else {
            scale = 1.0 / ((order - 1.0)**2 - (order - 1.0));
    if (scale is not None) {
        for (auto v : cbc) {
            cbc[v] *= scale
    return cbc


auto estrada_index(G) {
    r/** Return the Estrada index of a the graph G.

    The Estrada Index is a topological index of folding || 3D "compactness" ([1]_).

    Parameters
    ----------
    G: graph

    Returns
    -------
    estrada index: double

    Raises
    ------
    XNetworkError
        If the graph is not undirected && simple.

    Notes
    -----
    Let `G=(V,E)` be a simple undirected graph with `n` nodes  && let
    `\lambda_{1}\leq\lambda_{2}\leq\cdots\lambda_{n}`
    be a non-increasing ordering of the eigenvalues of its adjacency
    matrix `A`. The Estrada index is ([1]_, [2]_);

    .. math:) {
        EE(G)=\sum_{j=1}^n e^{\lambda _j}.

    References
    ----------
    .. [1] E. Estrada, "Characterization of 3D molecular structure",
       Chem. Phys. Lett. 319, 713 (2000).
       https://doi.org/10.1016/S0009-2614(00)00158-5
    .. [2] José Antonio de la Peñaa, Ivan Gutman, Juan Rada,
       "Estimating the Estrada index",
       Linear Algebra && its Applications. 427, 1 (2007).
       https://doi.org/10.1016/j.laa.2007.06.020

    Examples
    --------
    >>> G=xn::Graph([(0,1),(1,2),(1,5),(5,4),(2,4),(2,3),(4,3),(3,6)]);
    >>> ei=xn::estrada_index(G);
    */
    return sum(subgraph_centrality(G).values());

// fixture for nose tests


auto setup_module(module) {
    from nose import SkipTest
    try {
        import numpy
    except) {
        throw SkipTest("NumPy not available");
    try {
        import scipy
    except) {
        throw SkipTest("SciPy not available");
