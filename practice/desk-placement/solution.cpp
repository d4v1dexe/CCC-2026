/*
 * Desk Placement  -  Cloudflight Coding Contest 2024 (October)
 * Reused (MIT license) from dpaguba/cloudflight-coding-contest.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct Room {
    int X, Y, target;
    std::vector<std::vector<int>> cell;

    Room(int x, int y, int n) : X(x), Y(y), target(n),
        cell(y, std::vector<int>(x, 0)) {}

    bool in_bounds(int r, int c) const {
        return r >= 0 && r < Y && c >= 0 && c < X;
    }

    void apply_exclusion(int r, int c, std::vector<std::pair<int,int>>& undo) {
        for (int dr = -1; dr <= 1; ++dr)
            for (int dc = -1; dc <= 1; ++dc) {
                int nr = r + dr, nc = c + dc;
                if (!in_bounds(nr, nc)) continue;
                if (cell[nr][nc] == 0) {
                    cell[nr][nc] = -1;
                    undo.push_back({nr, nc});
                }
            }
    }

    bool try_place(int r, int c, int dr, int dc,
                   int desks_placed,
                   std::vector<std::pair<int,int>>* solution) {

        int r2 = r + dr, c2 = c + dc;
        if (!in_bounds(r2, c2)) return false;
        if (cell[r][c] != 0 || cell[r2][c2] != 0) return false;

        cell[r][c]   = 1;
        cell[r2][c2] = 1;

        std::vector<std::pair<int,int>> undo;
        apply_exclusion(r,  c,  undo);
        apply_exclusion(r2, c2, undo);

        bool found = solve(r, c + 1, desks_placed + 1, solution);

        cell[r][c]   = 0;
        cell[r2][c2] = 0;
        for (auto [ur, uc] : undo) cell[ur][uc] = 0;

        return found;
    }

    int free_cells_from(int r, int c) const {
        int cnt = 0;
        for (int row = r; row < Y; ++row) {
            int col_start = (row == r) ? c : 0;
            for (int col = col_start; col < X; ++col)
                if (cell[row][col] == 0) cnt++;
        }
        return cnt;
    }

    bool solve(int r, int c, int desks_placed,
               std::vector<std::pair<int,int>>* solution) {

        while (c >= X) { c -= X; r++; }

        if (desks_placed == target) {
            if (solution) {
                solution->clear();
                for (int row = 0; row < Y; ++row)
                    for (int col = 0; col < X; ++col)
                        if (cell[row][col] == 1)
                            solution->push_back({row, col});
            }
            return true;
        }

        if (r >= Y) return false;

        int remaining = target - desks_placed;
        if (free_cells_from(r, c) / 2 < remaining) return false;

        if (cell[r][c] != 0)
            return solve(r, c + 1, desks_placed, solution);

        if (try_place(r, c, 0, 1, desks_placed, solution)) return true;
        if (try_place(r, c, 1, 0, desks_placed, solution)) return true;
        return solve(r, c + 1, desks_placed, solution);
    }

    bool find_placement(std::vector<std::pair<int,int>>& sol) {
        return solve(0, 0, 0, &sol);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int k;
    std::cin >> k;

    while (k--) {
        int x, y, n;
        std::cin >> x >> y >> n;

        Room room(x, y, n);
        std::vector<std::pair<int,int>> sol;
        bool ok = room.find_placement(sol);

        std::vector<std::string> out(y, std::string(x, '.'));
        if (ok) {
            for (auto [r, c] : sol)
                out[r][c] = 'X';
        }

        for (auto& row : out) std::cout << row << "\n";
        std::cout << "\n";
    }

    return 0;
}
