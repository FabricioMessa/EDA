import pandas as pd
import matplotlib.pyplot as plt
import os

os.makedirs("cluster_figures", exist_ok=True)

archivos = ["clusters_brute_run_1.csv", "clusters_brute_run_2.csv", "clusters_brute_run_3.csv", "clusters_kdtree_run_1.csv", "clusters_kdtree_run_2.csv", "clusters_kdtree_run_3.csv"]

for archivo in archivos:
    df = pd.read_csv(archivo)
    puntos = df[df["tipo"] == "punto"]
    centroides = df[df["tipo"] == "centroide"]
    plt.figure(figsize=(8, 6))
    plt.scatter(puntos["x"], puntos["y"], c=puntos["cluster"], cmap="tab20", s=12, alpha=0.75)
    plt.scatter(centroides["x"], centroides["y"], c="black", marker="X", s=220, label="Centroides")
    plt.title(archivo.replace(".csv", ""))
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True, alpha=0.25)
    plt.legend()
    plt.tight_layout()
    salida = os.path.join("cluster_figures", archivo.replace(".csv", ".png"))
    plt.savefig(salida, dpi=300)
    plt.close()
