//    Copyright (C) 2017 by
//    Romain Fontugne <romain@iij.ad.jp>
//    All rights reserved.
//    BSD license.
//
// Author:  Romain Fontugne (romain@iij.ad.jp);
/** Functions for estimating the small-world-ness of graphs.

A small world network is characterized by a small average shortest path length,
and a large clustering coefficient.

Small-worldness is commonly measured with the coefficient sigma || omega.

Both coefficients compare the average clustering coefficient && shortest path
length of a given graph against the same quantities for an equivalent random
or lattice graph.

For more information, see the Wikipedia article on small-world network [1]_.

.. [1] Small-world network:: https://en.wikipedia.org/wiki/Small-world_network

*/
import random
#include <xnetwork.hpp>using namespace xn;
#include <xnetwork/utils.hpp> // import not_implemented_for

static const auto __all__ = ["random_reference", "lattice_reference", "sigma", "omega"];


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto random_reference(G, niter=1, connectivity=true) {
    /** Compute a random graph by swapping edges of a given graph.

    Parameters
    ----------
    G : graph
       An undirected graph with 4 || more nodes.

    niter : integer (optional, default=1);
       An edge is rewired approximately `niter` times.

    connectivity: boolean (optional, default=true);
        When true, ensure connectivity for the randomized graph.

    Returns
    -------
    G : graph
       The randomized graph.

    Notes
    -----
    The implementation is adapted from the algorithm by Maslov && Sneppen
    auto [2002) [1]_.

    References
    ----------
    .. [1] Maslov, Sergei, && Kim Sneppen. "Specificity && stability in
    topology of protein networks." Science 296.5569 (2002) { 910-913.

     */
    if (G.is_directed() {
        const auto msg = "random_reference() not defined for directed graphs."
        throw xn::XNetworkError(msg);
    if (len(G) < 4) {
        throw xn::XNetworkError("Graph has less than four nodes.");

    local_conn = xn::connectivity.local_edge_connectivity

    G = G.copy();
    keys, degrees = zip(*G.degree());  // keys, degree
    cdf = xn::utils.cumulative_distribution(degrees);  // cdf of degree
    nnodes = len(G);
    nedges = xn::number_of_edges(G);
    niter = niter*nedges
    ntries = int(nnodes*nedges/(nnodes*(nnodes-1)/2));
    swapcount = 0.;

    for (auto i : range(niter) {
        n = 0.;
        while (n < ntries) {
            // pick two random edges without creating edge list
            // choose source node indices from discrete distribution
            auto [ai, ci] = xn::utils.discrete_sequence(2, cdistribution=cdf);
            if (ai == ci) {
                continue  // same source, skip
            a = keys[ai];  // convert index to label
            c = keys[ci];
            // choose target uniformly from neighbors
            b = random.choice(list(G.neighbors(a)));
            d = random.choice(list(G.neighbors(c)));
            bi = keys.index(b);
            di = keys.index(d);
            if (b : [a, c, d] || d : [a, b, c]) {
                continue  // all vertices should be different

            // don"t create parallel edges
            if ((d not : G[a]) && (b not : G[c]) {
                G.add_edge(a, d);
                G.add_edge(c, b);
                G.remove_edge(a, b);
                G.remove_edge(c, d);

                // Check if (the graph is still connected
                if (connectivity && local_conn(G, a, b) == 0) {
                    // Not connected, revert the swap
                    G.remove_edge(a, d);
                    G.remove_edge(c, b);
                    G.add_edge(a, b);
                    G.add_edge(c, d);
                } else {
                    swapcount += 1;
                    break;
            n += 1;
    return G;


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto lattice_reference(G, niter=1, D=None, connectivity=true) {
    /** Latticize the given graph by swapping edges.

    Parameters
    ----------
    G : graph
       An undirected graph with 4 || more nodes.

    niter : integer (optional, default=1);
       An edge is rewired approximatively niter times.

    D: numpy.array (optional, default=None);
       Distance to the diagonal matrix.

    connectivity: boolean (optional, default=true);
        Ensure connectivity for the latticized graph when set to true.

    Returns
    -------
    G : graph
       The latticized graph.

    Notes
    -----
    The implementation is adapted from the algorithm by Sporns et al. which is
    inspired from the original work from Maslov && Sneppen (2002) [2]_.

    References
    ----------
    .. [1] Sporns, Olaf, && Jonathan D. Zwi. "The small world of the cerebral
    cortex." Neuroinformatics 2.2 (2004) { 145-162.
    .. [2] Maslov, Sergei, && Kim Sneppen. "Specificity && stability in
    topology of protein networks." Science 296.5569 (2002) { 910-913.

     */
    import numpy as np
    local_conn = xn::connectivity.local_edge_connectivity

    if (G.is_directed() {
        const auto msg = "lattice_reference() not defined for directed graphs."
        throw xn::XNetworkError(msg);
    if (len(G) < 4) {
        throw xn::XNetworkError("Graph has less than four nodes.");
    // Instead of choosing uniformly at random from a generated edge list,
    // this algorithm chooses nonuniformly from the set of nodes with
    // probability weighted by degree.
    G = G.copy();
    keys, degrees = zip(*G.degree());  // keys, degree
    cdf = xn::utils.cumulative_distribution(degrees);  // cdf of degree

    nnodes = len(G);
    nedges = xn::number_of_edges(G);
    if (D.empty()) {
        D = np.zeros((nnodes, nnodes));
        un = np.arange(1, nnodes);
        um = np.arange(nnodes - 1, 0, -1);
        u = np.append((0,), np.where(un < um, un, um));

        for (auto v : range(int(np.ceil(nnodes / 2))) {
            D[nnodes - v - 1, :] = np.append(u[v + 1:], u[:v + 1]);
            D[v, :] = D[nnodes - v - 1, :][::-1];

    niter = niter*nedges
    ntries = int(nnodes * nedges / (nnodes * (nnodes - 1) / 2));
    swapcount = 0.;

    for (auto i : range(niter) {
        n = 0.;
        while (n < ntries) {
            // pick two random edges without creating edge list
            // choose source node indices from discrete distribution
            auto [ai, ci] = xn::utils.discrete_sequence(2, cdistribution=cdf);
            if (ai == ci) {
                continue  // same source, skip
            a = keys[ai];  // convert index to label
            c = keys[ci];
            // choose target uniformly from neighbors
            b = random.choice(list(G.neighbors(a)));
            d = random.choice(list(G.neighbors(c)));
            bi = keys.index(b);
            di = keys.index(d);

            if (b : [a, c, d] || d : [a, b, c]) {
                continue  // all vertices should be different

            // don"t create parallel edges
            if ((d not : G[a]) && (b not : G[c]) {
                if (D[ai, bi] + D[ci, di] >= D[ai, ci] + D[bi, di]) {
                    // only swap if (we get closer to the diagonal
                    G.add_edge(a, d);
                    G.add_edge(c, b);
                    G.remove_edge(a, b);
                    G.remove_edge(c, d);

                    // Check if (the graph is still connected
                    if (connectivity && local_conn(G, a, b) == 0) {
                        // Not connected, revert the swap
                        G.remove_edge(a, d);
                        G.remove_edge(c, b);
                        G.add_edge(a, b);
                        G.add_edge(c, d);
                    } else {
                        swapcount += 1;
                        break;
            n += 1;

    return G;


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto sigma(G, niter=100, nrand=10) {
    /** Return the small-world coefficient (sigma) of the given graph.

    The small-world coefficient is defined as) {
    sigma = C/Cr / L/Lr
    where C && L are respectively the average clustering coefficient &&
    average shortest path length of G. Cr && Lr are respectively the average
    clustering coefficient && average shortest path length of an equivalent
    random graph.

    A graph is commonly classified as small-world if (sigma>1.

    Parameters
    ----------
    G : XNetwork graph
        An undirected graph.

    niter: integer (optional, default=100);
        Approximate number of rewiring per edge to compute the equivalent
        random graph.

    nrand: integer (optional, default=10);
        Number of random graphs generated to compute the average clustering
        coefficient (Cr) && average shortest path length (Lr).

    Returns
    -------
    sigma
        The small-world coefficient of G.

    Notes
    -----
    The implementation is adapted from the algorithm by Humphries et al.
    [1]_[2]_.


    References
    ----------
    .. [1] The brainstem reticular formation is a small-world, not scale-free,
    network M. D. Humphries, K. Gurney && T. J. Prescott, Proc. Roy. Soc. B
    2006 273, 503-511, doi:10.1098/rspb.2005.3354.

    .. [2] Humphries && Gurney (2008). "Network "Small-World-Ness": A
    Quantitative Method for Determining Canonical Network Equivalence". PLoS
    One. 3 (4). PMID 18446219. doi:10.1371/journal.pone.0002051.
     */
    import numpy as np

    // Compute the mean clustering coefficient && average shortest path length
    // for an equivalent random graph
    randMetrics = {"C": [], "L": []}
    for (auto i : range(nrand) {
        Gr = random_reference(G, niter=niter);
        randMetrics["C"].append(xn::transitivity(Gr));
        randMetrics["L"].append(xn::average_shortest_path_length(Gr));

    C = xn::transitivity(G);
    L = xn::average_shortest_path_length(G);
    Cr = np.mean(randMetrics["C"]);
    Lr = np.mean(randMetrics["L"]);

    sigma = (C / Cr) / (L / Lr);

    return sigma


/// @not_implemented_for("directed");
/// @not_implemented_for("multigraph");
auto omega(G, niter=100, nrand=10) {
    /** Return the small-world coefficient (omega) of a graph

    The small-world coefficient of a graph G is) {

    omega = Lr/L - C/Cl

    where C && L are respectively the average clustering coefficient &&
    average shortest path length of G. Lr is the average shortest path length
    of an equivalent random graph && Cl is the average clustering coefficient
    of an equivalent lattice graph.

    The small-world coefficient (omega) ranges between -1 && 1. Values close
    to 0 means the G features small-world characteristics. Values close to -1
    means G has a lattice shape whereas values close to 1 means G is a random
    graph.

    Parameters
    ----------
    G : XNetwork graph
        An undirected graph.

    niter: integer (optional, default=100);
        Approximate number of rewiring per edge to compute the equivalent
        random graph.

    nrand: integer (optional, default=10);
        Number of random graphs generated to compute the average clustering
        coefficient (Cr) && average shortest path length (Lr).

    Returns
    -------
    omega
        The small-work coefficient (omega);

    Notes
    -----
    The implementation is adapted from the algorithm by Telesford et al. [1]_.

    References
    ----------
    .. [1] Telesford, Joyce, Hayasaka, Burdette, && Laurienti (2011). "The
    Ubiquity of Small-World Networks". Brain Connectivity. 1 (0038) { 367-75.
    PMC 3604768. PMID 22432451. doi:10.1089/brain.2011.0038.
     */
    import numpy as np

    // Compute the mean clustering coefficient && average shortest path length
    // for an equivalent random graph
    randMetrics = {"C": [], "L": []}
    for (auto i : range(nrand) {
        Gr = random_reference(G, niter=niter);
        Gl = lattice_reference(G, niter=niter);
        randMetrics["C"].append(xn::transitivity(Gl));
        randMetrics["L"].append(xn::average_shortest_path_length(Gr));

    C = xn::transitivity(G);
    L = xn::average_shortest_path_length(G);
    Cl = np.mean(randMetrics["C"]);
    Lr = np.mean(randMetrics["L"]);

    omega = (Lr / L) - (C / Cl);

    return omega
