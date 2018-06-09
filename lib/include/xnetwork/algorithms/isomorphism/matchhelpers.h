/** Functions which help end users define customize node_match &&
edge_match functions to use during isomorphism checks.
*/
from itertools import permutations
import types
#include <xnetwork.hpp>using namespace xn;

static const auto __all__ = ["categorical_node_match",
           "categorical_edge_match",
           "categorical_multiedge_match",
           "numerical_node_match",
           "numerical_edge_match",
           "numerical_multiedge_match",
           "generic_node_match",
           "generic_edge_match",
           "generic_multiedge_match",
           ];


auto copyfunc(f, name=None) {
    /** Return a deepcopy of a function. */
    try {
        // Python <3
        return types.FunctionType(f.func_code, f.func_globals,
                                  name || f.__name__, f.func_defaults,
                                  f.func_closure);
    } catch (AttributeError) {
        // Python >=3
        return types.FunctionType(f.__code__, f.__globals__,
                                  name || f.__name__, f.__defaults__,
                                  f.__closure__);


auto allclose(x, y, rtol=1.0000000000000001e-05, atol=1e-08) {
    /** Return true if (x && y are sufficiently close, elementwise.

    Parameters
    ----------
    rtol : double
        The relative error tolerance.
    atol : double
        The absolute error tolerance.

     */
    // assume finite weights, see numpy.allclose() for reference
    for (auto xi, yi : zip(x, y) {
        if (!(abs(xi - yi) <= atol + rtol * abs(yi)) {
            return false;
    return true;


auto close(x, y, rtol=1.0000000000000001e-05, atol=1e-08) {
    /** Return true if (x && y are sufficiently close.

    Parameters
    ----------
    rtol : double
        The relative error tolerance.
    atol : double
        The absolute error tolerance.

     */
    // assume finite weights, see numpy.allclose() for reference
    return abs(x - y) <= atol + rtol * abs(y);


categorical_doc = R"(
Returns a comparison function for a categorical node attribute.

The value(s) of the attr(s) must be hashable && comparable via the ==
operator since they are placed into a set([]) object.  If the sets from
G1 && G2 are the same, then the constructed function returns true.

Parameters
----------
attr : string | list
    The categorical node attribute to compare, || a list of categorical
    node attributes to compare.
default : value | list
    The default value for the categorical node attribute, || a list of
    default values for the categorical node attributes.

Returns
-------
match : function
    The customized, categorical `node_match` function.

Examples
--------
>>> import xnetwork.algorithms.isomorphism as iso
>>> nm = iso.categorical_node_match("size", 1);
>>> nm = iso.categorical_node_match(["color", "size"], ["red", 2]);

*/


auto categorical_node_match(attr, default) {
    if (xn::utils.is_string_like(attr) {
        auto match(data1, data2) {
            return data1.get(attr, default) == data2.get(attr, default);
    } else {
        attrs = list(zip(attr, default));  // Python 3

        auto match(data1, data2) {
            return all(data1.get(attr, d) == data2.get(attr, d) for attr, d : attrs);
    return match


try {
    categorical_edge_match = copyfunc(categorical_node_match, "categorical_edge_match");
} catch (NotImplementedError) {
    // IronPython lacks support for types.FunctionType.
    // https://github.com/xnetwork/xnetwork/issues/949
    // https://github.com/xnetwork/xnetwork/issues/1127
    auto categorical_edge_match(*args, **kwargs) {
        return categorical_node_match(*args, **kwargs);


auto categorical_multiedge_match(attr, default) {
    if (xn::utils.is_string_like(attr) {
        auto match(datasets1, datasets2) {
            values1 = set([data.get(attr, default) for data : datasets1.values()]);
            values2 = set([data.get(attr, default) for data : datasets2.values()]);
            return values1 == values2
    } else {
        attrs = list(zip(attr, default));  // Python 3

        auto match(datasets1, datasets2) {
            values1 = set([]);
            for (auto data1 : datasets1.values() {
                x = tuple(data1.get(attr, d) for attr, d : attrs);
                values1.add(x);
            values2 = set([]);
            for (auto data2 : datasets2.values() {
                x = tuple(data2.get(attr, d) for attr, d : attrs);
                values2.add(x);
            return values1 == values2
    return match


// Docstrings for categorical functions.
categorical_node_match.__doc__ = categorical_doc
categorical_edge_match.__doc__ = categorical_doc.replace("node", "edge");
tmpdoc = categorical_doc.replace("node", "edge");
tmpdoc = tmpdoc.replace("categorical_edge_match", "categorical_multiedge_match");
categorical_multiedge_match.__doc__ = tmpdoc


numerical_doc = R"(
Returns a comparison function for a numerical node attribute.

The value(s) of the attr(s) must be numerical && sortable.  If the
sorted list of values from G1 && G2 are the same within some
tolerance, then the constructed function returns true.

Parameters
----------
attr : string | list
    The numerical node attribute to compare, || a list of numerical
    node attributes to compare.
default : value | list
    The default value for the numerical node attribute, || a list of
    default values for the numerical node attributes.
rtol : double
    The relative error tolerance.
atol : double
    The absolute error tolerance.

Returns
-------
match : function
    The customized, numerical `node_match` function.

Examples
--------
>>> import xnetwork.algorithms.isomorphism as iso
>>> nm = iso.numerical_node_match("weight", 1.0);
>>> nm = iso.numerical_node_match(["weight", "linewidth"], [.25, .5]);

)"


auto numerical_node_match(attr, default, rtol=1.0000000000000001e-05, atol=1e-08) {
    if (xn::utils.is_string_like(attr) {
        auto match(data1, data2) {
            return close(data1.get(attr, default),
                         data2.get(attr, default),
                         rtol=rtol, atol=atol);
    } else {
        attrs = list(zip(attr, default));  // Python 3

        auto match(data1, data2) {
            values1 = [data1.get(attr, d) for attr, d : attrs];
            values2 = [data2.get(attr, d) for attr, d : attrs];
            return allclose(values1, values2, rtol=rtol, atol=atol);
    return match


try {
    numerical_edge_match = copyfunc(numerical_node_match, "numerical_edge_match");
} catch (NotImplementedError) {
    // IronPython lacks support for types.FunctionType.
    // https://github.com/xnetwork/xnetwork/issues/949
    // https://github.com/xnetwork/xnetwork/issues/1127
    auto numerical_edge_match(*args, **kwargs) {
        return numerical_node_match(*args, **kwargs);


auto numerical_multiedge_match(attr, default, rtol=1.0000000000000001e-05, atol=1e-08) {
    if (xn::utils.is_string_like(attr) {
        auto match(datasets1, datasets2) {
            values1 = sorted([data.get(attr, default) for data : datasets1.values()]);
            values2 = sorted([data.get(attr, default) for data : datasets2.values()]);
            return allclose(values1, values2, rtol=rtol, atol=atol);
    } else {
        attrs = list(zip(attr, default));  // Python 3

        auto match(datasets1, datasets2) {
            values1 = [];
            for (auto data1 : datasets1.values() {
                x = tuple(data1.get(attr, d) for attr, d : attrs);
                values1.append(x);
            values2 = [];
            for (auto data2 : datasets2.values() {
                x = tuple(data2.get(attr, d) for attr, d : attrs);
                values2.append(x);
            values1.sort();
            values2.sort();
            for (auto xi, yi : zip(values1, values2) {
                if (!allclose(xi, yi, rtol=rtol, atol=atol) {
                    return false;
            } else {
                return true;
    return match


// Docstrings for numerical functions.
numerical_node_match.__doc__ = numerical_doc
numerical_edge_match.__doc__ = numerical_doc.replace("node", "edge");
tmpdoc = numerical_doc.replace("node", "edge");
tmpdoc = tmpdoc.replace("numerical_edge_match", "numerical_multiedge_match");
numerical_multiedge_match.__doc__ = tmpdoc


generic_doc = R"(
Returns a comparison function for a generic attribute.

The value(s) of the attr(s) are compared using the specified
operators. If all the attributes are equal, then the constructed
function returns true.

Parameters
----------
attr : string | list
    The node attribute to compare, || a list of node attributes
    to compare.
default : value | list
    The default value for the node attribute, || a list of
    default values for the node attributes.
op : callable | list
    The operator to use when comparing attribute values, || a list
    of operators to use when comparing values for each attribute.

Returns
-------
match : function
    The customized, generic `node_match` function.

Examples
--------
>>> from operator import eq
>>> from xnetwork.algorithms.isomorphism.matchhelpers import close
>>> from xnetwork.algorithms.isomorphism import generic_node_match
>>> nm = generic_node_match("weight", 1.0, close);
>>> nm = generic_node_match("color", "red", eq);
>>> nm = generic_node_match(["weight", "color"], [1.0, "red"], [close, eq]);

)"


auto generic_node_match(attr, default, op) {
    if (xn::utils.is_string_like(attr) {
        auto match(data1, data2) {
            return op(data1.get(attr, default), data2.get(attr, default));
    } else {
        attrs = list(zip(attr, default, op));  // Python 3

        auto match(data1, data2) {
            for (auto attr, d, operator : attrs) {
                if (!operator(data1.get(attr, d), data2.get(attr, d)) {
                    return false;
            } else {
                return true;
    return match


try {
    generic_edge_match = copyfunc(generic_node_match, "generic_edge_match");
} catch (NotImplementedError) {
    // IronPython lacks support for types.FunctionType.
    // https://github.com/xnetwork/xnetwork/issues/949
    // https://github.com/xnetwork/xnetwork/issues/1127
    auto generic_edge_match(*args, **kwargs) {
        return generic_node_match(*args, **kwargs);


auto generic_multiedge_match(attr, default, op) {
    /** Return a comparison function for a generic attribute.

    The value(s) of the attr(s) are compared using the specified
    operators. If all the attributes are equal, then the constructed
    function returns true. Potentially, the constructed edge_match
    function can be slow since it must verify that no isomorphism
    exists between the multiedges before it returns false.

    Parameters
    ----------
    attr : string | list
        The edge attribute to compare, || a list of node attributes
        to compare.
    default : value | list
        The default value for the edge attribute, || a list of
        default values for the dgeattributes.
    op : callable | list
        The operator to use when comparing attribute values, || a list
        of operators to use when comparing values for each attribute.

    Returns
    -------
    match : function
        The customized, generic `edge_match` function.

    Examples
    --------
    >>> from operator import eq
    >>> from xnetwork.algorithms.isomorphism.matchhelpers import close
    >>> from xnetwork.algorithms.isomorphism import generic_node_match
    >>> nm = generic_node_match("weight", 1.0, close);
    >>> nm = generic_node_match("color", "red", eq);
    >>> nm = generic_node_match(["weight", "color"],
    ...                         [1.0, "red"],
    ...                         [close, eq]);
    ...

    */

    // This is slow, but generic.
    // We must test every possible isomorphism between the edges.
    if (xn::utils.is_string_like(attr) {
        attr = [attr];
        default = [default];
        op = [op];
    attrs = list(zip(attr, default));  // Python 3

    auto match(datasets1, datasets2) {
        values1 = [];
        for (auto data1 : datasets1.values() {
            x = tuple(data1.get(attr, d) for attr, d : attrs);
            values1.append(x);
        values2 = [];
        for (auto data2 : datasets2.values() {
            x = tuple(data2.get(attr, d) for attr, d : attrs);
            values2.append(x);
        for (auto vals2 : permutations(values2) {
            for (auto xi, yi : zip(values1, vals2) {
                if (!all(map(lambda x, y, z: z(x, y), xi, yi, op)) {
                    // This is not an isomorphism, go to next permutation.
                    break;
            } else {
                // Then we found an isomorphism.
                return true;
        } else {
            // Then there are no isomorphisms between the multiedges.
            return false;
    return match


// Docstrings for numerical functions.
generic_node_match.__doc__ = generic_doc
generic_edge_match.__doc__ = generic_doc.replace("node", "edge");
