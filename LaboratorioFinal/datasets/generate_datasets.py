import numpy as np
import pandas as pd
from sklearn.datasets import make_blobs, make_moons

np.random.seed(42)

def generate_2d():
    X1, _ = make_blobs(n_samples=250, centers=[(2, 2)], cluster_std=0.6)
    X2, _ = make_blobs(n_samples=250, centers=[(8, 3)], cluster_std=0.7)
    X3, _ = make_blobs(n_samples=250, centers=[(5, 9)], cluster_std=0.5)
    noise = np.random.uniform(low=-2, high=12, size=(40, 2))
    X = np.vstack([X1, X2, X3, noise])
    np.random.shuffle(X)
    cols = [f"x{i}" for i in range(X.shape[1])]
    pd.DataFrame(X, columns=cols).to_csv("2d_dataset.csv", index=False)
    print(f"Dataset 2D generado: {X.shape[0]} puntos, {X.shape[1]} dimensiones")

def generate_nd():
    X1, _ = make_blobs(n_samples=200, n_features=6, centers=[(1, 2, 0, 3, -1, 2)], cluster_std=0.9)
    X2, _ = make_blobs(n_samples=200, n_features=6, centers=[(9, 4, -2, 1, 5, 3)], cluster_std=1.0)
    X3, _ = make_blobs(n_samples=200, n_features=6, centers=[(5, 9, 3, -2, 0, 7)], cluster_std=0.8)
    X4, _ = make_blobs(n_samples=200, n_features=6, centers=[(-1, -2, 6, 5, 3, -3)], cluster_std=0.85)
    noise = np.random.uniform(low=-3, high=11, size=(50, 6))
    X = np.vstack([X1, X2, X3, X4, noise])
    np.random.shuffle(X)
    cols = [f"x{i}" for i in range(X.shape[1])]
    pd.DataFrame(X, columns=cols).to_csv("nd_dataset.csv", index=False)
    print(f"Dataset 6D generado: {X.shape[0]} puntos, {X.shape[1]} dimensiones")

if __name__ == "__main__":
    generate_2d()
    generate_nd()
