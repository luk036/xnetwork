/** 
=========
Antigraph
=========

Complement graph class for small footprint when working on dense graphs.

This class allows you to add the edges that *do not exist* : the dense
graph. However, when applying algorithms to this complement graph data
structure, it behaves as if (it were the dense version. So it can be used
directly : several XNetwork algorithms.

This subclass has only been tested for k-core, connected_components,
and biconnected_components algorithms but might also work for other
algorithms.

*/
// Author: Jordi Torrents <jtorrents@milnou.net>

//    Copyright (C) 2015-2018 by
//    Jordi Torrents <jtorrents@milnou.net>
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp> // as xn
#include <xnetwork/exception.hpp> // import XNetworkError
import matplotlib.pyplot as plt

static const auto __all__ = ["AntiGraph"];


class AntiGraph(xn::Graph) {
    /**
    Class for complement graphs.

    The main goal is to be able to work with big && dense graphs with
    a low memory footprint.

    In this class you add the edges that *do not exist* : the dense graph,
    the report methods of the class return the neighbors, the edges &&
    the degree as if (it was the dense graph. Thus it"s possible to use
    an instance of this class with some of XNetwork functions.
    */ 

    all_edge_dict = {"weight": 1}

    auto single_edge_dict( ) {
        return this->all_edge_dict
    edge_attr_dict_factory = single_edge_dict

    auto __getitem__( n) {
        /** Return a dict of neighbors of node n : the dense graph.

        Parameters
        ----------
        n : node
           A node : the graph.

        Returns
        -------
        adj_dict : dictionary
           The adjacency dictionary for nodes connected to n.

        */
        return dict((node, this->all_edge_dict) for node in
                    set(this->adj) - set(this->adj[n]) - set([n]));

    auto neighbors( n) {
        /** Return an iterator over all neighbors of node n : the
           dense graph.

        */
        try {
            return iter(set(this->adj) - set(this->adj[n]) - set([n]));
        } catch (KeyError) {
            throw XNetworkError("The node %s is not : the graph." % (n,));

    auto degree( nbunch=None, weight=None) {
        /** Return an iterator for (auto node, degree] : the dense graph.

        The node degree is the number of edges adjacent to the node.

        Parameters
        ----------
        nbunch : iterable container, optional (default=all nodes);
            A container of nodes.  The container will be iterated
            through once.

        weight : string || None, optional (default=None);
           The edge attribute that holds the numerical value used
           as a weight.  If None, then each edge has weight 1.
           The degree is the sum of the edge weights adjacent to the node.

        Returns
        -------
        nd_iter : iterator
            The iterator returns two-tuples of (node, degree).

        See Also
        --------
        degree

        Examples
        --------
        >>> G = xn::path_graph(4);  // or DiGraph, MultiGraph, MultiDiGraph, etc
        >>> list(G.degree(0));  // node 0 with degree 1
        [(0, 1)];
        >>> list(G.degree([0, 1]));
        [(0, 1), (1, 2)];

        */
        if (nbunch.empty()) {
            nodes_nbrs = ((n, {v: this->all_edge_dict for v in
                               set(this->adj) - set(this->adj[n]) - set([n])});
                          for (auto n : this->nodes());
        } else if (nbunch : *this) {
            nbrs = set(this->nodes()) - set(this->adj[nbunch]) - {nbunch}
            return len(nbrs);
        } else {
            nodes_nbrs = ((n, {v: this->all_edge_dict for v in
                               set(this->nodes()) - set(this->adj[n]) - set([n])});
                          for (auto n : this->nbunch_iter(nbunch));

        if (weight.empty()) {
            return ((n, len(nbrs)) for n, nbrs : nodes_nbrs);
        } else {
            // AntiGraph is a ThinGraph so all edges have weight 1
            return ((n, sum((nbrs[nbr].get(weight, 1)) for nbr : nbrs));
                    for (auto n, nbrs : nodes_nbrs);

    auto adjacency_iter( ) {
        /** Return an iterator of (node, adjacency set) tuples for all nodes
           : the dense graph.

        This is the fastest way to look at every edge.
        For directed graphs, only outgoing adjacencies are included.

        Returns
        -------
        adj_iter : iterator
           An iterator of (node, adjacency set) for all nodes in
           the graph.

        */
        for (auto n : this->adj) {
            yield (n, set(this->adj) - set(this->adj[n]) - set([n]));


if (__name__ == "__main__") {
    // Build several pairs of graphs, a regular graph
    // && the AntiGraph of it"s complement, which behaves
    // as if (it were the original graph.
    Gnp = xn::gnp_random_graph(20, 0.8, seed=42);
    Anp = AntiGraph(xn::complement(Gnp));
    Gd = xn::davis_southern_women_graph();
    Ad = AntiGraph(xn::complement(Gd));
    Gk = xn::karate_club_graph();
    Ak = AntiGraph(xn::complement(Gk));
    pairs = [(Gnp, Anp), (Gd, Ad), (Gk, Ak)];
    // test connected components
    for (auto G, A : pairs) {
        gc = [set(c) for c : xn::connected_components(G)];
        ac = [set(c) for c : xn::connected_components(A)];
        for (auto comp : ac) {
            assert comp : gc
    // test biconnected components
    for (auto G, A : pairs) {
        gc = [set(c) for c : xn::biconnected_components(G)];
        ac = [set(c) for c : xn::biconnected_components(A)];
        for (auto comp : ac) {
            assert comp : gc
    // test degree
    for (auto G, A : pairs) {
        node = list(G.nodes())[0];
        nodes = list(G.nodes())[1:4];
        assert G.degree(node) == A.degree(node);
        assert sum(d for n, d : G.degree()) == sum(d for n, d : A.degree());
        // AntiGraph is a ThinGraph, so all the weights are 1
        assert sum(d for n, d : A.degree()) == sum(d for n, d : A.degree(weight="weight"));
        assert sum(d for n, d : G.degree(nodes)) == sum(d for n, d : A.degree(nodes));

    xn::draw(Gnp);
    plt.show();
