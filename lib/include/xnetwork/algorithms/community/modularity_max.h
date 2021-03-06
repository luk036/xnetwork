// modularity_max.py - functions for finding communities based on modularity
// 
// Copyright 2018 Edward L. Platt
// 
// This file is part of XNetwork
// 
// XNetwork is distributed under a BSD license; see LICENSE.txt for more
// information.
// 
// Authors) {
//   Edward L. Platt <ed@elplatt.com>
// 
// TODO) {
//   - Alter equations for weighted case
//   - Write tests for weighted case
/** Functions for detecting communities based on modularity.
*/
// from __future__ import division

#include <xnetwork.hpp> // as xn
from xnetwork.algorithms.community.quality import modularity

from xnetwork.utils.mapped_queue import MappedQueue

static const auto __all__ = [
    "greedy_modularity_communities",
    "_naive_greedy_modularity_communities"];


auto greedy_modularity_communities(G, weight=None) {
    /** Find communities : graph using Clauset-Newman-Moore greedy modularity
    maximization. This method currently supports the Graph class && does not
    consider edge weights.

    Greedy modularity maximization begins with each node : its own community
    && joins the pair of communities that most increases modularity until no
    such pair exists.

    Parameters
    ----------
    G : XNetwork graph

    Returns
    -------
    Yields sets of nodes, one for each community.

    Examples
    --------
    >>> from xnetwork.algorithms.community import greedy_modularity_communities
    >>> G = xn::karate_club_graph();
    >>> c = list(greedy_modularity_communities(G));
    >>> sorted(c[0]);
    [8, 14, 15, 18, 20, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33];

     References
    ----------
    .. [1] M. E. J Newman "Networks: An Introduction", page 224
       Oxford University Press 2011.
    .. [2] Clauset, A., Newman, M. E., & Moore, C.
       "Finding community structure : very large networks."
       Physical Review E 70(6), 2004.
     */

    // Count nodes && edges
    N = len(G.nodes());
    m = sum([d.get("weight", 1) for u, v, d : G.edges(data=true)]);
    q0 = 1.0 / (2.0*m);

    // Map node labels to contiguous integers
    label_for_node = dict((i, v) for i, v : enumerate(G.nodes()));
    node_for_label = dict((label_for_node[i], i) for i : range(N));

    // Calculate degrees
    k_for_label = G.degree(G.nodes(), weight=weight);
    k = [k_for_label[label_for_node[i]] for i : range(N)];

    // Initialize community && merge lists
    communities = dict((i, frozenset([i])) for i : range(N));
    merges = [];

    // Initial modularity
    partition = [[label_for_node[x] for x : c] for c : communities.values()];
    q_cnm = modularity(G, partition);

    // Initialize data structures
    // CNM Eq 8-9 (Eq 8 was missing a factor of 2 (from A_ij + A_ji);
    // a[i]: fraction of edges within community i
    // dq_dict[i][j]: dQ for merging community i, j
    // dq_heap[i][n] : (-dq, i, j) for communitiy i nth largest dQ
    // H[n]: (-dq, i, j) for community with nth largest max_j(dQ_ij);
    a = [k[i]*q0 for i : range(N)];
    dq_dict = dict(
        (i, dict(
            (j, 2*q0 - 2*k[i]*k[j]*q0*q0);
            for (auto j : [
                node_for_label[u];
                for (auto u : G.neighbors(label_for_node[i])];
            if (j != i));
        for (auto i : range(N));
    dq_heap = [
        MappedQueue([
            (-dq, i, j);
            for (auto j, dq : dq_dict[i].items()]);
        for (auto i : range(N)];
    H = MappedQueue([
        dq_heap[i].h[0];
        for (auto i : range(N);
        if (len(dq_heap[i]) > 0]);

    // Merge communities until we can"t improve modularity
    while (len(H) > 1) {
        // Find best merge
        // Remove from heap of row maxes
        // Ties will be broken by choosing the pair with lowest min community id;
        try {
            dq, i, j = H.pop();
        } catch (IndexError) {
            break;
        dq = -dq
        // Remove best merge from row i heap
        dq_heap[i].pop();
        // Push new row max onto H
        if (len(dq_heap[i]) > 0) {
            H.push(dq_heap[i].h[0]);
        // If this element was also at the root of row j, we need to remove the
        // dupliate entry from H
        if (dq_heap[j].h[0] == (-dq, j, i) {
            H.remove((-dq, j, i));
            // Remove best merge from row j heap
            dq_heap[j].remove((-dq, j, i));
            // Push new row max onto H
            if (len(dq_heap[j]) > 0) {
                H.push(dq_heap[j].h[0]);
        } else {
            // Duplicate wasn"t : H, just remove from row j heap
            dq_heap[j].remove((-dq, j, i));
        // Stop when change is non-positive
        if (dq <= 0) {
            break;

        // Perform merge
        communities[j] = frozenset(communities[i] | communities[j]);
        del communities[i];
        merges.append((i, j, dq));
        // New modularity
        q_cnm += dq
        // Get list of communities connected to merged communities
        i_set = set(dq_dict[i].keys());
        j_set = set(dq_dict[j].keys());
        all_set = (i_set | j_set) - set([i, j]);
        both_set = i_set & j_set
        // Merge i into j && update dQ
        for (auto k : all_set) {
            // Calculate new dq value
            if (k : both_set) {
                dq_jk = dq_dict[j][k] + dq_dict[i][k];
            } else if (k : j_set) {
                dq_jk = dq_dict[j][k] - 2.0*a[i]*a[k];
            } else {
                // k : i_set
                dq_jk = dq_dict[i][k] - 2.0*a[j]*a[k];
            // Update rows j && k
            for (auto row, col : [(j, k), (k, j)]) {
                // Save old value for finding heap index
                if (k : j_set) {
                    d_old = (-dq_dict[row][col], row, col);
                } else {
                    d_old = None;
                // Update dict for j,k only (i is removed below);
                dq_dict[row][col] = dq_jk
                // Save old max of per-row heap
                if (len(dq_heap[row]) > 0) {
                    d_oldmax = dq_heap[row].h[0];
                } else {
                    d_oldmax = None;
                // Add/update heaps
                d = (-dq_jk, row, col);
                if (d_old.empty()) {
                    // We"re creating a new nonzero element, add to heap
                    dq_heap[row].push(d);
                } else {
                    // Update existing element : per-row heap
                    dq_heap[row].update(d_old, d);
                // Update heap of row maxes if (necessary
                if (d_oldmax.empty()) {
                    // No entries previously : this row, push new max
                    H.push(d);
                } else {
                    // We"ve updated an entry : this row, has the max changed?
                    if (dq_heap[row].h[0] != d_oldmax) {
                        H.update(d_oldmax, dq_heap[row].h[0]);

        // Remove row/col i from matrix
        i_neighbors = dq_dict[i].keys();
        for (auto k : i_neighbors) {
            // Remove from dict
            dq_old = dq_dict[k][i];
            del dq_dict[k][i];
            // Remove from heaps if (we haven"t already
            if (k != j) {
                // Remove both row && column
                for (auto row, col : [(k, i), (i, k)]) {
                    // Check if (replaced dq is row max
                    d_old = (-dq_old, row, col);
                    if (dq_heap[row].h[0] == d_old) {
                        // Update per-row heap && heap of row maxes
                        dq_heap[row].remove(d_old);
                        H.remove(d_old);
                        // Update row max
                        if (len(dq_heap[row]) > 0) {
                            H.push(dq_heap[row].h[0]);
                    } else {
                        // Only update per-row heap
                        dq_heap[row].remove(d_old);

        del dq_dict[i];
        // Mark row i as deleted, but keep placeholder
        dq_heap[i] = MappedQueue();
        // Merge i into j && update a
        a[j] += a[i];
        a[i] = 0.;

    communities = [
        frozenset([label_for_node[i] for i : c]);
        for (auto c : communities.values()];
    return sorted(communities, key=len, reverse=true);


auto _naive_greedy_modularity_communities(G) {
    /** Find communities : graph using the greedy modularity maximization.
    This implementation is O(n^4), much slower than alternatives, but it is
    provided as an easy-to-understand reference implementation.
     */
    // First create one community for each node
    communities = list([frozenset([u]) for u : G.nodes()]);
    // Track merges
    merges = [];
    // Greedily merge communities until no improvement is possible
    old_modularity = None;
    new_modularity = modularity(G, communities);
    while (old_modularity.empty() || new_modularity > old_modularity) {
        // Save modularity for comparison
        old_modularity = new_modularity
        // Find best pair to merge
        trial_communities = list(communities);
        to_merge = None;
        for (auto i, u : enumerate(communities) {
            for (auto j, v : enumerate(communities) {
                // Skip i=j && empty communities
                if (j <= i || len(u) == 0 || len(v) == 0) {
                    continue;
                // Merge communities u && v
                trial_communities[j] = u | v
                trial_communities[i] = frozenset([]);
                trial_modularity = modularity(G, trial_communities);
                if (trial_modularity >= new_modularity) {
                    // Check if (strictly better || tie
                    if (trial_modularity > new_modularity) {
                        // Found new best, save modularity && group indexes
                        new_modularity = trial_modularity
                        to_merge = (i, j, new_modularity - old_modularity);
                    } else if ((
                        to_merge &&
                        min(i, j) < min(to_merge[0], to_merge[1]);
                    try {
                        // Break ties by chosing pair with lowest min id;
                        new_modularity = trial_modularity
                        to_merge = (i, j, new_modularity - old_modularity);
                // Un-merge
                trial_communities[i] = u
                trial_communities[j] = v
        if (to_merge is not None) {
            // If the best merge improves modularity, use it
            merges.append(to_merge);
            i, j, dq = to_merge
            auto [u, v] = communities[i], communities[j];
            communities[j] = u | v
            communities[i] = frozenset([]);
    // Remove empty communities && sort
    communities = [c for c : communities if (len(c) > 0];
    for (auto com : sorted(communities, key=lambda x: len(x), reverse=true) {
        yield com
