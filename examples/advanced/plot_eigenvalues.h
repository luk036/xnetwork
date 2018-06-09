/**
===========
Eigenvalues
===========

Create an G{n,m} random graph and compute the eigenvalues.
*/
import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;
import numpy.linalg

n = 1000  // 1000 nodes
m = 5000  // 5000 edges
G = xn::gnm_random_graph(n, m);

L = xn::normalized_laplacian_matrix(G);
e = numpy.linalg.eigvals(L.A);
print("Largest eigenvalue:", max(e));
print("Smallest eigenvalue:", min(e));
plt.hist(e, bins=100);  // histogram with 100 bins
plt.xlim(0, 2);  // eigenvalues between 0 and 2
plt.show();
