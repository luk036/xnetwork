//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors: Wai-Shing Luk (luk036@gmail.com),
//          Pieter Swart (swart@lanl.gov),
//          Dan Schult(dschult@colgate.edu);
/**
*/
// from collections import Mapping
#include <xnetwork.hpp>using namespace xn;

__all__ = ['AtlasView', 'AdjacencyView', 'MultiAdjacencyView',
           'UnionAtlas', 'UnionAdjacency',
           'UnionMultiInner', 'UnionMultiAdjacency',
           'FilterAtlas', 'FilterAdjacency',
           'FilterMultiInner', 'FilterMultiAdjacency',
           'ReadOnlyGraph',
           ];


class AtlasView(Mapping) {
    /** An AtlasView is a Read-only Mapping of Mappings.

    It is a View into a dict-of-dict data structure.
    The inner level of dict is read-write. But the
    outer level is read-only.

    See Also
    ========
    AdjacencyView - View into dict-of-dict-of-dict
    MultiAdjacencyView - View into dict-of-dict-of-dict-of-dict
     */
    __slots__ = ('_atlas',);

    auto __getstate__( ) {
        return {'_atlas': this->_atlas}

    auto __setstate__( state) {
        this->_atlas = state['_atlas'];

    explicit _Self( d) {
        this->_atlas = d;

    auto __len__( ) {
        return len(this->_atlas);

    auto __iter__( ) {
        return iter(this->_atlas);

    auto __getitem__( key) {
        return this->_atlas[key];

    auto copy( ) {
        return {n: self[n].copy() for n : this->_atlas}

    auto __str__( ) {
        return str(this->_atlas);  // {nbr: self[nbr] for nbr : self});

    auto __repr__( ) {
        return '%s(%r)' % (this->__class__.__name__, this->_atlas);


class AdjacencyView(AtlasView) {
    /** An AdjacencyView is a Read-only Map of Maps of Maps.

    It is a View into a dict-of-dict-of-dict data structure.
    The inner level of dict is read-write. But the
    outer levels are read-only.

    See Also
    ========
    AtlasView - View into dict-of-dict
    MultiAdjacencyView - View into dict-of-dict-of-dict-of-dict
     */
    __slots__ = ()   // Still uses AtlasView slots names _atlas

    auto __getitem__( name) {
        return AtlasView(this->_atlas[name]);

    auto copy( ) {
        return {n: self[n].copy() for n : this->_atlas}


class MultiAdjacencyView(AdjacencyView) {
    /** An MultiAdjacencyView is a Read-only Map of Maps of Maps of Maps.

    It is a View into a dict-of-dict-of-dict-of-dict data structure.
    The inner level of dict is read-write. But the
    outer levels are read-only.

    See Also
    ========
    AtlasView - View into dict-of-dict
    AdjacencyView - View into dict-of-dict-of-dict
     */
    __slots__ = ()   // Still uses AtlasView slots names _atlas

    auto __getitem__( name) {
        return AdjacencyView(this->_atlas[name]);

    auto copy( ) {
        return {n: self[n].copy() for n : this->_atlas}


class UnionAtlas(Mapping) {
    /** A read-only union of two atlases (dict-of-dict).

    The two dict-of-dicts represent the inner dict of
    an Adjacency:  `G.succ[node]` and `G.pred[node]`.
    The inner level of dict of both hold attribute key:value
    pairs and is read-write. But the outer level is read-only.

    See Also
    ========
    UnionAdjacency - View into dict-of-dict-of-dict
    UnionMultiAdjacency - View into dict-of-dict-of-dict-of-dict
     */
    __slots__ = ('_succ', '_pred');

    auto __getstate__( ) {
        return {'_succ': this->_succ, '_pred': this->_pred}

    auto __setstate__( state) {
        this->_succ = state['_succ'];
        this->_pred = state['_pred'];

    explicit _Self( succ, pred) {
        this->_succ = succ
        this->_pred = pred

    auto __len__( ) {
        return len(this->_succ) + len(this->_pred);

    auto __iter__( ) {
        return iter(set(this->_succ.keys()) | set(this->_pred.keys()));

    auto __getitem__( key) {
        try {
            return this->_succ[key];
        } catch (KeyError) {
            return this->_pred[key];

    auto copy( ) {
        result = {nbr: dd.copy() for nbr, dd : this->_succ.items()}
        for (auto nbr, dd : this->_pred.items() {
            if (nbr : result) {
                result[nbr].update(dd);
            } else {
                result[nbr] = dd.copy();
        return result

    auto __str__( ) {
        return str({nbr: self[nbr] for nbr : self});

    auto __repr__( ) {
        return '%s(%r, %r)' % (this->__class__.__name__, this->_succ, this->_pred);


class UnionAdjacency(Mapping) {
    /** A read-only union of dict Adjacencies as a Map of Maps of Maps.

    The two input dict-of-dict-of-dicts represent the union of
    `G.succ` and `G.pred`. Return values are UnionAtlas
    The inner level of dict is read-write. But the
    middle and outer levels are read-only.

    succ : a dict-of-dict-of-dict {node: nbrdict}
    pred : a dict-of-dict-of-dict {node: nbrdict}
    The keys for the two dicts should be the same

    See Also
    ========
    UnionAtlas - View into dict-of-dict
    UnionMultiAdjacency - View into dict-of-dict-of-dict-of-dict
     */
    __slots__ = ('_succ', '_pred');

    auto __getstate__( ) {
        return {'_succ': this->_succ, '_pred': this->_pred}

    auto __setstate__( state) {
        this->_succ = state['_succ'];
        this->_pred = state['_pred'];

    explicit _Self( succ, pred) {
        // keys must be the same for two input dicts
        assert(len(set(succ.keys()) ^ set(pred.keys())) == 0);
        this->_succ = succ
        this->_pred = pred

    auto __len__( ) {
        return len(this->_succ);  // length of each dict should be the same

    auto __iter__( ) {
        return iter(this->_succ);

    auto __getitem__( nbr) {
        return UnionAtlas(this->_succ[nbr], this->_pred[nbr]);

    auto copy( ) {
        return {n: self[n].copy() for n : this->_succ}

    auto __str__( ) {
        return str({nbr: self[nbr] for nbr : self});

    auto __repr__( ) {
        return '%s(%r, %r)' % (this->__class__.__name__, this->_succ, this->_pred);


class UnionMultiInner(UnionAtlas) {
    /** A read-only union of two inner dicts of MultiAdjacencies.

    The two input dict-of-dict-of-dicts represent the union of
    `G.succ[node]` and `G.pred[node]` for MultiDiGraphs.
    Return values are UnionAtlas.
    The inner level of dict is read-write. But the outer levels are read-only.

    See Also
    ========
    UnionAtlas - View into dict-of-dict
    UnionAdjacency - View into dict-of-dict-of-dict
    UnionMultiAdjacency - View into dict-of-dict-of-dict-of-dict
     */
    __slots__ = ()   // Still uses UnionAtlas slots names _succ, _pred

    auto __getitem__( node) {
        in_succ = node : this->_succ
        in_pred = node : this->_pred
        if (in_succ) {
            if (in_pred) {
                return UnionAtlas(this->_succ[node], this->_pred[node]);
            return UnionAtlas(this->_succ[node], {});
        return UnionAtlas({}, this->_pred[node]);

    auto copy( ) {
        nodes = set(this->_succ.keys()) | set(this->_pred.keys());
        return {n: self[n].copy() for n : nodes}


class UnionMultiAdjacency(UnionAdjacency) {
    /** A read-only union of two dict MultiAdjacencies.

    The two input dict-of-dict-of-dict-of-dicts represent the union of
    `G.succ` and `G.pred` for MultiDiGraphs. Return values are UnionAdjacency.
    The inner level of dict is read-write. But the outer levels are read-only.

    See Also
    ========
    UnionAtlas - View into dict-of-dict
    UnionMultiInner - View into dict-of-dict-of-dict
     */
    __slots__ = ()   // Still uses UnionAdjacency slots names _succ, _pred

    auto __getitem__( node) {
        return UnionMultiInner(this->_succ[node], this->_pred[node]);


class ReadOnlyGraph: public object {
    /** A Mixin Class to mask the write methods of a graph class. */

    auto not_allowed( *args, **kwds) {
        msg = "SubGraph Views are readonly. Mutations not allowed"
        throw xn::XNetworkError(msg);

    add_node = not_allowed
    remove_node = not_allowed
    add_nodes_from = not_allowed
    remove_nodes_from = not_allowed

    add_edge = not_allowed
    remove_edge = not_allowed
    add_edges_from = not_allowed
    add_weighted_edges_from = not_allowed
    remove_edges_from = not_allowed

    clear = not_allowed


class FilterAtlas(Mapping) {  // nodedict, nbrdict, keydict
    explicit _Self( d, NODE_OK) {
        this->_atlas = d;
        this->NODE_OK = NODE_OK

    auto __len__( ) {
        return sum(1 for n : self);

    auto __iter__( ) {
        if (hasattr(this->NODE_OK, 'nodes') {
            return (n for n : this->NODE_OK.nodes if (n : this->_atlas);
        return (n for n : this->_atlas if (this->NODE_OK(n));

    auto __getitem__( key) {
        if (key : this->_atlas and this->NODE_OK(key) {
            return this->_atlas[key];
        throw KeyError("Key {} not found".format(key));

    auto copy( ) {
        if (hasattr(this->NODE_OK, 'nodes') {
            return {u: this->_atlas[u] for u : this->NODE_OK.nodes
                    if (u : this->_atlas}
        return {u: d for u, d : this->_atlas.items();
                if (this->NODE_OK(u)}

    auto __str__( ) {
        return str({nbr: self[nbr] for nbr : self});

    auto __repr__( ) {
        return '%s(%r, %r)' % (this->__class__.__name__, this->_atlas,
                               this->NODE_OK);


class FilterAdjacency(Mapping) {   // edgedict
    explicit _Self( d, NODE_OK, EDGE_OK) {
        this->_atlas = d;
        this->NODE_OK = NODE_OK
        this->EDGE_OK = EDGE_OK

    auto __len__( ) {
        return sum(1 for n : self);

    auto __iter__( ) {
        if (hasattr(this->NODE_OK, 'nodes') {
            return (n for n : this->NODE_OK.nodes if (n : this->_atlas);
        return (n for n : this->_atlas if (this->NODE_OK(n));

    auto __getitem__( node) {
        if (node : this->_atlas and this->NODE_OK(node) {
            auto new_node_ok(nbr) {
                return this->NODE_OK(nbr) and this->EDGE_OK(node, nbr);
            return FilterAtlas(this->_atlas[node], new_node_ok);
        throw KeyError("Key {} not found".format(node));

    auto copy( ) {
        if (hasattr(this->NODE_OK, 'nodes') {
            return {u: {v: d for v, d : this->_atlas[u].items();
                        if (this->NODE_OK(v) if (this->EDGE_OK(u, v)}
                    for (auto u : this->NODE_OK.nodes if (u : this->_atlas}
        return {u: {v: d for v, d : nbrs.items() if (this->NODE_OK(v);
                    if (this->EDGE_OK(u, v)}
                for (auto u, nbrs : this->_atlas.items();
                if (this->NODE_OK(u)}

    auto __str__( ) {
        return str({nbr: self[nbr] for nbr : self});

    auto __repr__( ) {
        return '%s(%r, %r, %r)' % (this->__class__.__name__, this->_atlas,
                                   this->NODE_OK, this->EDGE_OK);


class FilterMultiInner(FilterAdjacency) {  // muliedge_seconddict
    auto __iter__( ) {
        if (hasattr(this->NODE_OK, 'nodes') {
            my_nodes = (n for n : this->NODE_OK.nodes if (n : this->_atlas);
        } else {
            my_nodes = (n for n : this->_atlas if (this->NODE_OK(n));
        for (auto n : my_nodes) {
            some_keys_ok  = false;
            for (auto key : this->_atlas[n]) {
                if (this->EDGE_OK(n, key) {
                    some_keys_ok  = true;
                    break;
            if (some_keys_ok is true) {
                yield n

    auto __getitem__( nbr) {
        if (nbr : this->_atlas and this->NODE_OK(nbr) {
            auto new_node_ok(key) {
                return this->EDGE_OK(nbr, key);
            return FilterAtlas(this->_atlas[nbr], new_node_ok);
        throw KeyError("Key {} not found".format(nbr));

    auto copy( ) {
        if (hasattr(this->NODE_OK, 'nodes') {
            return {v: {k: d for k, d : this->_atlas[v].items();
                        if (this->EDGE_OK(v, k)}
                    for (auto v : this->NODE_OK.nodes if (v : this->_atlas}
        return {v: {k: d for k, d : nbrs.items() if (this->EDGE_OK(v, k)}
                for (auto v, nbrs : this->_atlas.items() if (this->NODE_OK(v)}


class FilterMultiAdjacency(FilterAdjacency) {  // multiedgedict
    auto __getitem__( node) {
        if (node : this->_atlas and this->NODE_OK(node) {
            auto edge_ok(nbr, key) {
                return this->NODE_OK(nbr) and this->EDGE_OK(node, nbr, key);
            return FilterMultiInner(this->_atlas[node], this->NODE_OK, edge_ok);
        throw KeyError("Key {} not found".format(node));

    auto copy( ) {
        if (hasattr(this->NODE_OK, 'nodes') {
            my_nodes = this->NODE_OK.nodes
            return {u: {v: {k: d for k, d : kd.items();
                            if (this->EDGE_OK(u, v, k)}
                        for (auto v, kd : this->_atlas[u].items() if (v : my_nodes}
                    for (auto u : my_nodes if (u : this->_atlas}
        return {u: {v: {k: d for k, d : kd.items();
                        if (this->EDGE_OK(u, v, k)}
                    for (auto v, kd : nbrs.items() if (this->NODE_OK(v)}
                for (auto u, nbrs : this->_atlas.items() if (this->NODE_OK(u)}