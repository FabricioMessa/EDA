import hnswlib
import numpy as np
import time
from scipy.stats import wilcoxon

N = 10000           
DIM = 100           
SEMILLAS = [42, 123, 456, 789, 101112]

def crear_indice(dim, capacidad, semilla, usar_mnru=True):
    indice = hnswlib.Index(space="l2", dim=dim)
    indice.init_index(max_elements=capacidad, M=16, ef_construction=200, random_seed=semilla, allow_replace_deleted=True,)
    indice.set_ef(100)
    if usar_mnru:
        indice.set_tau(200)
        indice.set_tau_params(alpha=10.0, beta=5.0, interval=50)
    return indice

def medir_recall(indice, num_consultas=100):
    ids_consulta = np.arange(900000, 900000 + num_consultas)
    datos_consulta = np.random.randn(num_consultas, DIM).astype(np.float32)
    indice.add_items(datos_consulta, ids_consulta)

    aciertos = 0
    tiempos = []
    for i in range(num_consultas):
        inicio = time.time()
        resultado, distancias = indice.knn_query(datos_consulta[i], k=1)
        tiempos.append(time.time() - inicio)
        if resultado[0][0] == ids_consulta[i]:
            aciertos += 1

    recall = aciertos / num_consultas
    latencia_promedio = np.mean(tiempos) * 1000  # en milisegundos
    return recall, latencia_promedio

def obtener_metricas(indice):
    try:
        inalcanzables = indice.get_unreachable_count()
        operaciones = indice.get_operation_counter()
        tau = indice.get_tau()
        tiene_backup = indice.has_backup_index()
        if tiene_backup:
            backup_count = indice.get_backup_count()
        else:
            backup_count = 0
        return inalcanzables, operaciones, tau, tiene_backup, backup_count
    except AttributeError:
        return 0, 0, 0, False, 0

def ejecutar_escenario(nombre, funcion_escenario, usar_mnru):
    resultados = []
    for semilla in SEMILLAS:
        np.random.seed(semilla)
        resultado = funcion_escenario(semilla, usar_mnru)
        resultados.append(resultado)
    return resultados

def full_coverage(semilla, usar_mnru):
    np.random.seed(semilla)
    indice = crear_indice(DIM, N * 3, semilla, usar_mnru)

    siguiente_label = 0
    datos = np.random.randn(N, DIM).astype(np.float32)
    labels = np.arange(siguiente_label, siguiente_label + N)
    siguiente_label += N
    indice.add_items(datos, labels)

    tamano_bloque = max(1, N // 100)
    eliminados = 0
    labels_vivos = set(labels)

    for iteracion in range(100):
        vivos = np.array(list(labels_vivos))
        if len(vivos) < tamano_bloque:
            break
        a_eliminar = np.random.choice(vivos, tamano_bloque, replace=False)
        for id_vec in a_eliminar:
            indice.mark_deleted(int(id_vec))
            labels_vivos.discard(id_vec)
            eliminados += 1

        nuevos_datos = np.random.randn(tamano_bloque, DIM).astype(np.float32)
        nuevos_labels = np.arange(siguiente_label, siguiente_label + tamano_bloque)
        siguiente_label += tamano_bloque
        indice.add_items(nuevos_datos, nuevos_labels, replace_deleted=True)
        labels_vivos.update(nuevos_labels)

    recall, latencia = medir_recall(indice)
    inalc, ops, tau, tiene_bkp, bkp_cnt = obtener_metricas(indice)

    return {
        "recall": recall,
        "latencia": latencia,
        "inalcanzables": inalc,
        "operaciones": ops,
        "tau": tau,
        "tiene_backup": tiene_bkp,
        "backup_count": bkp_cnt,
        "eliminados": eliminados,
    }

def random_delete(semilla, usar_mnru):
    np.random.seed(semilla)
    indice = crear_indice(DIM, N * 2, semilla, usar_mnru)

    siguiente_label = 0
    datos = np.random.randn(N, DIM).astype(np.float32)
    labels = np.arange(siguiente_label, siguiente_label + N)
    siguiente_label += N
    indice.add_items(datos, labels)

    tamano_bloque = 50
    eliminados = 0
    labels_vivos = set(labels)

    for iteracion in range(50):
        vivos = np.array(list(labels_vivos))
        if len(vivos) < tamano_bloque:
            break
        a_eliminar = np.random.choice(vivos, tamano_bloque, replace=False)
        for id_vec in a_eliminar:
            indice.mark_deleted(int(id_vec))
            labels_vivos.discard(id_vec)
            eliminados += 1

        nuevos_datos = np.random.randn(tamano_bloque, DIM).astype(np.float32)
        nuevos_labels = np.arange(siguiente_label, siguiente_label + tamano_bloque)
        siguiente_label += tamano_bloque
        indice.add_items(nuevos_datos, nuevos_labels, replace_deleted=True)
        labels_vivos.update(nuevos_labels)

    recall, latencia = medir_recall(indice)
    inalc, ops, tau, tiene_bkp, bkp_cnt = obtener_metricas(indice)

    return {
        "recall": recall,
        "latencia": latencia,
        "inalcanzables": inalc,
        "operaciones": ops,
        "tau": tau,
        "tiene_backup": tiene_bkp,
        "backup_count": bkp_cnt,
        "eliminados": eliminados,
    }

def new_data(semilla, usar_mnru):
    np.random.seed(semilla)
    indice = crear_indice(DIM, N * 2, semilla, usar_mnru)

    siguiente_label = 0
    datos = np.random.randn(N, DIM).astype(np.float32)
    labels = np.arange(siguiente_label, siguiente_label + N)
    siguiente_label += N
    indice.add_items(datos, labels)

    eliminados = 0
    labels_vivos = set(labels)

    for iteracion in range(3):
        vivos = np.array(list(labels_vivos))
        a_eliminar = np.random.choice(vivos, N // 3, replace=False)
        for id_vec in a_eliminar:
            indice.mark_deleted(int(id_vec))
            labels_vivos.discard(id_vec)
            eliminados += 1

        nuevos_datos = np.random.randn(N // 3, DIM).astype(np.float32)
        nuevos_labels = np.arange(siguiente_label, siguiente_label + N // 3)
        siguiente_label += N // 3
        indice.add_items(nuevos_datos, nuevos_labels, replace_deleted=True)
        labels_vivos.update(nuevos_labels)

    recall, latencia = medir_recall(indice)
    inalc, ops, tau, tiene_bkp, bkp_cnt = obtener_metricas(indice)

    return {
        "recall": recall,
        "latencia": latencia,
        "inalcanzables": inalc,
        "operaciones": ops,
        "tau": tau,
        "tiene_backup": tiene_bkp,
        "backup_count": bkp_cnt,
        "eliminados": eliminados,
    }

def estadistica(lista):
    media = np.mean(lista)
    std = np.std(lista)
    return f"{media:.4f} +/- {std:.4f}"

def imprimir_tabla(nombre, resultados_base, resultados_mnru):
    recall_base = [r["recall"] for r in resultados_base]
    recall_mnru = [r["recall"] for r in resultados_mnru]
    latencia_base = [r["latencia"] for r in resultados_base]
    latencia_mnru = [r["latencia"] for r in resultados_mnru]
    inalc_base = [r["inalcanzables"] for r in resultados_base]
    inalc_mnru = [r["inalcanzables"] for r in resultados_mnru]
    bkp_base = [r["backup_count"] for r in resultados_base]
    bkp_mnru = [r["backup_count"] for r in resultados_mnru]

    try:
        _, p_valor = wilcoxon(recall_base, recall_mnru, zero_method="zsplit")
        p_valor = f"{p_valor:.4f}"
    except Exception:
        p_valor = "N/A"

    print(f"\n{nombre}")
    print(f"{'Metrica':<20}   {'Baseline':>25}   {'MN-RU':>25}   {'p-valor':>8}")
    print("-" * 85)
    print(f"{'Recall':<20}   {estadistica(recall_base):>25}   {estadistica(recall_mnru):>25}   {p_valor:>8}")
    print(f"{'Latencia (ms)':<20}   {estadistica(latencia_base):>25}   {estadistica(latencia_mnru):>25}")
    print(f"{'Inalcanzables':<20}   {estadistica(inalc_base):>25}   {estadistica(inalc_mnru):>25}")
    print(f"{'Backup count':<20}   {estadistica(bkp_base):>25}   {estadistica(bkp_mnru):>25}")

if __name__ == "__main__":
    print("=" * 65)
    print("EXPERIMENTOS MN-RU ADAPTATIVO")
    print(f"Dataset: {N} vectores, dimension = {DIM}")
    print("=" * 65)

    escenarios = [("Full Coverage", full_coverage), ("Random", random_delete), ("New Data", new_data)]

    for nombre, funcion in escenarios:
        resultados_base = ejecutar_escenario(nombre, funcion, usar_mnru=False)
        resultados_mnru = ejecutar_escenario(nombre, funcion, usar_mnru=True)
        imprimir_tabla(nombre, resultados_base, resultados_mnru)
