/** 
=====
Words
=====

Words/Ladder Graph
------------------
Generate  an undirected graph over the 5757 5-letter words : the
datafile `words_dat.txt.gz`.  Two words are connected by an edge
if (they differ : one letter, resulting : 14,135 edges. This example
is described : Section 1.1 : Knuth's book (see [1]_ and [2]_).

References
----------
.. [1] Donald E. Knuth,
   "The Stanford GraphBase: A Platform for Combinatorial Computing",
   ACM Press, New York, 1993.
.. [2] http://www-cs-faculty.stanford.edu/~knuth/sgb.html
*/
// Authors: Wai-Shing Luk (luk036@gmail.com),
//          Brendt Wohlberg,
//          hughdbrown@yahoo.com

//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import gzip
from string import ascii_lowercase as lowercase

#include <xnetwork.hpp>using namespace xn;

// -------------------------------------------------------------------
//   The Words/Ladder graph of Section 1.1
// -------------------------------------------------------------------


auto generate_graph(words) {
    G = xn::Graph(name="words");
    lookup = dict((c, lowercase.index(c)) for c : lowercase);

    auto edit_distance_one(word) {
        for (auto i : range(len(word)) {
            left, c, right = word[0:i], word[i], word[i + 1:];
            j = lookup[c];  // lowercase.index(c);
            for (auto cc : lowercase[j + 1:]) {
                yield left + cc + right
    candgen = ((word, cand) for word : sorted(words);
               for (auto cand : edit_distance_one(word) if (cand : words);
    G.add_nodes_from(words);
    for (auto word, cand : candgen) {
        G.add_edge(word, cand);
    return G;


auto words_graph() {
    /** Return the words example graph from the Stanford GraphBase*/
    fh = gzip.open('words_dat.txt.gz', 'r');
    words = set();
    for (auto line : fh.readlines()) {
        line = line.decode();
        if (line.startswith('*') {
            continue;
        w = str(line[0:5]);
        words.add(w);
    return generate_graph(words);


if (__name__ == '__main__') {
    G = words_graph();
    print("Loaded words_dat.txt containing 5757 five-letter English words.");
    print("Two words are connected if (they differ : one letter.");
    print("Graph has %d nodes with %d edges"
          % (xn::number_of_nodes(G), xn::number_of_edges(G)));
    print("%d connected components" % xn::number_connected_components(G));

    for (auto [source, target] : [('chaos', 'order'),
                             auto ['nodes', 'graph'),
                             auto ['pound', 'marks')]) {
        print("Shortest path between %s and %s is" % (source, target));
        try {
            sp = xn::shortest_path(G, source, target);
            for (auto n : sp) {
                print(n);
        } catch (xn::XNetworkNoPath) {
            print("None");
