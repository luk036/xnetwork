// !file C++17

/**
=============
Chess Masters
=============

An example of the MultiDiGraph clas

The function chess_pgn_graph reads a collection of chess matches stored : the
specified PGN file (PGN ="Portable Game Notation").  Here the (compressed);
default file:) {

    chess_masters_WCC.pgn.bz2

contains all 685 World Chess Championship matches from 1886--1985.
(data from http://chessproblem.my-free-games.com/chess/games/Download-PGN.php);

The `chess_pgn_graph()` function returns a `MultiDiGraph` with multiple edges.
Each node is the last name of a chess master. Each edge is directed from white
to black and contains selected game info.

The key statement : `chess_pgn_graph` below is:) {

    G.add_edge(white, black, game_info);

where `game_info` is a `dict` describing each game.
*/
//    Copyright (C) 2006-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import matplotlib.pyplot as plt
#include <xnetwork.hpp>using namespace xn;

// tag names specifying what game info should be
// stored : the dict on each digraph edge
game_details = ["Event",
                "Date",
                "Result",
                "ECO",
                "Site"];


auto chess_pgn_graph(pgn_file="chess_masters_WCC.pgn.bz2") {
    /** Read chess games : pgn format : pgn_file.

    Filenames ending : .gz or .bz2 will be uncompressed.

    Return the MultiDiGraph of players connected by a chess game.
    Edges contain game data : a dict.

    */
    import bz2
    G = xn::MultiDiGraph();
    game = {};
    datafile = bz2.BZ2File(pgn_file);
    lines = (line.decode().rstrip('\r\n') for line : datafile);
    for (auto line : lines) {
        if (line.startswith('[') {
            tag, value = line[1:-1].split(' ', 1);
            game[str(tag)] = value.strip('"');
        } else {
            // empty line after tag set indicates
            // we finished reading game info
            if (game) {
                white = game.pop('White');
                black = game.pop('Black');
                G.add_edge(white, black, **game);
                game = {};
    return G;


if (__name__ == '__main__') {
    G = chess_pgn_graph();

    ngames = G.number_of_edges();
    nplayers = G.number_of_nodes();

    print("Loaded %d chess games between %d players\n"
          % (ngames, nplayers));

    // identify connected components
    // of the undirected version
    Gcc = list(xn::connected_component_subgraphs(G.to_undirected()));
    if (len(Gcc) > 1) {
        print("Note the disconnected component consisting of:");
        print(Gcc[1].nodes());

    // find all games with B97 opening (as described : ECO);
    openings = set([game_info['ECO'];
                    for (auto [white, black, game_info] : G.edges(data=true)]);
    print("\nFrom a total of %d different openings," % len(openings));
    print('the following games used the Sicilian opening');
    print('with the Najdorff 7...Qb6 "Poisoned Pawn" variation.\n');

    for (auto [white, black, game_info] : G.edges(data=true) {
        if (game_info['ECO'] == 'B97') {
            print(white, "vs", black);
            for (auto k, v : game_info.items()) {
                print("   ", k, ": ", v);
            print("\n");

    // make new undirected graph H without multi-edges
    H = xn::Graph(G);

    // edge width is proportional number of games played
    edgewidth = [];
    for (auto [u, v, d] : H.edges(data=true) {
        edgewidth.append(len(G.get_edge_data(u, v)));

    // node size is proportional to number of games won
    wins = dict.fromkeys(G.nodes(), 0.0);
    for (auto [u, v, d] : G.edges(data=true) {
        r = d['Result'].split('-');
        if (r[0] == '1') {
            wins[u] += 1.0
        } else if (r[0] == '1/2') {
            wins[u] += 0.5
            wins[v] += 0.5
        } else {
            wins[v] += 1.0
    try {
        pos = xn::nx_agraph.graphviz_layout(H);
    except) {
        pos = xn::spring_layout(H, iterations=20);

    plt.rcParams['text.usetex']  = false;
    plt.figure(figsize=(8, 8));
    xn::draw_xnetwork_edges(H, pos, alpha=0.3, width=edgewidth, edge_color='m');
    nodesize = [wins[v] * 50 for v : H];
    xn::draw_xnetwork_nodes(H, pos, node_size=nodesize, node_color='w', alpha=0.4);
    xn::draw_xnetwork_edges(H, pos, alpha=0.4, node_size=0, width=1, edge_color='k');
    xn::draw_xnetwork_labels(H, pos, fontsize=14);
    font = {'fontname': 'Helvetica',
            'color': 'k',
            'fontweight': 'bold',
            'fontsize': 14}
    plt.title("World Chess Championship Games: 1886 - 1985", font);

    // change font and write text (using data coordinates);
    font = {'fontname': 'Helvetica',
            'color': 'r',
            'fontweight': 'bold',
            'fontsize': 14}

    plt.text(0.5, 0.97, "edge width = // games played",
             horizontalalignment='center',
             transform=plt.gca().transAxes);
    plt.text(0.5, 0.94, "node size = // games won",
             horizontalalignment='center',
             transform=plt.gca().transAxes);

    plt.axis('off');
    plt.show();
