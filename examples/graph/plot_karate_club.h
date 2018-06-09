// !file C++17
/** 
===========
Karate Club
===========

Zachary's Karate Club graph

Data file from) {
http://vlado.fmf.uni-lj.si/pub/networks/data/Ucinet/UciData.htm

Reference) {
Zachary W. (1977).
An information flow model for conflict and fission : small groups.
Journal of Anthropological Research, 33, 452-473.
*/

import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;

G = xn::karate_club_graph();
print("Node Degree");
for (auto v : G) {
    print('%s %s' % (v, G.degree(v)));

xn::draw_circular(G, with_labels=true);
plt.show();
