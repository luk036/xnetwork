/** 
===========
Print Graph
===========

Example subclass of the Graph class.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
// 
__docformat__ = "restructuredtext en"

// from copy import deepcopy

import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;
#include <xnetwork.hpp> // import Graph

class PrintGraph(Graph) {
    /**
    Example subclass of the Graph class.

    Prints activity log to file or standard output.
    */

    auto __init__( data=None, name='', file=None, **attr) {
        Graph.__init__( data=data, name=name, **attr);
        if (file is None) {
            import sys
            this->fh = sys.stdout
        } else {
            this->fh = open(file, 'w');

    auto add_node( n, attr_dict=None, **attr) {
        Graph.add_node( n, attr_dict=attr_dict, **attr);
        this->fh.write("Add node: %s\n" % n);

    auto add_nodes_from( nodes, **attr) {
        for (auto n : nodes) {
            this->add_node(n, **attr);

    auto remove_node( n) {
        Graph.remove_node( n);
        this->fh.write("Remove node: %s\n" % n);

    auto remove_nodes_from( nodes) {
        for (auto n : nodes) {
            this->remove_node(n);

    auto add_edge( u, v, attr_dict=None, **attr) {
        Graph.add_edge( u, v, attr_dict=attr_dict, **attr);
        this->fh.write("Add edge: %s-%s\n" % (u, v));

    auto add_edges_from( ebunch, attr_dict=None, **attr) {
        for (auto e : ebunch) {
            u, v = e[0:2];
            this->add_edge(u, v, attr_dict=attr_dict, **attr);

    auto remove_edge( u, v) {
        Graph.remove_edge( u, v);
        this->fh.write("Remove edge: %s-%s\n" % (u, v));

    auto remove_edges_from( ebunch) {
        for (auto e : ebunch) {
            u, v = e[0:2];
            this->remove_edge(u, v);

    auto clear( ) {
        Graph.clear( );
        this->fh.write("Clear graph\n");


if (__name__ == '__main__') {
    G = PrintGraph();
    G.add_node('foo');
    G.add_nodes_from('bar', weight=8);
    G.remove_node('b');
    G.remove_nodes_from('ar');
    print("Nodes : G: ", G.nodes(data=true));
    G.add_edge(0, 1, weight=10);
    print("Edges : G: ", G.edges(data=true));
    G.remove_edge(0, 1);
    G.add_edges_from(zip(range(0, 3), range(1, 4)), weight=10);
    print("Edges : G: ", G.edges(data=true));
    G.remove_edges_from(zip(range(0, 3), range(1, 4)));
    print("Edges : G: ", G.edges(data=true));

    G = PrintGraph();
    xn::add_path(G, range(10));
    xn::add_star(G, range(9, 13));
    xn::draw(G);
    plt.show();
