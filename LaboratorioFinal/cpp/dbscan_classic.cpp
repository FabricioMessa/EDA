#include "dbscan.cpp"
#include <queue>

static std::vector<int> range_query_classic(const std::vector<Point>& points,
                                             const Point& q, double eps) {
    std::vector<int> neighbors;
    for (size_t i = 0; i < points.size(); ++i) {
        if (q.distance(points[i]) <= eps) {
            neighbors.push_back(static_cast<int>(i));
        }
    }
    return neighbors;
}

static void expand_cluster(std::vector<Point>& points, int pid,
                           const std::vector<int>& neighbors,
                           int cluster_id, double eps, int min_samples) {
    points[pid].cluster_id = cluster_id;
    std::vector<int> seeds = neighbors;

    size_t idx = 0;
    while (idx < seeds.size()) {
        int qid = seeds[idx];
        if (points[qid].cluster_id == -2) {
            points[qid].cluster_id = cluster_id;
            auto q_neighbors = range_query_classic(points, points[qid], eps);
            if (static_cast<int>(q_neighbors.size()) >= min_samples) {
                for (int nid : q_neighbors) {
                    if (points[nid].cluster_id == -2 || points[nid].cluster_id == 0) {
                        seeds.push_back(nid);
                    }
                }
            }
        } else if (points[qid].cluster_id == 0) {
            points[qid].cluster_id = cluster_id;
        }
        ++idx;
    }
}

int dbscan_classic(std::vector<Point>& points, double eps, int min_samples) {
    int cluster_count = 0;
    for (auto& p : points) p.cluster_id = -2;

    for (size_t i = 0; i < points.size(); ++i) {
        if (points[i].cluster_id != -2) continue;
        points[i].cluster_id = -2;
        auto neighbors = range_query_classic(points, points[i], eps);
        if (static_cast<int>(neighbors.size()) < min_samples) {
            points[i].cluster_id = 0;
        } else {
            ++cluster_count;
            expand_cluster(points, i, neighbors, cluster_count, eps, min_samples);
        }
    }
    return cluster_count;
}

int count_outliers(const std::vector<Point>& points) {
    int count = 0;
    for (const auto& p : points) {
        if (p.cluster_id == 0) ++count;
    }
    return count;
}
