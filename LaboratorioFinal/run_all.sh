#!/bin/bash
set -e
BASE="$(cd "$(dirname "$0")" && pwd)"
cd "$BASE"

echo "=== 1. Generando datasets ==="
mkdir -p datasets resultados
python3 datasets/generate_datasets.py

echo ""
echo "=== 2. Compilando C++ ==="
g++ -std=c++17 -O3 -o cpp/dbscan cpp/main.cpp cpp/dbscan_classic.cpp cpp/dbscan_kdtree.cpp

echo ""
echo "=== 3. Ejecutando DBSCAN ==="
mkdir -p resultados

echo "--- Dataset 2D (eps=1.5, minPts=5) ---"
cpp/dbscan datasets/2d_dataset.csv 1.5 5 2d

echo ""
echo "--- Dataset 6D (eps=4.0, minPts=10) ---"
cpp/dbscan datasets/nd_dataset.csv 4.0 10 6d

echo ""
echo "=== 4. Evaluando metricas ==="
python3 python/evaluate.py --classic resultados/2d_classic_labels.csv --kdtree resultados/2d_kdtree_labels.csv --out resultados/2d
python3 python/evaluate.py --classic resultados/6d_classic_labels.csv --kdtree resultados/6d_kdtree_labels.csv --out resultados/6d

echo ""
echo "=== 5. Generando visualizaciones ==="
python3 python/visualize.py --classic resultados/2d_classic_labels.csv --kdtree resultados/2d_kdtree_labels.csv --outdir resultados --reduce pca --suffix 2d
python3 python/visualize.py --classic resultados/6d_classic_labels.csv --kdtree resultados/6d_kdtree_labels.csv --outdir resultados --reduce pca --suffix 6d
python3 python/visualize.py --classic resultados/6d_classic_labels.csv --kdtree resultados/6d_kdtree_labels.csv --outdir resultados --reduce tsne --suffix 6d_tsne

echo ""
echo "=== Pipeline completado ==="
echo "Resultados en: $BASE/resultados/"
ls -la resultados/
