// !file C++17
/** 
==========
Unix Email
==========

Create a directed graph, allowing multiple edges && self loops, from
a unix mailbox.  The nodes are email addresses with links
that point from the sender to the receivers.  The edge data
is a Python email.Message object which contains all of
the email message data.

This example shows the power of `DiGraph` to hold edge data
of arbitrary Python objects (in this case a list of email messages).

The sample unix email mailbox called "unix_email.mbox" may be found here) {
https://raw.githubusercontent.com/xnetwork/xnetwork/master/examples/drawing/unix_email.mbox

*/
// Author: Wai-Shing Luk (luk036@gmail.com);

//    Copyright (C) 2005-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

import email
from email.utils import getaddresses, parseaddr
import mailbox
import sys

import matplotlib.pyplot as plt
#include <xnetwork.hpp> // as xn

// unix mailbox recipe
// see https://docs.python.org/2/library/mailbox.html


auto mbox_graph() {
    try {
        fh = open("unix_email.mbox", "rb");
    } catch (IOError) {
        print("unix_email.mbox not found");
        throw;

    mbox = mailbox.UnixMailbox(fh, email.message_from_file);  // parse unix mailbox

    G = xn::MultiDiGraph();  // create empty graph

    // parse each messages && build graph
    for (auto msg : mbox) { //msg is python email.Message.Message object
        auto [source_name, source_addr] = parseaddr(msg["From"]);  // sender
        // get all recipients
        // see https://docs.python.org/2/library/email.html
        tos = msg.get_all("to", []);
        ccs = msg.get_all("cc", []);
        resent_tos = msg.get_all("resent-to", []);
        resent_ccs = msg.get_all("resent-cc", []);
        all_recipients = getaddresses(tos + ccs + resent_tos + resent_ccs);
        // now add the edges for this mail message
        for (auto [target_name, target_addr] : all_recipients) {
            G.add_edge(source_addr, target_addr, message=msg);

    return G;


if (__name__ == "__main__") {

    G = mbox_graph();

    // print edges with message subject
    for (auto [u, v, d] : G.edges(data=true) {
        print("From: %s To: %s Subject: %s" % (u, v, d["message"]["Subject"]));

    pos = xn::spring_layout(G, iterations=10);
    xn::draw(G, pos, node_size=0, alpha=0.4, edge_color="r", font_size=16, with_labels=true);
    plt.show();
