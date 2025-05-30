#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <omp.h>
#include <tuple>
#include <iomanip>

using namespace std;

// Funcion Needleman-Wunsch para dos secuencias
tuple<int, string, string> alineamiento_dos(const string& s1, const string& s2, int match, int mismatch, int gap) {
    int n = s1.size(), m = s2.size();
    vector<vector<int>> matriz(n+1, vector<int>(m+1));
    
    // Inicializacion
    for (int i = 0; i <= n; ++i) matriz[i][0] = i * gap;
    for (int j = 0; j <= m; ++j) matriz[0][j] = j * gap;

    // Llenar matriz
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int diag = matriz[i-1][j-1] + (s1[i-1] == s2[j-1] ? match : mismatch);
            int up = matriz[i-1][j] + gap;
            int left = matriz[i][j-1] + gap;
            matriz[i][j] = max({diag, up, left});
        }
    }

    // Trazado hacia atras
    string al1, al2;
    int i = n, j = m;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && matriz[i][j] == matriz[i-1][j-1] + (s1[i-1] == s2[j-1] ? match : mismatch)) {
            al1 = s1[i-1] + al1;
            al2 = s2[j-1] + al2;
            --i; --j;
        } else if (i > 0 && matriz[i][j] == matriz[i-1][j] + gap) {
            al1 = s1[i-1] + al1;
            al2 = '-' + al2;
            --i;
        } else {
            al1 = '-' + al1;
            al2 = s2[j-1] + al2;
            --j;
        }
    }
    return {matriz[n][m], al1, al2};
}

int main() {
    int num_secuencias, match, mismatch, gap;
    cout << "Numero de secuencias: ";
    cin >> num_secuencias;
    vector<string> secuencias(num_secuencias);
    for (int i = 0; i < num_secuencias; ++i) {
        cout << "Secuencia " << i+1 << ": ";
        cin >> secuencias[i];
    }
    cout << "Match: "; cin >> match;
    cout << "Mismatch: "; cin >> mismatch;
    cout << "Gap: "; cin >> gap;

    vector<vector<int>> score(num_secuencias, vector<int>(num_secuencias));

    // Calcular matriz de score par-a-par
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < num_secuencias; ++i) {
        for (int j = i+1; j < num_secuencias; ++j) {
            auto [s, _, __] = alineamiento_dos(secuencias[i], secuencias[j], match, mismatch, gap);
            score[i][j] = score[j][i] = s;
        }
    }

    // Determinar secuencia centro
    int mejor_total = -1, centro = -1;
    for (int i = 0; i < num_secuencias; ++i) {
        int suma = 0;
        for (int j = 0; j < num_secuencias; ++j) suma += score[i][j];
        if (suma > mejor_total) {
            mejor_total = suma;
            centro = i;
        }
    }

    vector<string> alineadas(num_secuencias);
    alineadas[centro] = secuencias[centro];

    // Alinear cada secuencia con la del centro
    for (int i = 0; i < num_secuencias; ++i) {
        if (i == centro) continue;
        auto [_, al_centro, al_i] = alineamiento_dos(secuencias[centro], secuencias[i], match, mismatch, gap);
        // Sincronizar gaps
        string nueva_centro, nueva_i;
        int idx = 0;
        for (int k = 0; k < al_centro.size(); ++k) {
            for (int j = 0; j < alineadas.size(); ++j) {
                if (alineadas[j].size() < k) alineadas[j] += '-';
            }
            for (int j = 0; j < alineadas.size(); ++j) {
                if (j == centro) continue;
                alineadas[j].push_back('-');
            }
        }
        alineadas[i] = al_i;
        alineadas[centro] = al_centro;
    }

    // Escribir salida
    ofstream salida("alineamiento_estrella.txt");

    // Matriz de scores con encabezado y sumatorias
    salida << "Matriz de scores par-a-par:\n\n";

    // Imprimir encabezado de columnas
    salida << "     |";
    for (int j = 0; j < num_secuencias; ++j)
        salida << " S" << j+1 << " |";
    salida << " Total\n";

    // Línea separadora
    salida << string(6 + (num_secuencias * 6) + 7, '-') << "\n";

    // Variables para suma de columnas
    vector<int> suma_col(num_secuencias, 0);

    // Imprimir filas con suma por fila
    for (int i = 0; i < num_secuencias; ++i) {
        salida << " S" << i+1 << " |";
        int suma_fila = 0;
        for (int j = 0; j < num_secuencias; ++j) {
            salida << " " << setw(3) << score[i][j] << " |";
            suma_fila += score[i][j];
            suma_col[j] += score[i][j];
        }
        salida << " " << setw(5) << suma_fila << "\n";
    }

    // Línea separadora inferior
    salida << string(6 + (num_secuencias * 6) + 7, '-') << "\n";

    // Imprimir suma por columnas
    salida << "Total|";
    for (int j = 0; j < num_secuencias; ++j)
        salida << " " << setw(3) << suma_col[j] << " |";
    salida << "\n";
    salida << "\nAlineamientos individuales con la secuencia centro (Secuencia " << centro+1 << "):\n";
    for (int i = 0; i < num_secuencias; ++i) {
        salida << "Secuencia " << i+1 << ": " << alineadas[i] << "\n";
    }

    salida << "\nAlineamiento multiple final:\n";
    for (auto& a : alineadas)
        salida << a << "\n";

    salida << "\nAlineamiento múltiple final (formato tabular):\n";
    
    for (int i = 0; i < num_secuencias; ++i) {
        salida << "S" << i + 1 << " | ";
        for (char c : alineadas[i]) {
            salida << c << " ";
        }
        salida << "\n";
    }
    salida.close();
    cout << "\nAlineamiento estrella guardado en 'alineamiento_estrella.txt'.\n";
    return 0;
}
