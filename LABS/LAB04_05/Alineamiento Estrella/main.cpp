// alineamiento_estrella.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <omp.h>

using namespace std;

int match_score, mismatch_penalty, gap_penalty;

int puntuacion(const char &a, const char &b) {
    return a == b ? match_score : mismatch_penalty;
}

pair<int, vector<vector<int>>> alineamientoGlobal(const string &s1, const string &s2, string &res1, string &res2) {
    int n = s1.size();
    int m = s2.size();
    vector<vector<int>> matriz(n + 1, vector<int>(m + 1, 0));

    for (int i = 0; i <= n; ++i) matriz[i][0] = i * gap_penalty;
    for (int j = 0; j <= m; ++j) matriz[0][j] = j * gap_penalty;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int diag = matriz[i - 1][j - 1] + puntuacion(s1[i - 1], s2[j - 1]);
            int arriba = matriz[i - 1][j] + gap_penalty;
            int izq = matriz[i][j - 1] + gap_penalty;
            matriz[i][j] = max({diag, arriba, izq});
        }
    }

    // Trazado inverso
    int i = n, j = m;
    res1 = ""; res2 = "";
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && matriz[i][j] == matriz[i - 1][j - 1] + puntuacion(s1[i - 1], s2[j - 1])) {
            res1 = s1[i - 1] + res1;
            res2 = s2[j - 1] + res2;
            i--; j--;
        } else if (i > 0 && matriz[i][j] == matriz[i - 1][j] + gap_penalty) {
            res1 = s1[i - 1] + res1;
            res2 = '-' + res2;
            i--;
        } else {
            res1 = '-' + res1;
            res2 = s2[j - 1] + res2;
            j--;
        }
    }

    return {matriz[n][m], matriz};
}

int main() {
    int n;
    cout << "Ingrese el número de secuencias: ";
    cin >> n;
    cin.ignore();

    vector<string> secuencias(n);
    for (int i = 0; i < n; ++i) {
        cout << "Ingrese la secuencia S" << i + 1 << ": ";
        getline(cin, secuencias[i]);
    }

    cout << "Ingrese el puntaje de coincidencia (match): ";
    cin >> match_score;
    cout << "Ingrese la penalidad por no coincidencia (mismatch): ";
    cin >> mismatch_penalty;
    cout << "Ingrese la penalidad por gap: ";
    cin >> gap_penalty;

    vector<vector<int>> matrizPuntajes(n, vector<int>(n, 0));
    vector<int> totales(n, 0);

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                string temp1, temp2;
                int score = alineamientoGlobal(secuencias[i], secuencias[j], temp1, temp2).first;
                matrizPuntajes[i][j] = score;
                #pragma omp atomic
                totales[i] += score;
            }
        }
    }

    int idx_estrella = max_element(totales.begin(), totales.end()) - totales.begin();
    string secuencia_estrella = secuencias[idx_estrella];

    vector<string> alineadas(n);
    vector<string> alineadas_con_estrella(n);
    alineadas[idx_estrella] = secuencia_estrella;

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        if (i != idx_estrella) {
            string s1, s2;
            alineamientoGlobal(secuencia_estrella, secuencias[i], s1, s2);
            alineadas_con_estrella[i] = "S" + to_string(idx_estrella + 1) + " vs S" + to_string(i + 1) + ":\n" + s1 + "\n" + s2 + "\n";
            alineadas[i] = s2;
            alineadas[idx_estrella] = s1;
        }
    }

    ofstream out("alineamiento_estrella.txt");
    out << "Secuencias:\n";
    for (int i = 0; i < n; ++i) out << "S" << i + 1 << " : " << secuencias[i] << "\n";
    out << "\nMatriz de scores par-a-par:\n\n     ";
    for (int i = 0; i < n; ++i) out << " S" << i + 1 << " ";
    out << " Total\n-------------------------------------------\n";
    for (int i = 0; i < n; ++i) {
        out << "S" << i + 1 << " |";
        for (int j = 0; j < n; ++j)
            out << (i == j ? "  0" : (matrizPuntajes[i][j] >= 0 ? "  " : " ")) << matrizPuntajes[i][j] << " |";
        out << "  " << totales[i] << "\n";
    }
    out << "-------------------------------------------\nTotal|";
    for (int t : totales) out << (t >= 0 ? "  " : " ") << t << " |";
    out << "  " << accumulate(totales.begin(), totales.end(), 0) << "\n\n";

    out << "Alineamientos con la estrella (S" << idx_estrella + 1 << "):\n\n";
    for (int i = 0; i < n; ++i) {
        if (i != idx_estrella) {
            out << alineadas_con_estrella[i] << "\n";
        }
    }

    // Ajustar longitudes con gaps al final para consistencia
    size_t max_len = 0;
    for (const auto &seq : alineadas) max_len = max(max_len, seq.size());

    for (auto &seq : alineadas)
        while (seq.size() < max_len)
            seq += '-';

    // Escribir alineamiento múltiple con formato estilo tabla
    out << "Alineamiento múltiple:\n";
    for (int i = 0; i < n; ++i) {
        out << "S" << i + 1 << " | ";
        for (char c : alineadas[i]) out << c << ' ';
        out << '\n';
    }

    out.close();
    cout << "Archivo 'alineamiento_estrella.txt' generado correctamente.\n";
    return 0;
}
