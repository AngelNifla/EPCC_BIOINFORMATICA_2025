#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <omp.h>

using namespace std;

struct Resultado {
    int puntuacionMax;
    int filaFin;
    int colFin;
    string alineamiento1;
    string alineamiento2;
};

Resultado smith_waterman(const string& s1, const string& s2, int gap, int match,
                            int mismatch, vector<vector<int>>& matriz) {
    int n = s1.size();
    int m = s2.size();
    matriz.assign(n + 1, vector<int>(m + 1, 0));

    int puntuacionMax = 0;
    int filaMax = 0;
    int colMax = 0;

    // Procesamiento por anti-diagonales paralelizadas
    for (int suma = 2; suma <= n + m; suma++) {
        #pragma omp parallel for shared(matriz, puntuacionMax, filaMax, colMax)
        for (int i = 1; i <= n; i++) {
            int j = suma - i;
            if (j >= 1 && j <= m) {
                int diag = matriz[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? match : mismatch);
                int arriba = matriz[i - 1][j] + gap;
                int izq = matriz[i][j - 1] + gap;
                int maximo = max({0, diag, arriba, izq});
                matriz[i][j] = maximo;

                #pragma omp critical
                {
                    if (maximo > puntuacionMax) {
                        puntuacionMax = maximo;
                        filaMax = i;
                        colMax = j;
                    }
                }
            }
        }
    }

    string alineacion1 = "", alineacion2 = "";
    int i = filaMax, j = colMax;
    while (i > 0 && j > 0 && matriz[i][j] > 0) {
        if (matriz[i][j] == matriz[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? match : mismatch)) {
            alineacion1 = s1[i - 1] + alineacion1;
            alineacion2 = s2[j - 1] + alineacion2;
            i--; j--;
        } else if (matriz[i][j] == matriz[i - 1][j] + gap) {
            i--;
        } else {
            j--;
        }
    }

    return {puntuacionMax, i + 1, j + 1, alineacion1, alineacion2};
}

bool existe_otro_maximo(const vector<vector<int>>& matriz, int maximo) {
    int count = 0;
    for (const auto& fila : matriz) {
        for (int val : fila) {
            if (val == maximo) count++;
        }
    }
    return count > 1;
}

int main() {
    string cadena1, cadena2;
    int penalidadGap, recompensaMatch, penalidadMismatch;

    cout << "Secuencia 1: ";
    cin >> cadena1;
    cout << "Secuencia 2: ";
    cin >> cadena2;
    cout << "Penalidad gap (-): ";
    cin >> penalidadGap;
    cout << "Puntaje match: ";
    cin >> recompensaMatch;
    cout << "Penalidad mismatch: ";
    cin >> penalidadMismatch;

    vector<vector<int>> matriz;
    Resultado resultado = smith_waterman(cadena1, cadena2, penalidadGap, recompensaMatch, penalidadMismatch, matriz);

    ofstream archivo("resultado2.txt");
    archivo << "--------------------------SECUENCIAS--------------------------\n";
    archivo << "Secuencia 1: " << cadena1 << "\n";
    archivo << "Secuencia 2: " << cadena2 << "\n\n";

    archivo << "\n--------------------------PENALIDADES--------------------------\n";
    archivo << "Parámetros:\n  match: " << recompensaMatch << "\n  mismatch: " <<
                                                penalidadMismatch << "\n  Gap: " <<
                                                penalidadGap << "\n\n";
    archivo << "\n--------------------------DATOS HALLADOS--------------------------\n";
    archivo << "(i) Valor maximo de alineamiento: " << resultado.puntuacionMax << "\n";
    archivo << "(ii) Subcadena alineada comun: " << resultado.alineamiento1 << "\n";
    archivo << "(iii) Posicion en secuencia 1: " << resultado.filaFin << "\n";
    archivo << "      Posicion en secuencia 2: " << resultado.colFin << "\n";

    archivo << "\n--------------------------MATRIZ--------------------------\n\n";
    for (char c : cadena2) archivo << " " << c << " ";
    archivo << "\n  +";
    for (int i = 0; i <= cadena2.size(); i++) archivo << "---";
    archivo << "\n";

    for (size_t i = 0; i <= cadena1.size(); i++) {
        if (i == 0) archivo << "- |";
        else archivo << cadena1[i - 1] << " |";
        for (size_t j = 0; j <= cadena2.size(); j++) {
            archivo << (matriz[i][j] >= 0 ? " " : "") << matriz[i][j] << " ";
        }
        archivo << "\n";
    }

    archivo << "\n(v) Existe otra subsecuencia Optima? ";
    archivo << (existe_otro_maximo(matriz, resultado.puntuacionMax) ? "Si" : "No") << "\n";
    archivo.close();

    cout << "Resultado guardado en 'resultado.txt'\n";
    return 0;
}
