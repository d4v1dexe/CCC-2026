/*
 * Lawn Mowing - Cloudflight Coding Contest 2024 (April)
 * Reused (MIT license) from dpaguba/cloudflight-coding-contest.
 */

#include <algorithm>
#include <complex>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using Pos = std::complex<int>;

struct PosHash {
    size_t operator()(Pos p) const {
        size_t h = std::hash<int>{}(p.real());
        h ^= std::hash<int>{}(p.imag()) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

using PosSet = std::unordered_set<Pos, PosHash>;

static Pos move_to(Pos p, char d) {
    switch (d) {
        case 'W': return p + Pos{0, -1};
        case 'S': return p + Pos{0,  1};
        case 'A': return p + Pos{-1, 0};
        case 'D': return p + Pos{ 1, 0};
    }
    return p;
}

static char pos_to_dir(Pos from, Pos to) {
    Pos diff = to - from;
    if (diff == Pos{ 1, 0}) return 'D';
    if (diff == Pos{-1, 0}) return 'A';
    if (diff == Pos{ 0, 1}) return 'S';
    if (diff == Pos{ 0,-1}) return 'W';
    return '?';
}

struct Lawn {
    int W, H;
    PosSet blocked;
    PosSet valid;

    Lawn(int w, int h, const std::vector<std::string>& grid) : W(w), H(h) {
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x) {
                Pos p{x, y};
                if (grid[y][x] == 'X') blocked.insert(p);
                else valid.insert(p);
            }
    }

    bool in_bounds(Pos p) const {
        return p.real() >= 0 && p.real() < W && p.imag() >= 0 && p.imag() < H;
    }

    std::vector<Pos> successors(Pos p, const PosSet& visited) const {
        std::vector<Pos> res;
        for (char d : {'W', 'A', 'S', 'D'}) {
            Pos np = move_to(p, d);
            if (in_bounds(np) && !blocked.count(np) && !visited.count(np))
                res.push_back(np);
        }
        return res;
    }

    int degree(Pos np, const PosSet& visited) const {
        int cnt = 0;
        for (char d : {'W', 'A', 'S', 'D'}) {
            Pos nnp = move_to(np, d);
            if (in_bounds(nnp) && !blocked.count(nnp) && !visited.count(nnp))
                cnt++;
        }
        return cnt;
    }

    bool dfs(Pos pos, PosSet& visited, std::vector<Pos>& path) const {
        if (path.size() == valid.size()) return true;

        auto succs = successors(pos, visited);
        std::sort(succs.begin(), succs.end(), [&](Pos a, Pos b) {
            return degree(a, visited) < degree(b, visited);
        });

        for (Pos np : succs) {
            visited.insert(np);
            path.push_back(np);

            if (dfs(np, visited, path)) return true;

            path.pop_back();
            visited.erase(np);
        }
        return false;
    }

    std::string find_path() const {
        Pos tree_pos = *blocked.begin();

        std::vector<Pos> starts = {
            Pos{0, 0}, Pos{W-1, 0}, Pos{0, H-1}, Pos{W-1, H-1},
            tree_pos + Pos{0, 1}, tree_pos + Pos{0, -1},
            tree_pos + Pos{1, 0}, tree_pos + Pos{-1, 0},
        };

        for (Pos start : starts) {
            if (!in_bounds(start) || blocked.count(start)) continue;

            PosSet visited = {start};
            std::vector<Pos> path = {start};

            if (dfs(start, visited, path)) {
                std::string dirs;
                for (int i = 1; i < (int)path.size(); ++i)
                    dirs += pos_to_dir(path[i-1], path[i]);
                return dirs;
            }
        }
        return "";
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int k;
    std::cin >> k;

    while (k--) {
        int w, h;
        std::cin >> w >> h;

        std::vector<std::string> g(h);
        for (int i = 0; i < h; ++i) std::cin >> g[i];

        Lawn lawn(w, h, g);
        std::cout << lawn.find_path() << "\n";
    }

    return 0;
}
