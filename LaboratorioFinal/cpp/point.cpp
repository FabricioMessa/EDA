#pragma once
#include <vector>
#include <cmath>
#include <cstddef>

struct Point {
    std::vector<double> coords;
    int cluster_id = -2;

    double distance(const Point& other) const {
        double sum = 0.0;
        for (size_t i = 0; i < coords.size(); ++i) {
            double d = coords[i] - other.coords[i];
            sum += d * d;
        }
        return std::sqrt(sum);
    }
};
