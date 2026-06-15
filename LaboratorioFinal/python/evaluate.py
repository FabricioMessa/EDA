import argparse
import os
import numpy as np
import pandas as pd
from sklearn.metrics import silhouette_score

def load_labels_csv(path):
    df = pd.read_csv(path, header=None)
    coords = df.iloc[:, :-1].values
    labels = df.iloc[:, -1].values.astype(int)
    return coords, labels

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--classic", default="resultados/classic_labels.csv")
    parser.add_argument("--kdtree", default="resultados/kdtree_labels.csv")
    parser.add_argument("--out", default="resultados")
    parser.add_argument("--suffix", default="")
    args = parser.parse_args()

    classic_coords, classic_labels = load_labels_csv(args.classic)
    kdtree_coords, kdtree_labels = load_labels_csv(args.kdtree)
    suffix = "_" + args.suffix if args.suffix else ""

    mask_c = classic_labels != 0
    mask_k = kdtree_labels != 0

    print("=" * 50)
    print(f"METRICAS DE CLUSTERING [{args.suffix or 'default'}]")
    print("=" * 50)

    print(f"\nDBSCAN Clasico:")
    print(f"  Puntos totales: {len(classic_labels)}")
    print(f"  Outliers (ruido): {np.sum(classic_labels == 0)}")
    n_clusters_c = len(set(classic_labels)) - (1 if 0 in classic_labels else 0)
    print(f"  Clusters encontrados: {n_clusters_c}")

    if mask_c.sum() > 1 and n_clusters_c > 1:
        sil_c = silhouette_score(classic_coords[mask_c], classic_labels[mask_c])
        print(f"  Silhouette Score: {sil_c:.4f}")
    else:
        sil_c = None
        print("  Silhouette Score: N/A (insuficientes clusters)")

    print(f"\nDBSCAN + KDTree:")
    print(f"  Puntos totales: {len(kdtree_labels)}")
    print(f"  Outliers (ruido): {np.sum(kdtree_labels == 0)}")
    n_clusters_k = len(set(kdtree_labels)) - (1 if 0 in kdtree_labels else 0)
    print(f"  Clusters encontrados: {n_clusters_k}")

    if mask_k.sum() > 1 and n_clusters_k > 1:
        sil_k = silhouette_score(kdtree_coords[mask_k], kdtree_labels[mask_k])
        print(f"  Silhouette Score: {sil_k:.4f}")
    else:
        sil_k = None
        print("  Silhouette Score: N/A (insuficientes clusters)")

    print("=" * 50)

    os.makedirs(args.out, exist_ok=True)
    outpath = os.path.join(args.out, f"metrics{suffix}.txt")
    with open(outpath, "w") as f:
        f.write(f"metric,classic,kdtree\n")
        f.write(f"total_points,{len(classic_labels)},{len(kdtree_labels)}\n")
        f.write(f"outliers,{np.sum(classic_labels == 0)},{np.sum(kdtree_labels == 0)}\n")
        f.write(f"n_clusters,{n_clusters_c},{n_clusters_k}\n")
        if sil_c is not None and sil_k is not None:
            f.write(f"silhouette,{sil_c:.4f},{sil_k:.4f}\n")

    print(f"\nResultados guardados en {outpath}")

if __name__ == "__main__":
    main()
