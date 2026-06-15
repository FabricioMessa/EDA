#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include "dbscan.cpp"

static std::vector<Point> load_csv(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: no se pudo abrir " << filename << std::endl;
        return points;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string token;
        Point p;
        while (std::getline(ss, token, ',')) {
            try {
                p.coords.push_back(std::stod(token));
            } catch (...) {
                p.coords.clear();
                break;
            }
        }
        if (!p.coords.empty()) {
            p.cluster_id = -2;
            points.push_back(p);
        }
    }
    return points;
}

static std::vector<Point> copy_points(const std::vector<Point>& orig) {
    std::vector<Point> copy;
    copy.reserve(orig.size());
    for (const auto& p : orig) {
        Point c;
        c.coords = p.coords;
        c.cluster_id = p.cluster_id;
        copy.push_back(c);
    }
    return copy;
}

static void save_labels(const std::vector<Point>& points,
                        const std::string& filename) {
    std::ofstream out(filename);
    for (const auto& p : points) {
        for (size_t i = 0; i < p.coords.size(); ++i) {
            if (i > 0) out << ",";
            out << p.coords[i];
        }
        out << "," << p.cluster_id << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Uso: " << argv[0]
                  << " <dataset.csv> <eps> <min_samples> <output_name>\n";
        return 1;
    }

    std::string filename = argv[1];
    double eps = std::stod(argv[2]);
    int min_samples = std::stoi(argv[3]);
    std::string name = argv[4];

    std::cout << "Cargando dataset: " << filename << std::endl;
    auto points_orig = load_csv(filename);
    if (points_orig.empty()) {
        std::cerr << "Error: dataset vacio o no valido\n";
        return 1;
    }
    std::cout << "Puntos cargados: " << points_orig.size()
              << ", dimensiones: " << points_orig[0].coords.size() << std::endl;

    auto points_classic = copy_points(points_orig);
    auto t1 = std::chrono::high_resolution_clock::now();
    int clusters_classic = dbscan_classic(points_classic, eps, min_samples);
    auto t2 = std::chrono::high_resolution_clock::now();
    double time_classic = std::chrono::duration<double, std::milli>(t2 - t1).count();

    auto points_kdtree = copy_points(points_orig);
    auto t3 = std::chrono::high_resolution_clock::now();
    int clusters_kdtree = dbscan_kdtree(points_kdtree, eps, min_samples);
    auto t4 = std::chrono::high_resolution_clock::now();
    double time_kdtree = std::chrono::duration<double, std::milli>(t4 - t3).count();

    int outliers_classic = count_outliers(points_classic);
    int outliers_kdtree = count_outliers(points_kdtree);

    save_labels(points_classic, "resultados/" + name + "_classic_labels.csv");
    save_labels(points_kdtree, "resultados/" + name + "_kdtree_labels.csv");

    std::cout << "\n========== RESULTADOS ==========\n";
    std::cout << "DBSCAN Clasico:\n";
    std::cout << "  Tiempo: " << std::fixed << std::setprecision(2)
              << time_classic << " ms\n";
    std::cout << "  Clusters: " << clusters_classic << "\n";
    std::cout << "  Outliers: " << outliers_classic << "\n\n";
    std::cout << "DBSCAN + KDTree:\n";
    std::cout << "  Tiempo: " << std::fixed << std::setprecision(2)
              << time_kdtree << " ms\n";
    std::cout << "  Clusters: " << clusters_kdtree << "\n";
    std::cout << "  Outliers: " << outliers_kdtree << "\n";
    std::cout << "================================" << std::endl;

    std::ofstream timing("resultados/" + name + "_timing.txt");
    timing << "algoritmo,tiempo_ms,clusters,outliers\n";
    timing << "clasico," << time_classic << "," << clusters_classic << ","
           << outliers_classic << "\n";
    timing << "kdtree," << time_kdtree << "," << clusters_kdtree << ","
           << outliers_kdtree << "\n";

    return 0;
}
