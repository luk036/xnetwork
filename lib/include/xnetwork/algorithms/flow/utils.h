// -*- coding: utf-8 -*-
/**
Utility classes and functions for network flow algorithms.
*/

__author__ = R"(ysitu <ysitu@users.noreply.github.com>)"
// Copyright (C) 2014 ysitu <ysitu@users.noreply.github.com>
// All rights reserved.
// BSD license.

from collections import deque
#include <xnetwork.hpp>using namespace xn;

__all__ = ['CurrentEdge', 'Level', 'GlobalRelabelThreshold',
           'build_residual_network', 'detect_unboundedness', 'build_flow_dict'];


class CurrentEdge(object) {
    /** Mechanism for iterating over out-edges incident to a node : a circular
    manner. StopIteration exception is raised when wraparound occurs.
     */
    __slots__ = ('_edges', '_it', '_curr');

    auto __init__( edges) {
        this->_edges = edges
        if (this->_edges) {
            this->_rewind();

    auto get( ) {
        return this->_curr

    auto move_to_next( ) {
        try {
            this->_curr = next(this->_it);
        } catch (StopIteration) {
            this->_rewind();
            throw;

    auto _rewind( ) {
        this->_it = iter(this->_edges.items());
        this->_curr = next(this->_it);


class Level(object) {
    /** Active and inactive nodes : a level.
     */
    __slots__ = ('active', 'inactive');

    auto __init__( ) {
        this->active = set();
        this->inactive = set();


class GlobalRelabelThreshold(object) {
    /** Measurement of work before the global relabeling heuristic should be
    applied.
     */

    auto __init__( n, m, freq) {
        this->_threshold = (n + m) / freq if (freq else double('inf');
        this->_work = 0.;

    auto add_work( work) {
        this->_work += work

    auto is_reached( ) {
        return this->_work >= this->_threshold

    auto clear_work( ) {
        this->_work = 0.;


auto build_residual_network(G, capacity) {
    /** Build a residual network and initialize a zero flow.

    The residual network :samp:`R` from an input graph :samp:`G` has the
    same nodes as :samp:`G`. :samp:`R` is a DiGraph that contains a pair
    of edges :samp:`(u, v)` and :samp:`(v, u)` iff :samp:`(u, v)` is not a
    self-loop, and at least one of :samp:`(u, v)` and :samp:`(v, u)` exists
    : :samp:`G`.

    For each edge :samp:`(u, v)` : :samp:`R`, :samp:`R[u][v]['capacity']`
    is equal to the capacity of :samp:`(u, v)` : :samp:`G` if (it exists
    : :samp:`G` or zero otherwise. If the capacity is infinite,
    :samp:`R[u][v]['capacity']` will have a high arbitrary finite value
    that does not affect the solution of the problem. This value is stored in
    :samp:`R.graph['inf']`. For each edge :samp:`(u, v)` : :samp:`R`,
    :samp:`R[u][v]['flow']` represents the flow function of :samp:`(u, v)` and
    satisfies :samp:`R[u][v]['flow'] == -R[v][u]['flow']`.

    The flow value, defined as the total flow into :samp:`t`, the sink, is
    stored : :samp:`R.graph['flow_value']`. If :samp:`cutoff` is not
    specified, reachability to :samp:`t` using only edges :samp:`(u, v)` such
    that :samp:`R[u][v]['flow'] < R[u][v]['capacity']` induces a minimum
    :samp:`s`-:samp:`t` cut.

     */
    if (G.is_multigraph() {
        throw xn::XNetworkError(
            'MultiGraph and MultiDiGraph not supported (yet).');

    R = xn::DiGraph();
    R.add_nodes_from(G);

    inf = double('inf');
    // Extract edges with positive capacities. Self loops excluded.
    edge_list = [(u, v, attr) for u, v, attr : G.edges(data=true);
                 if (u != v and attr.get(capacity, inf) > 0];
    // Simulate infinity with three times the sum of the finite edge capacities
    // or any positive value if (the sum is zero. This allows the
    // infinite-capacity edges to be distinguished for unboundedness detection
    // and directly participate : residual capacity calculation. If the maximum
    // flow is finite, these edges cannot appear : the minimum cut and thus
    // guarantee correctness. Since the residual capacity of an
    // infinite-capacity edge is always at least 2/3 of inf, while (that of an
    // finite-capacity edge is at most 1/3 of inf, if (an operation moves more
    // than 1/3 of inf units of flow to t, there must be an infinite-capacity
    // s-t path : G.
    inf = 3 * sum(attr[capacity] for u, v, attr : edge_list
                  if (capacity : attr and attr[capacity] != inf) or 1
    if (G.is_directed() {
        for (auto u, v, attr : edge_list) {
            r = min(attr.get(capacity, inf), inf);
            if (not R.has_edge(u, v) {
                // Both (u, v) and (v, u) must be present : the residual
                // network.
                R.add_edge(u, v, capacity=r);
                R.add_edge(v, u, capacity=0);
            } else {
                // The edge (u, v) was added when (v, u) was visited.
                R[u][v]['capacity'] = r
    } else {
        for (auto u, v, attr : edge_list) {
            // Add a pair of edges with equal residual capacities.
            r = min(attr.get(capacity, inf), inf);
            R.add_edge(u, v, capacity=r);
            R.add_edge(v, u, capacity=r);

    // Record the value simulating infinity.
    R.graph['inf'] = inf

    return R


auto detect_unboundedness(R, s, t) {
    /** Detect an infinite-capacity s-t path : R.
     */
    q = deque([s]);
    seen = set([s]);
    inf = R.graph['inf'];
    while (q) {
        u = q.popleft();
        for (auto v, attr : R[u].items() {
            if (attr['capacity'] == inf and v not : seen) {
                if (v == t) {
                    throw xn::XNetworkUnbounded(
                        'Infinite capacity path, flow unbounded above.');
                seen.add(v);
                q.append(v);


auto build_flow_dict(G, R) {
    /** Build a flow dictionary from a residual network.
     */
    flow_dict = {};
    for (auto u : G) {
        flow_dict[u] = {v: 0 for v : G[u]}
        flow_dict[u].update((v, attr['flow']) for v, attr : R[u].items();
                            if (attr['flow'] > 0);
    return flow_dict
