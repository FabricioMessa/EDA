import argparse
import os
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.manifold import TSNE

def load_labels_csv(path):
    df = pd.read_csv(path, header=None)
    coords = df.iloc[:, :-1].values
    labels = df.iloc[:, -1].values.astype(int)
    return coords, labels

def reduce_2d(coords, method="pca"):
    if coords.shape[1] == 2:
        return coords
    if method == "pca":
        return PCA(n_components=2, random_state=42).fit_transform(coords)
    elif method == "tsne":
        n = coords.shape[0]
        perp = max(5, min(30, n // 3))
        return TSNE(n_components=2, perplexity=perp, random_state=42).fit_transform(coords)
    elif method == "umap":
        import umap
        return umap.UMAP(random_state=42).fit_transform(coords)
    else:
        raise ValueError(f"Metodo no soportado: {method}")

def plot_clusters(coords_2d, labels, title, outpath, reduce_method=""):
    unique = sorted(set(labels))
    cmap = plt.get_cmap("tab10")
    fig, ax = plt.subplots(figsize=(8, 6))
    for lab in unique:
        mask = labels == lab
        c = "gray" if lab == 0 else cmap((lab - 1) % 10)
        label_name = "Ruido" if lab == 0 else f"Cluster {lab}"
        ax.scatter(coords_2d[mask, 0], coords_2d[mask, 1],
                   c=[c], label=label_name, s=12, alpha=0.8, edgecolors="none")
    if reduce_method:
        title += f" ({reduce_method.upper()})"
    ax.set_title(title)
    ax.legend(markerscale=2, fontsize=8, loc="best")
    fig.tight_layout()
    fig.savefig(outpath, dpi=150)
    plt.close(fig)
    print(f"Grafico guardado: {outpath}")

def plot_comparison(orig_2d, classic_labels, kdtree_labels, title, outpath):
    fig, axes = plt.subplots(1, 2, figsize=(14, 6))
    cmap = plt.get_cmap("tab10")

    for ax, labels, tit in zip(
        axes,
        [classic_labels, kdtree_labels],
        ["DBSCAN Clasico", "DBSCAN + KDTree"]
    ):
        unique = sorted(set(labels))
        for lab in unique:
            mask = labels == lab
            c = "gray" if lab == 0 else cmap((lab - 1) % 10)
            lname = "Ruido" if lab == 0 else f"Cluster {lab}"
            ax.scatter(orig_2d[mask, 0], orig_2d[mask, 1],
                       c=[c], label=lname, s=12, alpha=0.8, edgecolors="none")
        ax.set_title(f"{tit}\n{title}")
        ax.legend(markerscale=2, fontsize=7, loc="best")

    fig.tight_layout()
    fig.savefig(outpath, dpi=150)
    plt.close(fig)
    print(f"Comparacion guardada: {outpath}")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--classic", default="resultados/classic_labels.csv")
    parser.add_argument("--kdtree", default="resultados/kdtree_labels.csv")
    parser.add_argument("--reduce", choices=["pca", "tsne", "umap"], default="pca")
    parser.add_argument("--outdir", default="resultados")
    parser.add_argument("--suffix", default="")
    args = parser.parse_args()

    classic_coords, classic_labels = load_labels_csv(args.classic)
    kdtree_coords, kdtree_labels = load_labels_csv(args.kdtree)

    suffix = "_" + args.suffix if args.suffix else ""
    ndims = classic_coords.shape[1]
    os.makedirs(args.outdir, exist_ok=True)

    reduce_label = ""
    if ndims == 2:
        coords_2d_classic = classic_coords
        coords_2d_kdtree = kdtree_coords
    else:
        coords_2d_classic = reduce_2d(classic_coords, args.reduce)
        coords_2d_kdtree = reduce_2d(kdtree_coords, args.reduce)
        reduce_label = args.reduce

    plot_clusters(coords_2d_classic, classic_labels,
                  f"DBSCAN Clasico - {ndims}D dataset",
                  f"{args.outdir}/clusters_classic_{ndims}d{suffix}.png",
                  reduce_label)

    plot_clusters(coords_2d_kdtree, kdtree_labels,
                  f"DBSCAN + KDTree - {ndims}D dataset",
                  f"{args.outdir}/clusters_kdtree_{ndims}d{suffix}.png",
                  reduce_label)

    if ndims == 2:
        plot_comparison(classic_coords, classic_labels, kdtree_labels,
                        f"Comparacion - Dataset {ndims}D",
                        f"{args.outdir}/comparison_{ndims}d{suffix}.png")

if __name__ == "__main__":
    main()
