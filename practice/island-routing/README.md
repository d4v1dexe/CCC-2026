# Island Routing

**Contest:** Cloudflight Coding Contest - October 2023 (38th CCC)
**Language:** C++17 (stdlib only)

_Source: reused (MIT license) from [dpaguba/cloudflight-coding-contest](https://github.com/dpaguba/cloudflight-coding-contest)._

## Problem

You have an `N x N` grid where each cell is either `'W'` (water) or `'L'`
(land). For each query coordinate `(x, y)`:

1. **Discover the island** - all `'L'` cells reachable from `(x, y)` via
   4-directional adjacency.
2. **Find the fringe** - all `'W'` cells that are directly adjacent
   (4-directionally) to any cell of the island. This is the "shore" that a
   ship sails along.
3. **Produce a valid closed route** - order the fringe cells into a sequence
   where consecutive cells are 8-connected (diagonal moves allowed on water),
   the route is non-self-intersecting, and the last cell is an 8-neighbour
   of the first.

## Input Format

```
N
<N rows of the grid>
Q
x,y       (0-based; x = column, y = row)
...
```

## Output Format

```
x1,y1 x2,y2 x3,y3 ...    (fringe cells in route order, one island per line)
```

## Algorithm

### Step 1 - Island Discovery (BFS)

Standard flood-fill from the query cell, moving only through `'L'` cells
using 4-connectivity. Produces the complete connected component.

```
visited = {start}
queue   = [start]
while queue not empty:
    cell = dequeue
    for each 4-neighbour nb of cell:
        if nb is 'L' and not visited:
            visit nb, enqueue
```

### Step 2 - Fringe Extraction

For every land cell, check its 4 neighbours. Any water cell adjacent to at
least one land cell belongs to the fringe.

```
fringe = {}
for each land_cell in island:
    for each 4-neighbour nb of land_cell:
        if nb is 'W':
            fringe.add(nb)
```

### Step 3 - Route Construction (Convex Hull + A*)

Naively visiting fringe cells in arbitrary order produces a route that
self-intersects. The approach:

1. **Convex hull** of the fringe points (Graham scan, O(n log n)). Hull
   vertices form a polygon that naturally avoids crossings.

2. **A\* pathfinding** between consecutive hull vertices on the 8-connected
   water graph. This inserts any non-hull fringe cells that lie "between"
   two hull vertices along the shore.

3. The A\* segments are concatenated; together they form a closed loop since
   we connect hull[last] back to hull[0].

### Why A* and not greedy nearest-neighbour?

Greedy nearest-neighbour on the fringe tends to jump across the island,
creating crossings. A* restricted to the water-cell graph follows the
actual coastline, which is naturally non-self-intersecting.

### Intersection detection

The C++ version relies on the A*-on-water guarantee: since every step moves
through a connected water region around the island, the route stays on one
side of the island and cannot cross itself.

## Complexity

- Island discovery: `O(N^2)` in the worst case
- Fringe extraction: `O(|island|)`
- Convex hull: `O(F log F)` where `F = |fringe|`
- A* per segment: `O(N^2 log N^2)` worst case; fast in practice on small fringes
- Total per query: `O(N^2 log N)`

## Compile & Run

```bash
g++ -std=c++17 -O2 -o solution solution.cpp
./solution < input.in
```
