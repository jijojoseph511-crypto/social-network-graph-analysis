# Comparison Table (from `output.txt`)

## Small network (V=6, E=6)
| Criterion | Adjacency Matrix | Adjacency List |
|---|---|---|
| Space | 36 bytes | 240 bytes |
| BFS order from A | A B C D E F | A B C D E F |
| DFS order from A | A B D E F C | A B D E F C |
| BFS operations | 36 | 12 |
| DFS operations | 36 | 12 |
| Search `F` (BFS ops) | 18 | 7 |
| Search `F` (DFS ops) | 19 | 7 |
| Search `Z` (absent, BFS/DFS ops) | 36 | 12 |
| Edge check ops (A-B, A-F, E-F, D-F) | 1, 1, 1, 1 | 1, 2, 2, 1 |
| BFS time (avg per run) | 0.2129 µs | 0.1631 µs |
| DFS time (avg per run) | 0.1980 µs | 0.1527 µs |

## Larger sparse network (V=1000, E=2000, density 0.40%)
| Criterion | Adjacency Matrix | Adjacency List |
|---|---|---|
| Space | 1,000,000 bytes | 72,000 bytes |
| BFS operations | 1,000,000 | 4,000 |
| DFS operations | 1,000,000 | 4,000 |
| Edge check (absent edge) | 1 op | 4 ops |
| BFS time (avg per run) | 3024.82 µs | 19.16 µs |
| DFS time (avg per run) | 3641.92 µs | 39.01 µs |

## Theoretical
| Criterion | Matrix | List |
|---|---|---|
| Space | O(V²) | O(V+E) |
| Traversal | O(V²) | O(V+E) |
| Edge check | O(1) | O(d(u)) |

Timings vary slightly between runs and machines; operation counts and space are deterministic.
