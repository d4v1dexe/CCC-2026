/*
 * Island Routing  -  Cloudflight Coding Contest 2023 (October)
 * Reused (MIT license) from dpaguba/cloudflight-coding-contest.
 */

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using Pt = std::pair<int,int>;

static int N;
static std::vector<std::string> grid;

static bool in_bounds(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}

static std::vector<Pt> neighbours4(int x, int y, char avoid) {
    std::vector<Pt> res;
    for (auto [dx, dy] : std::vector<Pt>{{-1,0},{1,0},{0,-1},{0,1}}) {
        int nx = x + dx, ny = y + dy;
        if (in_bounds(nx, ny) && grid[ny][nx] != avoid)
            res.push_back({nx, ny});
    }
    return res;
}

static std::vector<Pt> neighbours8_water(int x, int y) {
    std::vector<Pt> res;
    for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (in_bounds(nx, ny) && grid[ny][nx] != 'L')
                res.push_back({nx, ny});
        }
    return res;
}

static std::set<Pt> discover_island(int x, int y) {
    if (!in_bounds(x, y) || grid[y][x] != 'L') return {};

    std::set<Pt> island;
    std::queue<Pt> q;
    q.push({x, y});
    island.insert({x, y});

    while (!q.empty()) {
        auto [cx, cy] = q.front(); q.pop();
        for (auto nb : neighbours4(cx, cy, 'W')) {
            if (!island.count(nb)) {
                island.insert(nb);
                q.push(nb);
            }
        }
    }
    return island;
}

static std::set<Pt> get_fringe(const std::set<Pt>& island) {
    std::set<Pt> fringe;
    for (auto [x, y] : island) {
        for (auto [dx, dy] : std::vector<Pt>{{-1,0},{1,0},{0,-1},{0,1}}) {
            int nx = x + dx, ny = y + dy;
            if (in_bounds(nx, ny) && grid[ny][nx] == 'W' && !island.count({nx, ny}))
                fringe.insert({nx, ny});
        }
    }
    return fringe;
}

static double chebyshev(Pt a, Pt b) {
    return std::max(std::abs(a.first - b.first), std::abs(a.second - b.second));
}

static std::vector<Pt> astar(Pt start, Pt goal) {
    using Entry = std::tuple<double, int, Pt>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;
    std::map<Pt, double> cost;
    std::map<Pt, Pt> parent;
    int tie = 0;

    pq.push({0, tie++, start});
    cost[start] = 0;
    parent[start] = {-1, -1};

    while (!pq.empty()) {
        auto [f, _, cur] = pq.top(); pq.pop();

        if (cur == goal) {
            std::vector<Pt> path;
            Pt c = goal;
            while (c != Pt{-1,-1}) {
                path.push_back(c);
                c = parent[c];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (auto nb : neighbours8_water(cur.first, cur.second)) {
            double new_cost = cost[cur] + 1;
            if (!cost.count(nb) || new_cost < cost[nb]) {
                cost[nb] = new_cost;
                parent[nb] = cur;
                pq.push({new_cost + chebyshev(nb, goal), tie++, nb});
            }
        }
    }
    return {};
}

static double cross(Pt O, Pt A, Pt B) {
    return (double)(A.first - O.first) * (B.second - O.second)
         - (double)(A.second - O.second) * (B.first - O.first);
}

static std::vector<Pt> convex_hull(std::vector<Pt> pts) {
    int n = pts.size();
    if (n < 3) return pts;
    std::sort(pts.begin(), pts.end());
    std::vector<Pt> hull;

    for (auto& p : pts) {
        while (hull.size() >= 2 && cross(hull[hull.size()-2], hull.back(), p) <= 0)
            hull.pop_back();
        hull.push_back(p);
    }
    int lower_size = hull.size();
    for (int i = n - 2; i >= 0; --i) {
        while ((int)hull.size() > lower_size && cross(hull[hull.size()-2], hull.back(), pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }
    hull.pop_back();
    return hull;
}

static std::vector<Pt> build_route(const std::set<Pt>& fringe) {
    if (fringe.empty()) return {};
    if (fringe.size() == 1) return {*fringe.begin()};

    std::vector<Pt> pts(fringe.begin(), fringe.end());

    std::vector<Pt> hull = convex_hull(pts);
    if (hull.empty()) hull = pts;

    std::vector<Pt> route;
    for (int i = 0; i < (int)hull.size(); ++i) {
        Pt from = hull[i];
        Pt to   = hull[(i + 1) % hull.size()];

        std::vector<Pt> segment = astar(from, to);
        if (segment.empty()) {
            if (route.empty() || route.back() != from) route.push_back(from);
            continue;
        }

        for (int j = (route.empty() ? 0 : 1); j < (int)segment.size(); ++j)
            route.push_back(segment[j]);
    }

    if (route.size() > 1 && route.front() == route.back())
        route.pop_back();

    return route;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> N;
    grid.resize(N);
    for (int i = 0; i < N; ++i) std::cin >> grid[i];

    int q;
    std::cin >> q;

    while (q--) {
        std::string coord;
        std::cin >> coord;

        int comma = coord.find(',');
        int x = std::stoi(coord.substr(0, comma));
        int y = std::stoi(coord.substr(comma + 1));

        std::set<Pt> island = discover_island(x, y);
        std::set<Pt> fringe = get_fringe(island);

        std::vector<Pt> route = build_route(fringe);

        std::string out;
        for (size_t i = 0; i < route.size(); ++i) {
            if (i) out += ' ';
            out += std::to_string(route[i].first) + "," + std::to_string(route[i].second);
        }
        std::cout << out << "\n";
    }

    return 0;
}
