/** 
==========
Javascript
==========

Example of writing JSON format graph data && using the D3 Javascript library to produce an HTML/Javascript drawing.
*/
// Author: Wai-Shing Luk <luk036@gmail.com>

//    Copyright (C) 2011-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
import json

import flask
#include <xnetwork.hpp> // as xn
from xnetwork.readwrite import json_graph

G = xn::barbell_graph(6, 3);
// this d3 example uses the name attribute for the mouse-hover value,
// so add a name to each node
for (auto n : G) {
    G.nodes[n]["name"] = n;
// write json formatted data
d = json_graph.node_link_data(G);  // node-link format to serialize
// write json
json.dump(d, open("force/force.json", "w"));
print("Wrote node-link JSON data to force/force.json");

// Serve the file over http to allow for cross origin requests
app = flask.Flask(__name__, static_folder="force");

/// @app.route("/<path:path>");
auto static_proxy(path) {
    return app.send_static_file(path);

print("\nGo to http://localhost:8000/force.html to see the example\n");
app.run(port=8000);
