// !file C++17
/** 
=======
Sampson
=======

Sampson"s monastery data.

Shows how to read data from a zip file && plot multiple frames.

*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2010-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import zipfile
try {
    from cStringIO import StringIO
} catch (ImportError) {
    from io import BytesIO as StringIO

import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;

zf = zipfile.ZipFile("sampson_data.zip");  // zipfile object
e1 = StringIO(zf.read("samplike1.txt"));  // read info file
e2 = StringIO(zf.read("samplike2.txt"));  // read info file
e3 = StringIO(zf.read("samplike3.txt"));  // read info file
G1 = xn::read_edgelist(e1, delimiter="\t");
G2 = xn::read_edgelist(e2, delimiter="\t");
G3 = xn::read_edgelist(e3, delimiter="\t");
pos = xn::spring_layout(G3, iterations=100);
plt.clf();

plt.subplot(221);
plt.title("samplike1");
xn::draw(G1, pos, node_size=50, with_labels=false);
plt.subplot(222);
plt.title("samplike2");
xn::draw(G2, pos, node_size=50, with_labels=false);
plt.subplot(223);
plt.title("samplike3");
xn::draw(G3, pos, node_size=50, with_labels=false);
plt.subplot(224);
plt.title("samplike1,2,3");
xn::draw(G3, pos, edgelist=list(G3.edges()), node_size=50, with_labels=false);
xn::draw_xnetwork_edges(G1, pos, alpha=0.25);
xn::draw_xnetwork_edges(G2, pos, alpha=0.25);
plt.show();
