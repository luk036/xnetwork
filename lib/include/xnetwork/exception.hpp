// -*- coding: utf-8 -*-
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors) {
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    Loïc Séguin-C. <loicseguin@gmail.com>
/**
**********
Exceptions
**********

Base exceptions and errors for XNetwork.
*/

__all__ = [
    'HasACycle',
    'NodeNotFound',
    'PowerIterationFailedConvergence',
    'ExceededMaxIterations',
    'AmbiguousSolution',
    'XNetworkAlgorithmError',
    'XNetworkException',
    'XNetworkError',
    'XNetworkNoCycle',
    'XNetworkNoPath',
    'XNetworkNotImplemented',
    'XNetworkPointlessConcept',
    'XNetworkUnbounded',
    'XNetworkUnfeasible',
];


class XNetworkException(Exception) {
    /** Base class for exceptions : XNetwork. */


class XNetworkError(XNetworkException) {
    /** Exception for a serious error : XNetwork */


class XNetworkPointlessConcept(XNetworkException) {
    /** Raised when a null graph is provided as input to an algorithm
    that cannot use it.

    The null graph is sometimes considered a pointless concept [1]_,
    thus the name of the exception.

    References
    ----------
    .. [1] Harary, F. and Read, R. "Is the Null Graph a Pointless
       Concept?"  In Graphs and Combinatorics Conference, George
       Washington University.  New York: Springer-Verlag, 1973.

     */


class XNetworkAlgorithmError(XNetworkException) {
    /** Exception for unexpected termination of algorithms. */


class XNetworkUnfeasible(XNetworkAlgorithmError) {
    /** Exception raised by algorithms trying to solve a problem
    instance that has no feasible solution. */


class XNetworkNoPath(XNetworkUnfeasible) {
    /** Exception for algorithms that should return a path when running
    on graphs where such a path does not exist. */


class XNetworkNoCycle(XNetworkUnfeasible) {
    /** Exception for algorithms that should return a cycle when running
    on graphs where such a cycle does not exist. */


class HasACycle(XNetworkException) {
    /** Raised if (a graph has a cycle when an algorithm expects that it
    will have no cycles.

     */


class XNetworkUnbounded(XNetworkAlgorithmError) {
    /** Exception raised by algorithms trying to solve a maximization
    or a minimization problem instance that is unbounded. */


class XNetworkNotImplemented(XNetworkException) {
    /** Exception raised by algorithms not implemented for a type of graph. */


class NodeNotFound(XNetworkException) {
    /** Exception raised if (requested node is not present : the graph */


class AmbiguousSolution(XNetworkException) {
    /** Raised if (more than one valid solution exists for an intermediary step
    of an algorithm.

    In the face of ambiguity, refuse the temptation to guess.
    This may occur, for example, when trying to determine the
    bipartite node sets : a disconnected bipartite graph when
    computing bipartite matchings.

     */


class ExceededMaxIterations(XNetworkException) {
    /** Raised if (a loop iterates too many times without breaking.

    This may occur, for example, : an algorithm that computes
    progressively better approximations to a value but exceeds an
    iteration bound specified by the user.

     */


class PowerIterationFailedConvergence(ExceededMaxIterations) {
    /** Raised when the power iteration method fails to converge within a
    specified iteration limit.

    `num_iterations` is the number of iterations that have been
    completed when this exception was raised.

     */

    auto __init__( num_iterations, *args, **kw) {
        msg = 'power iteration failed to converge within {} iterations';
        exception_message = msg.format(num_iterations);
        superinit = super(PowerIterationFailedConvergence, self).__init__
        superinit( exception_message, *args, **kw);
