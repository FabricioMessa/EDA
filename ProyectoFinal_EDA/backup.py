import hnswlib
import numpy as np
import tempfile
import shutil
import os

dim = 100
n = 10000

tmpdir = tempfile.mkdtemp()

p = hnswlib.Index(space='l2', dim=dim)
p.init_index(max_elements=n, M=8, ef_construction=100, allow_replace_deleted=True, is_persistent_index=True, persistence_location=tmpdir)

ids = np.arange(n)
data = np.random.randn(n, dim).astype(np.float32)
p.add_items(data, ids)

print(f"\nInsertados {n} vectores")
print(f"Inalcanzables: {p.get_unreachable_count()}")

for i in range(15):
    p.mark_deleted(i)

print(f"Inalcanzables: {p.get_unreachable_count()}")
print(f"Operation counter: {p.get_operation_counter()}")

p.rebuild_backup()
print(f"Backup creado: {p.has_backup_index()}")
print(f"Backup count: {p.get_backup_count()}")

p.persist_dirty()
p.save_backup()

p2 = hnswlib.Index(space='l2', dim=dim)
p2.load_index(tmpdir, max_elements=n, allow_replace_deleted=True, is_persistent_index=True)

print(f"Elementos cargados: {p2.get_current_count()}")
print(f"Operation counter: {p2.get_operation_counter()}")
print(f"Tau: {p2.get_tau()}")

p2.load_backup()
print(f"Backup cargado: {p2.has_backup_index()}")
print(f"Backup count: {p2.get_backup_count()}")

query = np.random.randn(dim).astype(np.float32)
labels, dists = p2.knn_query(query, k=5)
print(f"Query OK - Top 5 labels: {labels}")

shutil.rmtree(tmpdir)
