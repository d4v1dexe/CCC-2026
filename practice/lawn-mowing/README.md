# Lawn Mowing

**Contest:** Cloudflight Coding Contest - April 2024 (39th CCC)
**Language:** C++17

_Source: reused (MIT license) from [dpaguba/cloudflight-coding-contest](https://github.com/dpaguba/cloudflight-coding-contest)._

## Problem

You have a `W x H` grid. One cell is a tree (`'X'`); all others are grass.
Find a path - using moves `W` (up), `A` (left), `S` (down),
`D` (right) - that starts anywhere on a grass cell and visits **every**
grass cell exactly once. Output the sequence of direction characters.

This is the **Hamiltonian path problem** on a grid graph with one missing
vertex (the tree).

## Input Format

```
K                   <- number of lawns
W H                 <- width and height
<H rows of the grid>
...
```

## Output Format

```
WASDWASD...    <- one direction string per lawn
```

## Algorithm - DFS + Warnsdorff's Heuristic

### Why plain DFS fails

On a 10x10 grid with one obstacle there are ~9! valid orderings. Naive DFS
explores most of them before finding a solution - too slow.

### Warnsdorff's heuristic

Well known from the **Knight's Tour** problem. At each step, always
move to the neighbour with the **fewest onward moves** (smallest forward
degree). Intuitively: visit the most constrained cell next to avoid getting
stuck in a corner later.

```
degree(cell, visited) = number of unvisited, reachable neighbours of cell
```

At each DFS node, sort successor cells by their degree (ascending) and try
them in that order. This prunes the search tree dramatically.

### Starting position

The starting cell affects whether the heuristic finds a solution quickly.
The solver tries several candidates in order:

1. Corners of the grid (`(0,0)`, `(W-1,0)`, `(0,H-1)`, `(W-1,H-1)`)
2. The four cells immediately adjacent to the tree

The first starting cell that yields a complete path is used.

### Termination guarantee

A step limit prevents infinite loops on degenerate inputs. For typical
contest grid sizes (<= 20x20) the heuristic finds a solution in well under
10 000 steps from the right starting cell.

### Pseudocode

```
dfs(start):
    stack = [(start, {start}, [start])]
    while stack not empty:
        pos, visited, path = stack.pop()
        if |path| == |valid_cells|:
            return path           <- found!

        succs = neighbours(pos) \ visited
        sort succs by degree(succ, visited)  <- Warnsdorff
        for succ in reversed(succs):
            push (succ, visited union {succ}, path + [succ])
```

## Complexity

- Naive Hamiltonian path: `O(N!)` - infeasible for large grids
- With Warnsdorff: typically `O(N)` probes; finding a solution for
  an `N`-cell grid usually takes `O(N)` steps in practice

## Compile & Run

```bash
g++ -std=c++17 -O2 -o solution solution.cpp
./solution < input.in
```
