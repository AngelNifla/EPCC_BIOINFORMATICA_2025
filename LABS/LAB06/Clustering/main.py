import pandas as pd
import matplotlib.pyplot as plt
from scipy.cluster.hierarchy import dendrogram
import numpy as np

def cargar_csv_a_linkage(csv_file):
    df = pd.read_csv(csv_file, dtype=str)

    df['Cluster1'] = df['Cluster1'].astype(str)
    df['Cluster2'] = df['Cluster2'].astype(str)
    df['Distance'] = df['Distance'].astype(float)
    df['Size'] = df['Size'].astype(int)

    hojas = sorted(set(df['Cluster1']) | set(df['Cluster2']), key=str)
    hojas_originales = [x for x in hojas if ',' not in x and '(' not in x and ')' not in x]

    nombre_to_id = {name: i for i, name in enumerate(hojas_originales)}
    current_id = len(hojas_originales)
    linkage_matrix = []

    for _, row in df.iterrows():
        c1, c2 = row['Cluster1'], row['Cluster2']

        id1 = nombre_to_id.get(c1)
        if id1 is None:
            id1 = nombre_to_id[c1] = current_id
            current_id += 1

        id2 = nombre_to_id.get(c2)
        if id2 is None:
            id2 = nombre_to_id[c2] = current_id
            current_id += 1

        linkage_matrix.append([id1, id2, float(row['Distance']), int(row['Size'])])

    return np.array(linkage_matrix), hojas_originales, df['Distance'].tolist()

def graficar_dendrograma(csv_file, titulo, archivo_salida):
    linkage_matrix, etiquetas, distancias = cargar_csv_a_linkage(csv_file)

    plt.figure(figsize=(10, 6))
    dendro = dendrogram(
        linkage_matrix,
        labels=etiquetas,
        orientation='left',
        color_threshold=0,
        above_threshold_color='black',
        show_contracted=True
    )

    # Personalizar eje X con distancias únicas
    distancias_unicas = sorted(set(distancias))
    plt.xticks(distancias_unicas)

    # Agregar líneas punteadas y etiquetas de distancia
    icoord = dendro['icoord']
    dcoord = dendro['dcoord']
    for xs, ys in zip(icoord, dcoord):
        x = ys[1]  # distancia de fusión
        y = (xs[1] + xs[2]) / 2  # centro vertical

        # Línea punteada hacia eje X
        plt.axvline(x=x, ymin=0, ymax=(y / max(np.array(icoord).flatten())),
                    linestyle='--', color='gray', linewidth=0.8)

        # Etiqueta de distancia
        plt.text(x + 0.09, y+0.7, f"{x:.2f}", va='center', fontsize=8, color='black')

    plt.title(titulo)
    plt.xlabel("Distancia")
    plt.tight_layout()
    plt.savefig(archivo_salida)
    plt.close()
    print(f"Dendrograma guardado como '{archivo_salida}'.")


# Generar los tres dendrogramas con detalles
graficar_dendrograma("linkage_min.csv", "Linkage Simple", "dendrograma_min.png")
graficar_dendrograma("linkage_max.csv", "Linkage Simple Max", "dendrograma_max.png")
graficar_dendrograma("linkage_prom.csv", "Linkage Simple Prom", "dendrograma_prom.png")
