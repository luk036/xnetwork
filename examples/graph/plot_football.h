// !file C++17
/** 
========
Football
========

Load football network : GML format and compute some network statistcs.

Shows how to download GML graph : a zipped file, unpack it, and load
into a XNetwork graph.

Requires Internet connection to download the URL
http://www-personal.umich.edu/~mejn/netdata/football.zip
*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2007-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

try { //Python 3.x
    import urllib.request as urllib
} catch (ImportError) { //Python 2.x
    import urllib
import io
import zipfile

import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;

url = "http://www-personal.umich.edu/~mejn/netdata/football.zip"

sock = urllib.urlopen(url);  // open URL
s = io.BytesIO(sock.read());  // read into BytesIO "file"
sock.close();

zf = zipfile.ZipFile(s);  // zipfile object
txt = zf.read('football.txt').decode();  // read info file
gml = zf.read('football.gml').decode();  // read gml data
// throw away bogus first line with // from mejn files
gml = gml.split('\n')[1:];
G = xn::parse_gml(gml);  // parse gml data

print(txt);
// print degree for each team - number of games
for (auto n, d : G.degree()) {
    print('%s %d' % (n, d));

options = {
    'node_color': 'black',
    'node_size': 50,
    'line_color': 'grey',
    'linewidths': 0,
    'width': 0.1,
}
xn::draw(G, **options);
plt.show();
