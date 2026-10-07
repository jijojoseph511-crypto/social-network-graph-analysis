# Final Conclusion

**The adjacency list is the more suitable representation for a sparse social network.**

## Justification from the execution results
1. **Traversal cost.** On the 6-person network, BFS/DFS needed 36 operations with the matrix but only 12 with the list (V² vs 2E). On the 1000-person sparse network the gap grew to 1,000,000 vs 4,000 operations, and runtime to about 3025 µs vs 19 µs for BFS (roughly 150× faster).
2. **Search cost.** Searching for an absent vertex costs a full traversal: 36 vs 12 operations on the small graph. The list does proportionally less work as the graph gets sparser.
3. **Space.** On the sparse 1000-vertex network the matrix needed 1,000,000 bytes against 72,000 for the list, since the matrix stores V² cells even though only 0.40% of possible friendships exist. On the tiny 6-vertex graph the list was actually larger (240 vs 36 bytes) because each list node carries a pointer; the O(V+E) advantage appears as V grows.
4. **Edge checking.** The matrix wins here: always 1 operation versus up to the vertex's degree for the list. In a social network degrees are small, so the list's extra cost is minor (at most 4 operations in the sparse test).

## Verdict
Real social networks have many users and few connections per user (E is close to V, not V²). The list gives O(V+E) space and traversal, which matches that structure, at the cost of slightly slower edge lookups. The matrix is preferable only for small or dense graphs where constant-time edge queries dominate.
