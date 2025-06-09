#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <omp.h>
#include <tuple>

using namespace std;

struct Cluster {
    string nombre;
    vector<string> historial;
};

void leer_matriz_distancia(const string& archivo, vector<Cluster>& clusters, vector<vector<double>>& matriz) {
    ifstream in(archivo);
    if (!in.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        exit(1);
    }

    string linea;
    vector<vector<double>> temp;
    while (getline(in, linea)) {
        istringstream iss(linea);
        string etiqueta;
        iss >> etiqueta;
        Cluster c;
        c.nombre = etiqueta;
        c.historial.push_back(etiqueta);
        clusters.push_back(c);

        vector<double> fila;
        double val;
        while (iss >> val) fila.push_back(val);
        temp.push_back(fila);
    }

    int n = clusters.size();
    matriz.assign(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < i; ++j)
            matriz[i][j] = matriz[j][i] = temp[i][j];
}

void imprimir_matriz(ofstream& out, const vector<Cluster>& clusters, const vector<vector<double>>& matriz) {
    out << fixed << setprecision(2);
    out << "    ";
    for (const auto& c : clusters) out << setw(10) << c.nombre;
    out << '\n';
    for (size_t i = 0; i < matriz.size(); ++i) {
        out << setw(4) << clusters[i].nombre;
        for (size_t j = 0; j < matriz[i].size(); ++j)
            out << setw(10) << (i == j ? 0.0 : matriz[i][j]);
        out << '\n';
    }
    out << '\n';
}

void clustering_aglomerativo(const string& metodo, const string& nombre_archivo_salida) {
    vector<Cluster> clusters;
    vector<vector<double>> matriz;
    leer_matriz_distancia("matriz.txt", clusters, matriz);

    ofstream salida(nombre_archivo_salida);
    salida << "Clustering jerárquico aglomerativo - " << metodo << " linkage\n\n";

    vector<string> resumen_niveles;
    vector<tuple<string, string, double, int>> linkage_data;

    int nivel = 0;
    while (clusters.size() > 1) {
        salida << "Nivel " << nivel << ":\n";
        imprimir_matriz(salida, clusters, matriz);

        int idx1 = -1, idx2 = -1;
        //double opt_dist = (metodo == "max") ? -1.0 : numeric_limits<double>::max();
        double opt_dist = numeric_limits<double>::max();  // Igual para todos los métodos

        #pragma omp parallel for collapse(2)
        for (size_t i = 0; i < matriz.size(); ++i) {
            for (size_t j = i + 1; j < matriz[i].size(); ++j) {
                bool mejor = false;
                if (metodo == "min" && matriz[i][j] < opt_dist) mejor = true;
                if (metodo == "max" && matriz[i][j] < opt_dist) mejor = true;
                if (metodo == "prom" && matriz[i][j] < opt_dist) mejor = true;

                if (mejor) {
                    #pragma omp critical
                    {
                        if (mejor) {
                            opt_dist = matriz[i][j];
                            idx1 = i;
                            idx2 = j;
                        }
                    }
                }
            }
        }

        string nombre1 = clusters[idx1].nombre;
        string nombre2 = clusters[idx2].nombre;
        string combinacion = "(" + nombre1 + ", " + nombre2 + ")";
        salida << "Se unieron: " << nombre1 << " y " << nombre2;
        salida << " (distancia: " << opt_dist << ")\n\n";

        resumen_niveles.push_back("Nivel " + to_string(nivel) + ": " + combinacion + "  distancia: " + to_string(opt_dist));
        int size_cluster = clusters[idx1].historial.size() + clusters[idx2].historial.size();
        linkage_data.push_back({nombre1, nombre2, opt_dist, size_cluster});

        Cluster nuevo;
        nuevo.nombre = combinacion;
        nuevo.historial.push_back(combinacion);

        vector<double> nueva_fila;
        for (size_t k = 0; k < clusters.size(); ++k) {
            if (k != idx1 && k != idx2) {
                double val;
                if (metodo == "min")
                    val = min(matriz[idx1][k], matriz[idx2][k]);
                else if (metodo == "max")
                    val = max(matriz[idx1][k], matriz[idx2][k]);
                else // prom
                    val = (matriz[idx1][k] + matriz[idx2][k]) / 2.0;
                nueva_fila.push_back(val);
            }
        }

        vector<Cluster> nuevos_clusters;
        vector<vector<double>> nueva_matriz;
        for (size_t i = 0; i < clusters.size(); ++i) {
            if (i != idx1 && i != idx2) nuevos_clusters.push_back(clusters[i]);
        }
        nuevos_clusters.push_back(nuevo);

        for (size_t i = 0; i < matriz.size(); ++i) {
            if (i == idx1 || i == idx2) continue;
            vector<double> fila;
            for (size_t j = 0; j < matriz.size(); ++j) {
                if (j == idx1 || j == idx2) continue;
                fila.push_back(matriz[i][j]);
            }
            nueva_matriz.push_back(fila);
        }

        for (size_t i = 0; i < nueva_matriz.size(); ++i)
            nueva_matriz[i].push_back(nueva_fila[i]);
        nueva_fila.push_back(0.0);
        nueva_matriz.push_back(nueva_fila);

        clusters = nuevos_clusters;
        matriz = nueva_matriz;
        nivel++;
    }

    salida << "\nResumen de niveles de agrupamiento:\n";
    for (const auto& linea : resumen_niveles)
        salida << linea << "\n";

    salida << "\nDendograma: " << clusters[0].historial[0] << "\n";
    salida.close();

    string csv_name = "linkage_" + metodo + ".csv";
    ofstream csv(csv_name);
    csv << "\"Cluster1\",\"Cluster2\",Distance,Size\n";
    for (const auto& [a, b, dist, size] : linkage_data)
        csv << "\""<< a << "\",\"" << b << "\"," << dist << "," << size << "\n";
    csv.close();

    cout << "\n[" << metodo << "] Proceso completado. Salida en '" << nombre_archivo_salida << "' y '" << csv_name << "'\n";
}

int main() {
    clustering_aglomerativo("min", "clustering_min.txt");
    clustering_aglomerativo("max", "clustering_max.txt");
    clustering_aglomerativo("prom", "clustering_prom.txt");
    return 0;
}
