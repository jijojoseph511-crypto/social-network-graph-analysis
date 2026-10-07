# Social Network Graph: Adjacency Matrix vs Adjacency List

Implementation and comparison of two graph representations in C for a small social network.

**Network:** vertices A–F, edges A-B, A-C, B-D, B-E, C-F, E-F

## Files
| File | Purpose |
|---|---|
| `source code.c` | C implementation (matrix, list, BFS, DFS, search, edge check, timing) |
| `input data.txt` | Vertices, edges, start vertex, search targets |
| `output.txt` | Actual program output |
| `complexity_analysis.md` | Time and space complexity |
| `comparison_table.md` | Side-by-side comparison using measured results |
| `final_conclusion.md` | Which representation suits a sparse social network |

## Build and run
```bash
gcc -O0 -o graph "source code.c"
./graph
```
Run from the folder containing `input data.txt`.

## What the program does
- **(a)** Builds the graph as an adjacency matrix and an adjacency list; runs BFS and DFS from A.
- **(b)** Searches for vertices (`F` present, `Z` absent) using both representations and counts operations. Also counts edge-check operations.
- **(c)** Reports space, operation counts and timings, on the 6-vertex network and on a 1000-vertex sparse network (2000 edges) to show scaling.

## Input format
```
n
<n vertex labels>
m
<m edges, one per line>
<start vertex>
k
<k search targets>
```
