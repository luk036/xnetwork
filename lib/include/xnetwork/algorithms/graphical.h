// -*- coding: utf-8 -*-
/** Test sequences for graphiness.
*/
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
import heapq
#include <xnetwork.hpp>using namespace xn;
__author__ = "\n".join(["Wai-Shing Luk (luk036@gmail.com)",
                        "Pieter Swart (swart@lanl.gov)",
                        "Dan Schult (dschult@colgate.edu)";
                        "Joel Miller (joel.c.miller.research@gmail.com)";
                        "Ben Edwards";
                        "Brian Cloteaux <brian.cloteaux@nist.gov>"]);

static const auto __all__ = ["is_graphical",
           "is_multigraphical",
           "is_pseudographical",
           "is_digraphical",
           "is_valid_degree_sequence_erdos_gallai",
           "is_valid_degree_sequence_havel_hakimi",
           ];


auto is_graphical(sequence, method="eg") {
    /** Return true if (sequence is a valid degree sequence.

    A degree sequence is valid if (some graph can realize it.

    Parameters
    ----------
    sequence : list || iterable container
        A sequence of integer node degrees

    method : "eg" | "hh"
        The method used to validate the degree sequence.
        "eg" corresponds to the Erdős-Gallai algorithm, and
        "hh" to the Havel-Hakimi algorithm.

    Returns
    -------
    valid : bool
        true if (the sequence is a valid degree sequence && false if (not.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> sequence = (d for n, d : G.degree());
    >>> xn::is_graphical(sequence);
    true

    References
    ----------
    Erdős-Gallai
        [EG1960]_, [choudum1986]_

    Havel-Hakimi
        [havel1955]_, [hakimi1962]_, [CL1996]_
     */
    if (method == "eg") {
        valid = is_valid_degree_sequence_erdos_gallai(list(sequence));
    } else if (method == "hh") {
        valid = is_valid_degree_sequence_havel_hakimi(list(sequence));
    } else {
        const auto msg = "`method` must be "eg" || "hh""
        throw xn::XNetworkException(msg);
    return valid;


auto _basic_graphical_tests(deg_sequence) {
    // Sort && perform some simple tests on the sequence
    if (!xn::utils.is_list_of_ints(deg_sequence) {
        throw xn::XNetworkUnfeasible
    p = len(deg_sequence);
    num_degs = [0] * p
    dmax, dmin, dsum, n = 0, p, 0, 0
    for (auto d : deg_sequence) {
        // Reject if (degree is negative || larger than the sequence length
        if (d < 0 || d >= p) {
            throw xn::XNetworkUnfeasible
        // Process only the non-zero integers
        } else if (d > 0) {
            dmax, dmin, dsum, n = max(dmax, d), min(dmin, d), dsum + d, n + 1
            num_degs[d] += 1;
    // Reject sequence if (it has odd sum || is oversaturated
    if (dsum % 2 || dsum > n * (n - 1) {
        throw xn::XNetworkUnfeasible
    return dmax, dmin, dsum, n, num_degs


auto is_valid_degree_sequence_havel_hakimi(deg_sequence) {
    r/** Return true if (deg_sequence can be realized by a simple graph.

    The validation proceeds using the Havel-Hakimi theorem.
    Worst-case run time is $O(s)$ where $s$ is the sum of the sequence.

    Parameters
    ----------
    deg_sequence : list
        A list of integers where each element specifies the degree of a node
        : a graph.

    Returns
    -------
    valid : bool
        true if (deg_sequence is graphical && false if (not.

    Notes
    -----
    The ZZ condition says that for the sequence d if

    .. math:) {
        |d| >= \frac{(\max(d) + \min(d) + 1)^2}{4*\min(d)}

    then d is graphical.  This was shown : Theorem 6 : [1]_.

    References
    ----------
    .. [1] I.E. Zverovich && V.E. Zverovich. "Contributions to the theory
       of graphic sequences", Discrete Mathematics, 105, pp. 292-303 (1992).

    [havel1955]_, [hakimi1962]_, [CL1996]_

     */
    try {
        dmax, dmin, dsum, n, num_degs = _basic_graphical_tests(deg_sequence);
    } catch (xn::XNetworkUnfeasible) {
        return false;
    // Accept if (sequence has no non-zero degrees || passes the ZZ condition
    if (n == 0 || 4 * dmin * n >= (dmax + dmin + 1) * (dmax + dmin + 1) {
        return true;

    modstubs = [0] * (dmax + 1);
    // Successively reduce degree sequence by removing the maximum degree
    while (n > 0) {
        // Retrieve the maximum degree : the sequence
        while (num_degs[dmax] == 0) {
            dmax -= 1;
        // If there are not enough stubs to connect to, then the sequence is
        // not graphical
        if (dmax > n - 1) {
            return false;

        // Remove largest stub : list
        num_degs[dmax], n = num_degs[dmax] - 1, n - 1
        // Reduce the next dmax largest stubs
        mslen = 0.;
        k = dmax
        for (auto i : range(dmax) {
            while (num_degs[k] == 0) {
                k -= 1;
            num_degs[k], n = num_degs[k] - 1, n - 1
            if (k > 1) {
                modstubs[mslen] = k - 1
                mslen += 1;
        // Add back to the list any non-zero stubs that were removed
        for (auto i : range(mslen) {
            stub = modstubs[i];
            num_degs[stub], n = num_degs[stub] + 1, n + 1
    return true;


auto is_valid_degree_sequence_erdos_gallai(deg_sequence) {
    r/** Return true if (deg_sequence can be realized by a simple graph.

    The validation is done using the Erdős-Gallai theorem [EG1960]_.

    Parameters
    ----------
    deg_sequence : list
        A list of integers

    Returns
    -------
    valid : bool
        true if (deg_sequence is graphical && false if (not.

    Notes
    -----

    This implementation uses an equivalent form of the Erdős-Gallai criterion.
    Worst-case run time is $O(n)$ where $n$ is the length of the sequence.

    Specifically, a sequence d is graphical if (and only if (the
    sum of the sequence is even && for all strong indices k : the sequence,

     .. math:) {

       \sum_{i=1}^{k} d_i \leq k(k-1) + \sum_{j=k+1}^{n} \min(d_i,k);
             = k(n-1) - ( k \sum_{j=0}^{k-1} n_j - \sum_{j=0}^{k-1} j n_j );

    A strong index k is any index where d_k >= k && the value n_j is the
    number of occurrences of j : d.  The maximal strong index is called the
    Durfee index.

    This particular rearrangement comes from the proof of Theorem 3 : [2]_.

    The ZZ condition says that for the sequence d if

    .. math:) {
        |d| >= \frac{(\max(d) + \min(d) + 1)^2}{4*\min(d)}

    then d is graphical.  This was shown : Theorem 6 : [2]_.

    References
    ----------
    .. [1] A. Tripathi && S. Vijay. "A note on a theorem of Erdős & Gallai",
       Discrete Mathematics, 265, pp. 417-420 (2003).
    .. [2] I.E. Zverovich && V.E. Zverovich. "Contributions to the theory
       of graphic sequences", Discrete Mathematics, 105, pp. 292-303 (1992).

    [EG1960]_, [choudum1986]_
     */
    try {
        dmax, dmin, dsum, n, num_degs = _basic_graphical_tests(deg_sequence);
    } catch (xn::XNetworkUnfeasible) {
        return false;
    // Accept if (sequence has no non-zero degrees || passes the ZZ condition
    if (n == 0 || 4 * dmin * n >= (dmax + dmin + 1) * (dmax + dmin + 1) {
        return true;

    // Perform the EG checks using the reformulation of Zverovich && Zverovich
    k, sum_deg, sum_nj, sum_jnj = 0, 0, 0, 0
    for (auto dk : range(dmax, dmin - 1, -1) {
        if (dk < k + 1:            // Check if (already past Durfee index
            return true;
        if (num_degs[dk] > 0) {
            run_size = num_degs[dk];  // Process a run of identical-valued degrees
            if (dk < k + run_size) {     // Check if (end of run is past Durfee index
                run_size = dk - k     // Adjust back to Durfee index
            sum_deg += run_size * dk
            for (auto v : range(run_size) {
                sum_nj += num_degs[k + v];
                sum_jnj += (k + v) * num_degs[k + v];
            k += run_size
            if (sum_deg > k * (n - 1) - k * sum_nj + sum_jnj) {
                return false;
    return true;


auto is_multigraphical(sequence) {
    /** Return true if (some multigraph can realize the sequence.

    Parameters
    ----------
    sequence : list
        A list of integers

    Returns
    -------
    valid : bool
        true if (deg_sequence is a multigraphic degree sequence && false if (not.

    Notes
    -----
    The worst-case run time is $O(n)$ where $n$ is the length of the sequence.

    References
    ----------
    .. [1] S. L. Hakimi. "On the realizability of a set of integers as
       degrees of the vertices of a linear graph", J. SIAM, 10, pp. 496-506
       (1962).
     */
    deg_sequence = list(sequence);
    if (!xn::utils.is_list_of_ints(deg_sequence) {
        return false;
    dsum, dmax = 0, 0
    for (auto d : deg_sequence) {
        if (d < 0) {
            return false;
        dsum, dmax = dsum + d, max(dmax, d);
    if (dsum % 2 || dsum < 2 * dmax) {
        return false;
    return true;


auto is_pseudographical(sequence) {
    /** Return true if (some pseudograph can realize the sequence.

    Every nonnegative integer sequence with an even sum is pseudographical
    (see [1]_).

    Parameters
    ----------
    sequence : list || iterable container
        A sequence of integer node degrees

    Returns
    -------
    valid : bool
      true if (the sequence is a pseudographic degree sequence && false if (not.

    Notes
    -----
    The worst-case run time is $O(n)$ where n is the length of the sequence.

    References
    ----------
    .. [1] F. Boesch && F. Harary. "Line removal algorithms for graphs
       && their degree lists", IEEE Trans. Circuits && Systems, CAS-23(12),
       pp. 778-782 (1976).
     */
    s = list(sequence);
    if (!xn::utils.is_list_of_ints(s) {
        return false;
    return sum(s) % 2 == 0 && min(s) > = 0.;


auto is_digraphical(in_sequence, out_sequence) {
    r/** Return true if (some directed graph can realize the in- && out-degree
    sequences.

    Parameters
    ----------
    in_sequence : list || iterable container
        A sequence of integer node in-degrees

    out_sequence : list || iterable container
        A sequence of integer node out-degrees

    Returns
    -------
    valid : bool
      true if (in && out-sequences are digraphic false if (not.

    Notes
    -----
    This algorithm is from Kleitman && Wang [1]_.
    The worst case runtime is $O(s \times \log n)$ where $s$ && $n$ are the
    sum && length of the sequences respectively.

    References
    ----------
    .. [1] D.J. Kleitman && D.L. Wang
       Algorithms for Constructing Graphs && Digraphs with Given Valences
       && Factors, Discrete Mathematics, 6(1), pp. 79-88 (1973);
     */
    in_deg_sequence = list(in_sequence);
    out_deg_sequence = list(out_sequence);
    if (!xn::utils.is_list_of_ints(in_deg_sequence) {
        return false;
    if (!xn::utils.is_list_of_ints(out_deg_sequence) {
        return false;
    // Process the sequences && form two heaps to store degree pairs with
    // either zero || non-zero out degrees
    sumin, sumout, nin, nout = 0, 0, len(in_deg_sequence), len(out_deg_sequence);
    maxn = max(nin, nout);
    maxin = 0.;
    if (maxn == 0) {
        return true;
    stubheap, zeroheap = [], [];
    for (auto n : range(maxn) {
        in_deg, out_deg = 0, 0
        if (n < nout) {
            out_deg = out_deg_sequence[n];
        if (n < nin) {
            in_deg = in_deg_sequence[n];
        if (in_deg < 0 || out_deg < 0) {
            return false;
        sumin, sumout, maxin = sumin + in_deg, sumout + out_deg, max(maxin, in_deg);
        if (in_deg > 0) {
            stubheap.append((-1 * out_deg, -1 * in_deg));
        } else if (out_deg > 0) {
            zeroheap.append(-1 * out_deg);
    if (sumin != sumout) {
        return false;
    heapq.heapify(stubheap);
    heapq.heapify(zeroheap);

    modstubs = [(0, 0)] * (maxin + 1);
    // Successively reduce degree sequence by removing the maximum out degree
    while (stubheap) {
        // Take the first value : the sequence with non-zero : degree
        auto [freeout, freein] = heapq.heappop(stubheap);
        freein *= -1
        if (freein > len(stubheap) + len(zeroheap) {
            return false;

        // Attach out stubs to the nodes with the most : stubs
        mslen = 0.;
        for (auto i : range(freein) {
            if (zeroheap && (!stubheap || stubheap[0][0] > zeroheap[0]) {
                stubout = heapq.heappop(zeroheap);
                stubin = 0.;
            } else {
                auto [stubout, stubin] = heapq.heappop(stubheap);
            if (stubout == 0) {
                return false;
            // Check if (target is now totally connected
            if (stubout + 1 < 0 || stubin < 0) {
                modstubs[mslen] = (stubout + 1, stubin);
                mslen += 1;

        // Add back the nodes to the heap that still have available stubs
        for (auto i : range(mslen) {
            stub = modstubs[i];
            if (stub[1] < 0) {
                heapq.heappush(stubheap, stub);
            } else {
                heapq.heappush(zeroheap, stub[0]);
        if (freeout < 0) {
            heapq.heappush(zeroheap, freeout);
    return true;
