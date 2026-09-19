# Desk Placement

**Contest:** Cloudflight Coding Contest - October 2024 (40th CCC)
**Language:** C++17

_Source: reused (MIT license) from [dpaguba/cloudflight-coding-contest](https://github.com/dpaguba/cloudflight-coding-contest)._

## Problem

Place exactly `N` desks inside a room of `X` columns x `Y` rows.

Rules:
- Each desk occupies **2 consecutive cells** - either horizontally (left-right)
  or vertically (up-down).
- Two desks must **not touch**, not even diagonally.
- Output the room grid with `'.'` for empty cells and `'X'` for desk cells.

## Input Format

```
K               <- number of rooms
X Y N           <- width, height, number of desks to place
...
```

## Output Format

```
<Y rows of X characters>    <- '.' or 'X'
                             <- blank line after each room
```

## Algorithm - Backtracking DFS

### Representation

The room is a 2-D integer grid:
```
 0  = free cell
 1  = desk cell
-1  = blocked (inside the exclusion zone of an adjacent desk)
```

### Exclusion zone

When a desk is placed on cells `A` and `B`, all 8 immediate neighbours of
`A` and all 8 immediate neighbours of `B` are marked as blocked (`-1`).
This automatically enforces the "no touching, not even diagonally" rule.

### Search

Cells are scanned in **row-major order** (left to right, top to bottom).
At each free (`0`) cell we try three options:

1. **Place a horizontal desk** - covers `(r,c)` and `(r, c+1)`, applies
   exclusion zone, recurse.
2. **Place a vertical desk** - covers `(r,c)` and `(r+1, c)`, applies
   exclusion zone, recurse.
3. **Skip** - leave `(r,c)` empty, advance to next cell.

After recursion we **undo** the exclusion zone (backtrack).

### Pruning

Before recursing, check:
```
free_cells_remaining / 2 < desks_still_needed
```
If true, it is physically impossible to fit the remaining desks, so prune
the branch immediately. This single check eliminates the vast majority of
dead ends.

### Why row-major scanning works

By committing to the "first free cell determines desk origin" convention,
each placement is tried exactly once. Symmetry-breaking via fixed scan order
makes the search tree much smaller than without it.

### Example (4x3 room, 3 desks)

```
Initial:          After placing 3 desks:
....              XX.X
....              ....
....              XX.X

Desk 1: (0,0)-(0,1) horizontal  -> exclusion blocks row 1 cols 0-2
Desk 2: (0,3)-(1,3) vertical    -> exclusion blocks neighbours
Desk 3: (2,0)-(2,1) horizontal  -> fits without touching either desk
```

## Complexity

- Worst case: exponential in `X x Y` (true for any Hamiltonian-style problem)
- With pruning: fast for typical contest inputs (rooms up to ~20x20)
- The exclusion zone means each placed desk eliminates ~10 cells from future
  choices, making the effective branching factor small

## Compile & Run

```bash
g++ -std=c++17 -O2 -o solution solution.cpp
./solution < input.in
```
