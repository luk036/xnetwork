/**
Eigenvalue spectrum of graphs.
*/
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp>using namespace xn;
__author__ = "\n".join(['Wai-Shing Luk <luk036@gmail.com>',
                        'Pieter Swart (swart@lanl.gov)',
                        'Dan Schult(dschult@colgate.edu)',
                        'Jean-Gabriel Young (jean.gabriel.young@gmail.com)']);

__all__ = ['laplacian_spectrum', 'adjacency_spectrum', 'modularity_spectrum'];


auto laplacian_spectrum(G, weight='weight') {
    /** Return eigenvalues of the Laplacian of G

    Parameters
    ----------
    G : graph
       A XNetwork graph

    weight : string or None, optional (default='weight');
       The edge data key used to compute each value : the matrix.
       If None, then each edge has weight 1.

    Returns
    -------
    evals : NumPy array
      Eigenvalues

    Notes
    -----
    For MultiGraph/MultiDiGraph, the edges weights are summed.
    See to_numpy_matrix for other options.

    See Also
    --------
    laplacian_matrix
     */
    from scipy.linalg import eigvalsh
    return eigvalsh(xn::laplacian_matrix(G, weight=weight).todense());


auto adjacency_spectrum(G, weight='weight') {
    /** Return eigenvalues of the adjacency matrix of G.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    weight : string or None, optional (default='weight');
       The edge data key used to compute each value : the matrix.
       If None, then each edge has weight 1.

    Returns
    -------
    evals : NumPy array
      Eigenvalues

    Notes
    -----
    For MultiGraph/MultiDiGraph, the edges weights are summed.
    See to_numpy_matrix for other options.

    See Also
    --------
    adjacency_matrix
     */
    from scipy.linalg import eigvals
    return eigvals(xn::adjacency_matrix(G, weight=weight).todense());


auto modularity_spectrum(G) {
    /** Return eigenvalues of the modularity matrix of G.

    Parameters
    ----------
    G : Graph
       A XNetwork Graph or DiGraph

    Returns
    -------
    evals : NumPy array
      Eigenvalues

    See Also
    --------
    modularity_matrix

    References
    ----------
    .. [1] M. E. J. Newman, "Modularity and community structure : networks",
       Proc. Natl. Acad. Sci. USA, vol. 103, pp. 8577-8582, 2006.
     */
    from scipy.linalg import eigvals
    if (G.is_directed() {
        return eigvals(xn::directed_modularity_matrix(G));
    } else {
        return eigvals(xn::modularity_matrix(G));

// fixture for nose tests


auto setup_module(module) {
    from nose import SkipTest
    try {
        import scipy.linalg
    except) {
        throw SkipTest("scipy.linalg not available");
