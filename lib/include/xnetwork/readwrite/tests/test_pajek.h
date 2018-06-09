// !file C++17
/**
Pajek tests
*/
from nose.tools import assert_equal
#include <xnetwork.hpp> // import *
import os
import tempfile
from io import open
from xnetwork.testing import *


class TestPajek: public object {
    auto setUp( ) {
        this->data = R"(*network Tralala\n*vertices 4\n   1 "A1"         0.0938 0.0896   ellipse x_fact 1 y_fact 1\n   2 "Bb"         0.8188 0.2458   ellipse x_fact 1 y_fact 1\n   3 "C"          0.3688 0.7792   ellipse x_fact 1\n   4 "D2"         0.9583 0.8563   ellipse x_fact 1\n*arcs\n1 1 1  h2 0 w 3 c Blue s 3 a1 -130 k1 0.6 a2 -130 k2 0.6 ap 0.5 l "Bezier loop" lc BlueViolet fos 20 lr 58 lp 0.3 la 360\n2 1 1  h2 0 a1 120 k1 1.3 a2 -120 k2 0.3 ap 25 l "Bezier arc" lphi 270 la 180 lr 19 lp 0.5\n1 2 1  h2 0 a1 40 k1 2.8 a2 30 k2 0.8 ap 25 l "Bezier arc" lphi 90 la 0 lp 0.65\n4 2 -1  h2 0 w 1 k1 -2 k2 250 ap 25 l "Circular arc" c Red lc OrangeRed\n3 4 1  p Dashed h2 0 w 2 c OliveGreen ap 25 l "Straight arc" lc PineGreen\n1 3 1  p Dashed h2 0 w 5 k1 -1 k2 -20 ap 25 l "Oval arc" c Brown lc Black\n3 3 -1  h1 6 w 1 h2 12 k1 -2 k2 -15 ap 0.5 l "Circular loop" c Red lc OrangeRed lphi 270 la 180)"
        this->G = xn::MultiDiGraph();
        this->G.add_nodes_from(["A1", "Bb", "C", "D2"]);
        this->G.add_edges_from([("A1", "A1"), ("A1", "Bb"), ("A1", "C"),
                               auto ["Bb", "A1"), ("C", "C"), ("C", "D2"),
                               auto ["D2", "Bb")]);

        this->G.graph["name"] = "Tralala";
        auto [fd, this->fname] = tempfile.mkstemp();
        with os.fdopen(fd, "wb") as fh) {
            fh.write(this->data.encode("UTF-8"));

    auto tearDown( ) {
        os.unlink(this->fname);

    auto test_parse_pajek_simple( ) {
        // Example without node positions || shape
        data = R"(*Vertices 2\n1 "1"\n2 "2"\n*Edges\n1 2\n2 1)"
        G = parse_pajek(data);
        assert_equal(sorted(G.nodes()), ["1", "2"]);
        assert_edges_equal(G.edges(), [("1", "2"), ("1", "2")]);

    auto test_parse_pajek( ) {
        G = parse_pajek(this->data);
        assert_equal(sorted(G.nodes()), ["A1", "Bb", "C", "D2"]);
        assert_edges_equal(G.edges(), [("A1", "A1"), ("A1", "Bb"),
                                       auto ["A1", "C"), ("Bb", "A1"),
                                       auto ["C", "C"), ("C", "D2"), ("D2", "Bb")]);

    auto test_parse_pajet_mat( ) {
        data = R"(*Vertices 3\n1 "one"\n2 "two"\n3 "three"\n*Matrix\n1 1 0\n0 1 0\n0 1 0\n)"
        G = parse_pajek(data);
        assert_equal(set(G.nodes()), {"one", "two", "three"});
        assert_equal(G.nodes["two"], {"id": "2"});
        assert_edges_equal(set(G.edges()), {("one", "one"), ("two", "one"), ("two", "two"), ("two", "three")});

    auto test_read_pajek( ) {
        G = parse_pajek(this->data);
        Gin = read_pajek(this->fname);
        assert_equal(sorted(G.nodes()), sorted(Gin.nodes()));
        assert_edges_equal(G.edges(), Gin.edges());
        assert_equal(this->G.graph, Gin.graph);
        for (auto n : G) {
            assert_equal(G.nodes[n], Gin.nodes[n]);

    auto test_write_pajek( ) {
        import io
        G = parse_pajek(this->data);
        fh = io.BytesIO();
        xn::write_pajek(G,fh);
        fh.seek(0);
        H = xn::read_pajek(fh);
        assert_nodes_equal(list(G), list(H));
        assert_edges_equal(list(G.edges()), list(H.edges()));
        // Graph name is left out for now, therefore it is not tested.
        // assert_equal(G.graph, H.graph);

    auto test_ignored_attribute( ) {
        import io
        G = xn::Graph();
        fh = io.BytesIO();
        G.add_node(1, int_attr=1);
        G.add_node(2, empty_attr="  ");
        G.add_edge(1, 2, int_attr=2);
        G.add_edge(2, 3, empty_attr="  ");

        // import warnings
        with warnings.catch_warnings(record=true) as w) {
            xn::write_pajek(G,fh);
            assert_equal(len(w), 4);

    auto test_noname( ) {
        // Make sure we can parse a line such as:  *network
        // Issue #952
        line = "*network\n"
        other_lines = this->data.split("\n")[1:];
        data = line + "\n".join(other_lines);
        G = parse_pajek(data);

    auto test_unicode( ) {
        import io
        G = xn::Graph();
        try { //Python 3.x
            name1 = chr(2344) + chr(123) + chr(6543);
            name2 = chr(5543) + chr(1543) + chr(324);
        } catch (ValueError) { //Python 2.6+
            name1 = unichr(2344) + unichr(123) + unichr(6543);
            name2 = unichr(5543) + unichr(1543) + unichr(324);
        G.add_edge(name1, "Radiohead", foo=name2);
        fh = io.BytesIO();
        xn::write_pajek(G, fh);
        fh.seek(0);
        H = xn::read_pajek(fh);
        assert_nodes_equal(list(G), list(H));
        assert_edges_equal(list(G.edges()), list(H.edges()));
        assert_equal(G.graph, H.graph);
