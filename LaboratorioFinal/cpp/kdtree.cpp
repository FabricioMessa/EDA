#pragma once
#include <vector>
#include <cstddef>
#include <algorithm>
#include <limits>

struct KDNode {
    std::vector<double> point;
    int index;
    int split_dim;
    KDNode *left;
    KDNode *right;

    KDNode(const std::vector<double>& p, int idx, int dim)
        : point(p), index(idx), split_dim(dim), left(nullptr), right(nullptr) {}
};

class KDTree {
public:
    KDTree() : root(nullptr), dims(0) {}
    ~KDTree() { destroy(root); }

    void build(const std::vector<std::vector<double>>& points) {
        if (points.empty()) return;
        dims = points[0].size();
        std::vector<int> indices(points.size());
        for (size_t i = 0; i < points.size(); ++i) indices[i] = i;
        root = buildRecursive(points, indices, 0);
    }

    void radiusSearch(const std::vector<double>& query, double radius,
                      std::vector<int>& result) const {
        result.clear();
        radiusSearchRecursive(root, query, radius, result);
    }

private:
    KDNode* root;
    int dims;

    KDNode* buildRecursive(const std::vector<std::vector<double>>& points,
                           std::vector<int>& indices, int depth) {
        if (indices.empty()) return nullptr;

        int axis = depth % dims;
        size_t median = indices.size() / 2;

        std::nth_element(indices.begin(), indices.begin() + median, indices.end(),
            [&](int a, int b) { return points[a][axis] < points[b][axis]; });

        int mid_idx = indices[median];
        KDNode* node = new KDNode(points[mid_idx], mid_idx, axis);

        std::vector<int> left_indices(indices.begin(), indices.begin() + median);
        std::vector<int> right_indices(indices.begin() + median + 1, indices.end());

        node->left = buildRecursive(points, left_indices, depth + 1);
        node->right = buildRecursive(points, right_indices, depth + 1);

        return node;
    }

    void radiusSearchRecursive(KDNode* node, const std::vector<double>& query,
                                double radius, std::vector<int>& result) const {
        if (!node) return;

        double dist_sq = 0.0;
        for (int i = 0; i < dims; ++i) {
            double d = query[i] - node->point[i];
            dist_sq += d * d;
        }

        if (dist_sq <= radius * radius) {
            result.push_back(node->index);
        }

        int axis = node->split_dim;
        double diff = query[axis] - node->point[axis];

        KDNode* first = (diff <= 0) ? node->left : node->right;
        KDNode* second = (diff <= 0) ? node->right : node->left;

        radiusSearchRecursive(first, query, radius, result);

        if (std::abs(diff) <= radius || diff * diff <= radius * radius) {
            radiusSearchRecursive(second, query, radius, result);
        }
    }

    void destroy(KDNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
};
