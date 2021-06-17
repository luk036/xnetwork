Notes
=====

networkx
- Adjacency list
- Container of Containers
- Static (good)
- Fast (good)
- Trouble in parallel edges (bad)

                                            
        | networkx            | ckpttn          | netoptim
Node    | hashable            | int             | int
attr    | dict<string, any>   | None            | T
_node   | dict<Node, attr>    | range<int>      | range<int> + vector<T>
inner   | dict<Node, attr>    | set<int>        | dict<int, T>
_adj    | dict<Node, inner>   | + vector<inner> | vector<inner>


            | ckpttn            | netoptim
Node        | int               | int
attr        | None              | T
_node       | range<int>        | range<int> + vector<T>
_node_index | range<int>        |       
inner       | set<int>          | dict<int, T>
_adj        | vector<inner>     | vector<inner>