#pragma once
#include "kdtree.h"
#include "kmeans_brute.h"
#include <vector>
#include <random>
#include <limits>

inline ResultadoKM ArbolKDkMeans(const std::vector<Punto>& data, int k, int maxIter, std::mt19937& rng) {
    int n = (int)data.size();
    std::vector<Punto> centroides = initcentroides(data, k, rng);
    std::vector<int> asignaciones(n, 0);
    ArbolKD kdtree;
    int iter = 0;

    for (iter = 0; iter < maxIter; iter++) {
		kdtree.build(centroides);
		bool changed = false;
        for (int i = 0; i < n; i++) {
            int mejorK = kdtree.VecinoCercano(data[i]);

            if (asignaciones[i] != mejorK) {
                asignaciones[i] = mejorK;
                changed = true;
            }
        }

        std::vector<Punto> newcentroides(k, {0.0, 0.0});
        std::vector<int> counts(k, 0);

        for (int i = 0; i < n; i++) {
            int c = asignaciones[i];
            newcentroides[c].x += data[i].x;
            newcentroides[c].y += data[i].y;
            counts[c]++;
        }

        for (int j = 0; j < k; j++) {
            if (counts[j] > 0) {
                newcentroides[j].x /= counts[j];
                newcentroides[j].y /= counts[j];
            }

            else {
                newcentroides[j] = data[std::uniform_int_distribution<>(0, n-1)(rng)];
            }
        }

        centroides = newcentroides;

        if (!changed) {
        	break;
       	}
    }

    double inerciaTotal = 0.0;

    for (int i = 0; i < n; i++) {
        inerciaTotal += DistanciaEucladiana(data[i], centroides[asignaciones[i]]);
	}
	
    return {centroides, asignaciones, inerciaTotal, iter + 1};
}
