# Complexity Analysis

V = number of vertices, E = number of edges, d(u) = degree of vertex u.

## Space
| Representation | Space | Reason |
|---|---|---|
| Adjacency Matrix | **O(V²)** | V×V table regardless of E |
| Adjacency List | **O(V + E)** | V list heads + 2E nodes (undirected) |

Measured: small network (V=6, E=6): matrix 36 bytes, list 240 bytes (each list node is 16 bytes vs 1 byte per matrix cell, so constants dominate at tiny V). Sparse network (V=1000, E=2000): matrix 1,000,000 bytes, list 72,000 bytes.

## Traversal (BFS and DFS)
| Representation | Time | Reason |
|---|---|---|
| Matrix | **O(V²)** | each visited vertex scans a full row of V cells |
| List | **O(V + E)** | each vertex visited once, each adjacency node examined once |

Measured operations (cells/nodes examined): V=6 gives 36 (matrix) vs 12 (list) = V² vs 2E. V=1000 gives 1,000,000 vs 4,000.

## Search
- Vertex search by traversal (BFS/DFS until target is found): worst case (absent vertex) equals a full traversal, so **O(V²)** matrix and **O(V+E)** list. Measured: absent vertex `Z` cost 36 vs 12; present vertex `F` stopped early at 18 vs 7 (BFS).
- Label lookup in the vertex array: O(V) in both.

## Edge checking (is u adjacent to v?)
| Representation | Time |
|---|---|
| Matrix | **O(1)** |
| List | **O(d(u))**, worst case O(V) |

Measured: matrix always 1 operation; list 1–2 operations on the small graph, 4 on the sparse graph.

## Summary
| Operation | Matrix | List |
|---|---|---|
| Space | O(V²) | O(V+E) |
| BFS / DFS | O(V²) | O(V+E) |
| Edge check | O(1) | O(d(u)) |
| List all neighbours of u | O(V) | O(d(u)) |
