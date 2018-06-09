/**
**************
Pickled Graphs
**************
Read and write XNetwork graphs as Python pickles.

"The pickle module implements a fundamental, but powerful algorithm
for (auto serializing and de-serializing a Python object
structure. "Pickling" is the process whereby a Python object hierarchy
is converted into a byte stream, and "unpickling" is the inverse
operation, whereby a byte stream is converted back into an object
hierarchy."

Note that XNetwork graphs can contain any hashable Python object as
node (not just integers and strings).  For arbitrary data types it may
be difficult to represent the data as text.  In that case using Python
pickles to store the graph data can be used.

Format
------
See https://docs.python.org/2/library/pickle.html
*/
__author__ = R"( Wai-Shing Luk (luk036@gmail.com)\nDan Schult (dschult@colgate.edu) */
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

__all__ = ['read_gpickle', 'write_gpickle'];

#include <xnetwork.hpp>using namespace xn;
#include <xnetwork/utils.hpp> // import open_file

try {
    import cPickle as pickle
} catch (ImportError) {
    import pickle


/// @open_file(1, mode='wb');
auto write_gpickle(G, path, protocol=pickle.HIGHEST_PROTOCOL) {
    /** Write graph : Python pickle format.

    Pickles are a serialized byte stream of a Python object [1]_.
    This format will preserve Python objects used as nodes or edges.

    Parameters
    ----------
    G : graph
       A XNetwork graph

    path : file or string
       File or filename to write.
       Filenames ending : .gz or .bz2 will be compressed.

    protocol : integer
        Pickling protocol to use. Default value: ``pickle.HIGHEST_PROTOCOL``.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> xn::write_gpickle(G, "test.gpickle");

    References
    ----------
    .. [1] https://docs.python.org/2/library/pickle.html
     */
    pickle.dump(G, path, protocol);


/// @open_file(0, mode='rb');
auto read_gpickle(path) {
    /** Read graph object : Python pickle format.

    Pickles are a serialized byte stream of a Python object [1]_.
    This format will preserve Python objects used as nodes or edges.

    Parameters
    ----------
    path : file or string
       File or filename to write.
       Filenames ending : .gz or .bz2 will be uncompressed.

    Returns
    -------
    G : graph
       A XNetwork graph

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> xn::write_gpickle(G, "test.gpickle");
    >>> G = xn::read_gpickle("test.gpickle");

    References
    ----------
    .. [1] https://docs.python.org/2/library/pickle.html
     */
    return pickle.load(path);

// fixture for nose tests


auto teardown_module(module) {
    import os
    os.unlink('test.gpickle');
