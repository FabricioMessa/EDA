import pandas as pd
import matplotlib.pyplot as plt
import os

os.makedirs("figures", exist_ok=True)

exp2 = pd.read_csv("exp2_n_fijo_k.csv")

for k in sorted(exp2["k"].unique()):
    df = exp2[exp2["k"] == k]

    plt.figure(figsize=(8, 5))
    plt.plot(df["n"], df["brute_ms"], marker="o", label="Fuerza bruta")
    plt.plot(df["n"], df["kdtree_ms"], marker="o", label="KD-Tree")
    plt.title(f"Tiempo de ejecución variando n, k={k}")
    plt.xlabel("Número de puntos n")
    plt.ylabel("Tiempo de ejecución (ms)")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f"figures/exp2_k_{k}.png", dpi=300)
    plt.close()

exp3 = pd.read_csv("exp3_k_fijo_n.csv")

for n in sorted(exp3["n"].unique()):
    df = exp3[exp3["n"] == n]

    plt.figure(figsize=(8, 5))
    plt.plot(df["k"], df["brute_ms"], marker="o", label="Fuerza bruta")
    plt.plot(df["k"], df["kdtree_ms"], marker="o", label="KD-Tree")
    plt.title(f"Tiempo de ejecución variando k, n={n}")
    plt.xlabel("Número de centroides k")
    plt.ylabel("Tiempo de ejecución (ms)")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f"figures/exp3_n_{n}.png", dpi=300)
    plt.close()
