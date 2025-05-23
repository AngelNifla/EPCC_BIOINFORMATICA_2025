#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <omp.h>

using namespace std;

struct Alineamiento {
    string alineacion1;
    string alineacion2;
};

void retroceso(const vector<vector<int>>& matriz, const string& s1, const string& s2,
               int i, int j, string a1, string a2,
               vector<Alineamiento>& resultados, int match, int mismatch, int gap) {
    if (i == 0 && j == 0) {
        resultados.push_back({a1, a2});
        return;
    }

    if (i > 0 && j > 0) {
        int diag = matriz[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? match : mismatch);
        if (matriz[i][j] == diag)
            retroceso(matriz, s1, s2, i - 1, j - 1, s1[i - 1] + a1, s2[j - 1] + a2, resultados, match, mismatch, gap);
    }
    if (i > 0) {
        int arriba = matriz[i - 1][j] + gap;
        if (matriz[i][j] == arriba)
            retroceso(matriz, s1, s2, i - 1, j, s1[i - 1] + a1, "-" + a2, resultados, match, mismatch, gap);
    }
    if (j > 0) {
        int izq = matriz[i][j - 1] + gap;
        if (matriz[i][j] == izq)
            retroceso(matriz, s1, s2, i, j - 1, "-" + a1, s2[j - 1] + a2, resultados, match, mismatch, gap);
    }
}

int main() {
    string secuencia1, secuencia2;
    int gap, match, mismatch;

    cout << "Secuencia 1: ";
    cin >> secuencia1;
    cout << "Secuencia 2: ";
    cin >> secuencia2;

    cout << "Penalidad gap (-): ";
    cin >> gap;
    cout << "Puntaje match: ";
    cin >> match;
    cout << "Penalidad mismatch: ";
    cin >> mismatch;
    

    int n = secuencia1.size();
    int m = secuencia2.size();

    vector<vector<int>> matriz(n + 1, vector<int>(m + 1, 0));

    // Inicializar bordes
    for (int i = 0; i <= n; ++i) matriz[i][0] = i * gap;
    for (int j = 0; j <= m; ++j) matriz[0][j] = j * gap;


/*
    // Llenar matriz de puntuaciones
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int diag = matriz[i - 1][j - 1] + (secuencia1[i - 1] == secuencia2[j - 1] ? coincidencia : diferencia);
            int arriba = matriz[i - 1][j] + penalidad_gap;
            int izquierda = matriz[i][j - 1] + penalidad_gap;
            matriz[i][j] = max(diag, max(arriba, izquierda));
        }
    }
*/

    // Paralelización por anti-diagonales
    for (int suma = 2; suma <= n + m; suma++) {
        #pragma omp parallel for
        for (int i = 1; i <= n; i++) {
            int j = suma - i;
            if (j < 1 || j > m) continue;

            int punt_diag = matriz[i - 1][j - 1] + (secuencia1[i - 1] == secuencia2[j - 1] ? match : mismatch);
            int punt_arriba = matriz[i - 1][j] + gap;
            int punt_izq = matriz[i][j - 1] + gap;
            matriz[i][j] = max(punt_diag, max(punt_arriba, punt_izq));
        }
    }

    int puntaje_final = matriz[n][m];

    // Retroceso
    vector<Alineamiento> alineamientos;
    retroceso(matriz, secuencia1, secuencia2, n, m, "", "", alineamientos, match, mismatch, gap);

    // Salida
    ofstream archivo("resultado2.txt");

    archivo << "--------------------------SECUENCIAS--------------------------\n";
    archivo << "Secuencia 1: " << secuencia1 << "\n";
    archivo << "Secuencia 2: " << secuencia2 << "\n\n";

    
    archivo << "\n--------------------------PENALIDADES--------------------------\n";
    archivo << "Parámetros:\n  match: " << match << "\n  mismatch: " <<
                                                mismatch << "\n  Gap: " <<
                                                gap << "\n\n";

    archivo << "\n--------------------------MATRIZ--------------------------\n\n";
    archivo << setw(6) << " " << setw(4) << " ";
    for (char c : secuencia2) archivo << setw(4) << c;
    archivo << "\n" << setw(6) << " " << setw(4) << " ";
    for (int j = 0; j <= m; j++) archivo << "----";
    archivo << "\n";

    for (int i = 0; i <= n; i++) {
        if (i == 0) archivo << setw(6) << " ";
        else archivo << setw(6) << secuencia1[i - 1];
        archivo << "|";
        for (int j = 0; j <= m; j++)
            archivo << setw(4) << matriz[i][j];
        archivo << "\n";
    }

    archivo << "\nScore final: " << puntaje_final << "\n";
    archivo << "--------------------------ALINEAMIENTOS--------------------------\n\n";
    archivo << "Alineamientos Encontrados: " << alineamientos.size() << "\n\n";

    int contador = 1;
    for (const auto& a : alineamientos) {
        archivo << "Alineamiento #" << contador++ << ":\n\t" << a.alineacion1 << "\n\t" << a.alineacion2 << "\n\n";
    }

    archivo.close();
    cout << "\n----> Alineamiento completo guardado en: resultado.txt\n";

    return 0;
}

