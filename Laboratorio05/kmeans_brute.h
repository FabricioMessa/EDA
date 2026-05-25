#pragma once
#include "kdtree.h"
#include <vector>
#include <random>
#include <limits>
#include <numeric>

struct ResultadoKM {
    std::vector<Punto> centroides;
    std::vector<int> asignaciones;
    double inerciaTotal;
    int iteraciones;
};

inline std::vector<Punto> initcentroides(const std::vector<Punto>& data, int k, std::mt19937& rng) {
    std::vector<int> indices(data.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), rng);
    std::vector<Punto> centroides(k);
    
    for (int i = 0; i < k; i++) {
        centroides[i] = data[indices[i]];
    }
    
    return centroides;
}

inline ResultadoKM kMeansBrute(const std::vector<Punto>& data, int k, int maxIter, std::mt19937& rng) {
    int n = (int)data.size();
    std::vector<Punto> centroides = initcentroides(data, k, rng);
    std::vector<int> asignaciones(n, 0);
    int iter = 0;

    for (iter = 0; iter < maxIter; iter++) {
		bool changed = false;
        for (int i = 0; i < n; i++) {
            double mejorDistancia = std::numeric_limits<double>::max();
            int mejorK = 0;
            for (int j = 0; j < k; j++) {
                double d = DistanciaEucladiana(data[i], centroides[j]);
                if (d < mejorDistancia) {
                    mejorDistancia = d;
                    mejorK = j;
                }
            }

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
