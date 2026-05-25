#include "kmeans_brute.h"
#include "kmeans_kdtree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace std;
using Clock = chrono::high_resolution_clock;
using Ms = chrono::duration<double, milli>;

inline double elapsed(Clock::time_point start) {
    return Ms(Clock::now() - start).count();
}

vector<Punto> cargarArchivo(const string& filename) {
    vector<Punto> data;
    ifstream file(filename);

    if (!file.is_open()) {
        return data;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        replace(line.begin(), line.end(), ',', ' ');
        istringstream ss(line);
        Punto p;
        if (ss >> p.x >> p.y) {
            data.push_back(p);
        }
    }
    
    return data;
}

void experimento1(const vector<Punto>& data) {
    cout << "\n=== EXPERIMENTO 1: k=18, 10 ejecuciones ===\n";
    cout << fixed << setprecision(4);
    int k = 18;
    int maxIter = 300;

    cout << "\n--- Fuerza Bruta ---\n";
    cout << "Run | Tiempo(ms) | Inercia       | Iters\n";
    for (int run = 0; run < 10; run++) {
        mt19937 rng(run);  
        auto t0 = Clock::now();
        auto res = kMeansBrute(data, k, maxIter, rng);
        double t = elapsed(t0);
        cout << setw(3) << run+1 << " | " << setw(10) << t << " | " << setw(13) << res.inerciaTotal << " | " << res.iteraciones << "\n";
    }

    cout << "\n--- KD-Tree ---\n";
    cout << "Run | Tiempo(ms) | Inercia       | Iters\n";
    for (int run = 0; run < 10; run++) {
        mt19937 rng(run);
        auto t0 = Clock::now();
        auto res = ArbolKDkMeans(data, k, maxIter, rng);
        double t = elapsed(t0);
        cout << setw(3) << run+1 << " | " << setw(10) << t << " | " << setw(13) << res.inerciaTotal << " | " << res.iteraciones << "\n";
    }
}

void experimento2(const vector<Punto>& allData) {
    cout << "\n=== EXPERIMENTO 2: k fijo, variar n ===\n";
    vector<int> ks = {5, 15, 25, 50, 75};
    vector<int> ns = {1000, 1150, 1300, 1450, 1600, 1750, 1900, 2050, 2200, 2400};
    int maxIter = 300;
    int runs = 3;

    ofstream csv("exp2_n_fijo_k.csv");
    csv << "k,n,brute_ms,kdtree_ms\n";

    for (int k : ks) {
        for (int n : ns) {
            vector<Punto> data(allData.begin(), allData.begin() + n);
            double sumB = 0, sumK = 0;
            for (int r = 0; r < runs; r++) {
                mt19937 rng(r);
                auto t0 = Clock::now();
                kMeansBrute(data, k, maxIter, rng);
                sumB += elapsed(t0);

                rng = mt19937(r);
                t0 = Clock::now();
                ArbolKDkMeans(data, k, maxIter, rng);
                sumK += elapsed(t0);
            }
            csv << k << "," << n << "," << sumB/runs << "," << sumK/runs << "\n";
            cout << "k=" << k << " n=" << n << " OK\n";
        }
    }
    
    cout << "Guardado en exp2_n_fijo_k.csv\n";
}

void experimento3(const vector<Punto>& allData) {
    cout << "\n=== EXPERIMENTO 3: n fijo, variar k ===\n";
    vector<int> ns = {1000, 1450, 1900, 2400};
    vector<int> ks = {5, 15, 25, 50, 75, 100, 125, 150, 200};
    int maxIter = 300;
    int runs = 3;

    ofstream csv("exp3_k_fijo_n.csv");
    csv << "n,k,brute_ms,kdtree_ms\n";

    for (int n : ns) {
        vector<Punto> data(allData.begin(), allData.begin() + n);
        for (int k : ks) {
            double sumB = 0, sumK = 0;
            for (int r = 0; r < runs; r++) {
                mt19937 rng(r);
                auto t0 = Clock::now();
                kMeansBrute(data, k, maxIter, rng);
                sumB += elapsed(t0);

                rng = mt19937(r);
                t0 = Clock::now();
                ArbolKDkMeans(data, k, maxIter, rng);
                sumK += elapsed(t0);
            }
            csv << n << "," << k << "," << sumB/runs << "," << sumK/runs << "\n";
            cout << "n=" << n << " k=" << k << " OK\n";
        }
    }
    cout << "Guardado en exp3_k_fijo_n.csv\n";
}

int main(int argc, char* argv[]) {
    string filename = (argc > 1) ? argv[1] : "data.csv";
    vector<Punto> data = cargarArchivo(filename);
    
    if (data.empty()) {
        return 1;
    }
    cout << "Puntos cargados: " << data.size() << "\n";

    experimento1(data);
    experimento2(data);
    experimento3(data);

    return 0;
}
