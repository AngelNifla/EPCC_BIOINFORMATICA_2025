using System;


public static class SubstringAnalyzerKMP
{
    public static string VerificarSubstring(string secuencia1, string secuencia2)
    {
        if (string.IsNullOrEmpty(secuencia1) || string.IsNullOrEmpty(secuencia2))
            return "Una o ambas secuencias están vacías o son nulas.";

        if (secuencia1.Length > secuencia2.Length)
            if (KmpBuscar(secuencia1, secuencia2))
                return "La segunda secuencia es substring de la primera.";
            else return "Ninguna secuencia es substring de la otra.";
        else if (secuencia2.Length > secuencia1.Length)
            if (KmpBuscar(secuencia2, secuencia1))
                return "La primera secuencia es substring de la segunda.";
            else return "Ninguna secuencia es substring de la otra.";
        else
            if (KmpBuscar(secuencia1, secuencia2))
                return "Las secuencias son substring entre ambas.";
            else return "Ninguna secuencia es substring de la otra.";
    }

    // Algoritmo de Knuth-Morris-Pratt (KMP)
    private static bool KmpBuscar(string texto, string patron)
    {
        int n = texto.Length;
        int m = patron.Length;

        if (m == 0) return true;

        int[] lps = ConstruirLPS(patron);
        int i = 0; // índice para texto
        int j = 0; // índice para patrón

        while (i < n)
        {
            if (texto[i] == patron[j])
            {
                i++; j++;
                if (j == m) return true; // patrón completo encontrado
            }
            else
            {
                if (j != 0)
                    j = lps[j - 1];
                else
                    i++;
            }
        }

        return false;
    }

    // Construir el arreglo LPS
    private static int[] ConstruirLPS(string patron)
    {
        int m = patron.Length;
        int[] lps = new int[m];
        int len = 0;
        int i = 1;

        while (i < m)
        {
            if (patron[i] == patron[len])
            {
                len++;
                lps[i] = len;
                i++;
            }
            else
            {
                if (len != 0)
                    len = lps[len - 1];
                else
                {
                    lps[i] = 0;
                    i++;
                }
            }
        }

        return lps;
    }
}


class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("Ingrese la primera secuencia:");
        string secuencia1 = Console.ReadLine()?.Trim().ToUpper();

        Console.WriteLine("Ingrese la segunda secuencia:");
        string secuencia2 = Console.ReadLine()?.Trim().ToUpper();

        //string resultado = SubstringAnalyzer.VerificarSubstring(secuencia1, secuencia2);
        string resultado = SubstringAnalyzerKMP.VerificarSubstring(secuencia1, secuencia2);
        Console.WriteLine(resultado);
    }
}