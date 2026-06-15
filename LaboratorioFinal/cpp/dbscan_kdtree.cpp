#include "dbscan.cpp"
#include <queue>

static void expand_cluster_kdtree(std::vector<Point>& points, int pid,
                                   const std::vector<int>& neighbors,
                                   int cluster_id, double eps, int min_samples,
                                   const KDTree& tree) {
    points[pid].cluster_id = cluster_id;
    std::vector<int> seeds = neighbors;

    size_t idx = 0;
    std::vector<int> q_neighbors;
    while (idx < seeds.size()) {
        int qid = seeds[idx];
        if (points[qid].cluster_id == -2) {
            points[qid].cluster_id = cluster_id;
            tree.radiusSearch(points[qid].coords, eps, q_neighbors);
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

int dbscan_kdtree(std::vector<Point>& points, double eps, int min_samples) {
    for (auto& p : points) p.cluster_id = -2;

    std::vector<std::vector<double>> raw_points;
    raw_points.reserve(points.size());
    for (const auto& p : points) {
        raw_points.push_back(p.coords);
    }

    KDTree tree;
    tree.build(raw_points);

    int cluster_count = 0;
    std::vector<int> neighbors;

    for (size_t i = 0; i < points.size(); ++i) {
        if (points[i].cluster_id != -2) continue;
        tree.radiusSearch(points[i].coords, eps, neighbors);
        if (static_cast<int>(neighbors.size()) < min_samples) {
            points[i].cluster_id = 0;
        } else {
            ++cluster_count;
            expand_cluster_kdtree(points, i, neighbors, cluster_count, eps, min_samples, tree);
        }
    }
    return cluster_count;
}
