/*
 * Social Network Graph: Adjacency Matrix vs Adjacency List
 * - BFS and DFS from vertex A
 * - Vertex search and edge checking, with operation counts
 * - Space and timing comparison (small network + larger sparse network)
 *
 * Compile: gcc -O0 -o graph "source code.c"
 * Run    : ./graph        (reads "input data.txt" from current folder)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node {
    int v;
    struct Node *next;
} Node;

typedef struct {
    int n;
    char *m;        /* adjacency matrix (n x n, 1 byte per cell) */
    Node **adj;     /* adjacency list (array of sorted linked lists) */
    char *lab;      /* vertex labels (small graph only) */
    int edges;
} Graph;

static long long ops;       /* operation counter (cells / nodes examined) */
static int found_flag;
static volatile int sink;

/* ---------- construction ---------- */
Graph *make_graph(int n) {
    Graph *g = malloc(sizeof(Graph));
    g->n = n;
    g->m = calloc((size_t)n * n, 1);
    g->adj = calloc(n, sizeof(Node *));
    g->lab = NULL;
    g->edges = 0;
    return g;
}

static void insert_sorted(Node **head, int v) {
    Node *x = malloc(sizeof(Node));
    x->v = v;
    while (*head && (*head)->v < v) head = &(*head)->next;
    x->next = *head;
    *head = x;
}

void add_edge(Graph *g, int u, int v) {
    if (u == v || g->m[u * g->n + v]) return;
    g->m[u * g->n + v] = g->m[v * g->n + u] = 1;
    insert_sorted(&g->adj[u], v);
    insert_sorted(&g->adj[v], u);
    g->edges++;
}

void free_graph(Graph *g) {
    for (int i = 0; i < g->n; i++) {
        Node *p = g->adj[i];
        while (p) { Node *t = p->next; free(p); p = t; }
    }
    free(g->adj); free(g->m); free(g->lab); free(g);
}

static void show(Graph *g, int v, int verbose) {
    if (verbose) printf("%c ", g->lab[v]);
}

/* ---------- BFS (target = -1 means full traversal) ---------- */
int bfs_matrix(Graph *g, int s, int target, int verbose) {
    int n = g->n, head = 0, tail = 0, cnt = 1;
    char *vis = calloc(n, 1);
    int *q = malloc(n * sizeof(int));
    found_flag = 0;
    vis[s] = 1; q[tail++] = s; show(g, s, verbose);
    if (s == target) { found_flag = 1; goto done; }
    while (head < tail) {
        int u = q[head++];
        for (int v = 0; v < n; v++) {
            ops++;
            if (g->m[u * n + v] && !vis[v]) {
                vis[v] = 1; q[tail++] = v; cnt++; show(g, v, verbose);
                if (v == target) { found_flag = 1; goto done; }
            }
        }
    }
done:
    free(vis); free(q);
    return cnt;
}

int bfs_list(Graph *g, int s, int target, int verbose) {
    int n = g->n, head = 0, tail = 0, cnt = 1;
    char *vis = calloc(n, 1);
    int *q = malloc(n * sizeof(int));
    found_flag = 0;
    vis[s] = 1; q[tail++] = s; show(g, s, verbose);
    if (s == target) { found_flag = 1; goto done; }
    while (head < tail) {
        int u = q[head++];
        for (Node *p = g->adj[u]; p; p = p->next) {
            ops++;
            if (!vis[p->v]) {
                vis[p->v] = 1; q[tail++] = p->v; cnt++; show(g, p->v, verbose);
                if (p->v == target) { found_flag = 1; goto done; }
            }
        }
    }
done:
    free(vis); free(q);
    return cnt;
}

/* ---------- DFS (recursive) ---------- */
static void dfs_m_rec(Graph *g, int u, char *vis, int target, int verbose, int *cnt) {
    vis[u] = 1; (*cnt)++; show(g, u, verbose);
    if (u == target) { found_flag = 1; return; }
    for (int v = 0; v < g->n && !found_flag; v++) {
        ops++;
        if (g->m[u * g->n + v] && !vis[v]) dfs_m_rec(g, v, vis, target, verbose, cnt);
    }
}

static void dfs_l_rec(Graph *g, int u, char *vis, int target, int verbose, int *cnt) {
    vis[u] = 1; (*cnt)++; show(g, u, verbose);
    if (u == target) { found_flag = 1; return; }
    for (Node *p = g->adj[u]; p && !found_flag; p = p->next) {
        ops++;
        if (!vis[p->v]) dfs_l_rec(g, p->v, vis, target, verbose, cnt);
    }
}

int dfs_matrix(Graph *g, int s, int target, int verbose) {
    char *vis = calloc(g->n, 1); int cnt = 0; found_flag = 0;
    dfs_m_rec(g, s, vis, target, verbose, &cnt);
    free(vis); return cnt;
}

int dfs_list(Graph *g, int s, int target, int verbose) {
    char *vis = calloc(g->n, 1); int cnt = 0; found_flag = 0;
    dfs_l_rec(g, s, vis, target, verbose, &cnt);
    free(vis); return cnt;
}

/* ---------- edge checking ---------- */
int edge_matrix(Graph *g, int u, int v) { ops++; return g->m[u * g->n + v]; }

int edge_list(Graph *g, int u, int v) {
    for (Node *p = g->adj[u]; p; p = p->next) {
        ops++;
        if (p->v == v) return 1;
    }
    return 0;
}

/* ---------- vertex label lookup ---------- */
int find_label(Graph *g, char c) {
    for (int i = 0; i < g->n; i++) {
        ops++;
        if (g->lab[i] == c) return i;
    }
    return -1;
}

/* ---------- space ---------- */
size_t bytes_matrix(Graph *g) { return (size_t)g->n * g->n * sizeof(char); }
size_t bytes_list(Graph *g) {
    return g->n * sizeof(Node *) + (size_t)2 * g->edges * sizeof(Node);
}

/* ---------- timing helpers ---------- */
double time_bfs(Graph *g, int use_list, int reps) {
    clock_t t = clock();
    for (int i = 0; i < reps; i++)
        sink += use_list ? bfs_list(g, 0, -1, 0) : bfs_matrix(g, 0, -1, 0);
    return (double)(clock() - t) / CLOCKS_PER_SEC * 1e6 / reps;   /* microseconds per run */
}

double time_dfs(Graph *g, int use_list, int reps) {
    clock_t t = clock();
    for (int i = 0; i < reps; i++)
        sink += use_list ? dfs_list(g, 0, -1, 0) : dfs_matrix(g, 0, -1, 0);
    return (double)(clock() - t) / CLOCKS_PER_SEC * 1e6 / reps;
}

int main(void) {
    FILE *f = fopen("input data.txt", "r");
    if (!f) { printf("Cannot open input data.txt\n"); return 1; }

    int n, m;
    fscanf(f, "%d", &n);
    Graph *g = make_graph(n);
    g->lab = malloc(n + 1);
    for (int i = 0; i < n; i++) fscanf(f, " %c", &g->lab[i]);
    fscanf(f, "%d", &m);
    for (int i = 0; i < m; i++) {
        char a, b;
        fscanf(f, " %c %c", &a, &b);
        add_edge(g, find_label(g, a), find_label(g, b));
    }
    char start; fscanf(f, " %c", &start);
    int k; fscanf(f, "%d", &k);
    char targets[10];
    for (int i = 0; i < k; i++) fscanf(f, " %c", &targets[i]);
    fclose(f);
    int s = find_label(g, start);

    printf("=== SMALL SOCIAL NETWORK: %d vertices, %d edges ===\n\n", g->n, g->edges);

    /* a) representations */
    printf("--- Adjacency Matrix ---\n   ");
    for (int j = 0; j < n; j++) printf("%c ", g->lab[j]);
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%c  ", g->lab[i]);
        for (int j = 0; j < n; j++) printf("%d ", g->m[i * n + j]);
        printf("\n");
    }
    printf("\n--- Adjacency List ---\n");
    for (int i = 0; i < n; i++) {
        printf("%c -> ", g->lab[i]);
        for (Node *p = g->adj[i]; p; p = p->next) printf("%c ", g->lab[p->v]);
        printf("\n");
    }

    printf("\n--- BFS from %c ---\n", start);
    printf("Matrix: "); ops = 0; bfs_matrix(g, s, -1, 1); printf("  [operations = %lld]\n", ops);
    printf("List  : "); ops = 0; bfs_list(g, s, -1, 1);   printf("  [operations = %lld]\n", ops);

    printf("\n--- DFS from %c ---\n", start);
    printf("Matrix: "); ops = 0; dfs_matrix(g, s, -1, 1); printf("  [operations = %lld]\n", ops);
    printf("List  : "); ops = 0; dfs_list(g, s, -1, 1);   printf("  [operations = %lld]\n", ops);

    /* b) search */
    printf("\n--- b) Vertex search (traversal-based, starting at %c) ---\n", start);
    for (int i = 0; i < k; i++) {
        int t = find_label(g, targets[i]);   /* -1 if label does not exist */
        printf("Search '%c':\n", targets[i]);
        ops = 0; bfs_matrix(g, s, t, 0);
        printf("  BFS Matrix: %s, operations = %lld\n", found_flag ? "FOUND" : "NOT FOUND", ops);
        ops = 0; bfs_list(g, s, t, 0);
        printf("  BFS List  : %s, operations = %lld\n", found_flag ? "FOUND" : "NOT FOUND", ops);
        ops = 0; dfs_matrix(g, s, t, 0);
        printf("  DFS Matrix: %s, operations = %lld\n", found_flag ? "FOUND" : "NOT FOUND", ops);
        ops = 0; dfs_list(g, s, t, 0);
        printf("  DFS List  : %s, operations = %lld\n", found_flag ? "FOUND" : "NOT FOUND", ops);
    }

    printf("\n--- Edge-checking operations ---\n");
    char pairs[4][2] = {{'A','B'}, {'A','F'}, {'E','F'}, {'D','F'}};
    for (int i = 0; i < 4; i++) {
        int u = find_label(g, pairs[i][0]), v = find_label(g, pairs[i][1]);
        ops = 0; int r1 = edge_matrix(g, u, v); long long o1 = ops;
        ops = 0; int r2 = edge_list(g, u, v);   long long o2 = ops;
        printf("Edge %c-%c: Matrix=%s (%lld op) | List=%s (%lld op)\n",
               pairs[i][0], pairs[i][1], r1 ? "yes" : "no", o1, r2 ? "yes" : "no", o2);
    }

    /* c) space + timing */
    printf("\n--- c) Space (bytes) ---\n");
    printf("Matrix: %zu bytes (n*n*1)\n", bytes_matrix(g));
    printf("List  : %zu bytes (n pointers + 2E nodes)\n", bytes_list(g));

    printf("\n--- Timing, small network (average of 200000 runs, microseconds) ---\n");
    printf("BFS Matrix: %.4f us | BFS List: %.4f us\n", time_bfs(g, 0, 200000), time_bfs(g, 1, 200000));
    printf("DFS Matrix: %.4f us | DFS List: %.4f us\n", time_dfs(g, 0, 200000), time_dfs(g, 1, 200000));
    free_graph(g);

    /* larger sparse network to see how the representations scale */
    int N = 1000;
    Graph *big = make_graph(N);
    for (int i = 0; i < N; i++) {
        add_edge(big, i, (i + 1) % N);
        add_edge(big, i, (i + 7) % N);
    }
    double density = 100.0 * big->edges / ((double)N * (N - 1) / 2);
    printf("\n=== LARGER SPARSE NETWORK: %d vertices, %d edges (density %.2f%%) ===\n",
           big->n, big->edges, density);

    ops = 0; bfs_matrix(big, 0, -1, 0); long long bm = ops;
    ops = 0; bfs_list(big, 0, -1, 0);   long long bl = ops;
    ops = 0; dfs_matrix(big, 0, -1, 0); long long dm = ops;
    ops = 0; dfs_list(big, 0, -1, 0);   long long dl = ops;
    printf("BFS operations: Matrix=%lld | List=%lld\n", bm, bl);
    printf("DFS operations: Matrix=%lld | List=%lld\n", dm, dl);

    ops = 0; edge_matrix(big, 0, 500); long long em = ops;
    ops = 0; edge_list(big, 0, 500);   long long el = ops;
    printf("Edge check 0-500 (absent): Matrix=%lld op | List=%lld op\n", em, el);

    printf("Space: Matrix=%zu bytes | List=%zu bytes\n", bytes_matrix(big), bytes_list(big));
    printf("Timing BFS (avg of 2000 runs, us): Matrix=%.2f | List=%.2f\n",
           time_bfs(big, 0, 2000), time_bfs(big, 1, 2000));
    printf("Timing DFS (avg of 2000 runs, us): Matrix=%.2f | List=%.2f\n",
           time_dfs(big, 0, 2000), time_dfs(big, 1, 2000));

    free_graph(big);
    return 0;
}
