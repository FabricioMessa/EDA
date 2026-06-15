#pragma once
#include "point.cpp"
#include "kdtree.cpp"

int dbscan_classic(std::vector<Point>& points, double eps, int min_samples);
int dbscan_kdtree(std::vector<Point>& points, double eps, int min_samples);
int count_outliers(const std::vector<Point>& points);
