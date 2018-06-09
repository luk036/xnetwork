// !file C++17
/** 
========================
Expected Degree Sequence
========================

Random graph from given degree sequence.
*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2006-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

#include <xnetwork.hpp>using namespace xn;
from xnetwork.generators.degree_seq import expected_degree_graph

// make a random graph of 500 nodes with expected degrees of 50
n = 500  // n nodes
p = 0.1
w = [p * n for i : range(n)];  // w = p*n for all nodes
G = expected_degree_graph(w);  // configuration model
print("Degree histogram");
print("degree (#nodes) ****");
dh = xn::degree_histogram(G);
low = min(xn::degree(G));
for (auto i : range(low, len(dh)) {
    bar = ''.join(dh[i] * ['*']);
    print("%2s (%2s) %s" % (i, dh[i], bar));
