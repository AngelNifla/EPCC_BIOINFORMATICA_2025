/*using System;
using System.Linq;

public class BioStringAnalyzer
{
    private static readonly char[] ADN = { 'A', 'C', 'G', 'T' };
    private static readonly char[] ARN = { 'A', 'C', 'G', 'U' };
    private static readonly char[] PROTEINA = {
        'A', 'R', 'N', 'D', 'C', 'E', 'Q', 'G', 'H',
        'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V'
    };

    public static string IdentificarCadena(string cadena)
    {
        if (string.IsNullOrWhiteSpace(cadena))
            return "Cadena vacía o nula";

        cadena = cadena.ToUpper();

        if (cadena.All(c => ADN.Contains(c)))
            return "ADN";

        if (cadena.All(c => ARN.Contains(c)))
            return "ARN";

        if (cadena.All(c => PROTEINA.Contains(c)))
            return $"Proteína - Aminoácidos: {cadena}";

        return "Cadena no válida";
    }

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("Introduce la cadena:");
        string entrada = Console.ReadLine();
        string resultado = BioStringAnalyzer.IdentificarCadena(entrada);
        Console.WriteLine($"Resultado: {resultado}");
    }
}
}*/
//////////////////////////////////////////////////////////////////////////////////////////////
///
///*
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

public class FastaProcessor
{
    public static void ProcesarArchivoFASTA(string rutaEntrada, string rutaSalida)
    {
        if (!File.Exists(rutaEntrada))
        {
            Console.WriteLine("Archivo de entrada no encontrado.");
            return;
        }

        var lineas = File.ReadAllLines(rutaEntrada);
        StringBuilder salida = new StringBuilder();

        string nombreSecuencia = "";
        StringBuilder secuencia = new();

        foreach (string linea in lineas)
        {
            if (linea.StartsWith(">"))
            {
                // Procesa la secuencia anterior si hay una
                if (secuencia.Length > 0)
                {
                    salida.AppendLine($"Secuencia: {nombreSecuencia}");
                    salida.AppendLine(BioStringAnalyzer.IdentificarCadena(secuencia.ToString()));
                    salida.AppendLine();
                    secuencia.Clear();
                }
                nombreSecuencia = linea.Substring(1).Trim(); // quita el '>'
            }
            else
            {
                secuencia.Append(linea.Trim());
            }
        }

        // Procesa la última secuencia
        if (secuencia.Length > 0)
        {
            salida.AppendLine($"Secuencia: {nombreSecuencia}");
            salida.AppendLine(BioStringAnalyzer.IdentificarCadena(secuencia.ToString()));
        }

        // Escribe el resultado al archivo de salida
        File.WriteAllText(rutaSalida, salida.ToString());
        Console.WriteLine("Procesamiento terminado. Revisa el archivo de salida.");
    }
}

public class BioStringAnalyzer
{
    private static readonly char[] ADN = { 'A', 'C', 'G', 'T' };
    private static readonly char[] ARN = { 'A', 'C', 'G', 'U' };
    private static readonly char[] PROTEINA = {
        'A', 'R', 'N', 'D', 'C', 'E', 'Q', 'G', 'H',
        'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V'
    };

    private static readonly Dictionary<char, string> NombreAminoacido = new()
    {
        { 'A', "Alanina" },   { 'R', "Arginina" }, { 'N', "Asparagina" }, { 'D', "Ácido aspártico" },
        { 'C', "Cisteína" },  { 'E', "Ácido glutámico" }, { 'Q', "Glutamina" }, { 'G', "Glicina" },
        { 'H', "Histidina" }, { 'I', "Isoleucina" }, { 'L', "Leucina" }, { 'K', "Lisina" },
        { 'M', "Metionina" }, { 'F', "Fenilalanina" }, { 'P', "Prolina" }, { 'S', "Serina" },
        { 'T', "Treonina" },  { 'W', "Triptófano" }, { 'Y', "Tirosina" }, { 'V', "Valina" }
    };

    public static string IdentificarCadena(string cadena)
    {
        if (string.IsNullOrWhiteSpace(cadena))
            return "Cadena vacía o nula";

        cadena = cadena.ToUpper();

        if (cadena.All(c => ADN.Contains(c)))
            return "Tipo: ADN";

        if (cadena.All(c => ARN.Contains(c)))
            return "Tipo: ARN";

        if (cadena.All(c => PROTEINA.Contains(c)))
        {
            var conteo = new Dictionary<char, int>();

            foreach (char c in cadena)
            {
                if (!conteo.ContainsKey(c))
                    conteo[c] = 0;
                conteo[c]++;
            }

            string resultado = "Tipo: Proteína\nAminoácidos encontrados:";
            foreach (var kv in conteo.OrderBy(k => k.Key))
            {
                string nombre = NombreAminoacido[kv.Key];
                resultado += $"\n- {kv.Key} ({nombre}): {kv.Value} vez/veces";
            }

            return resultado;
        }

        return "Cadena no válida (mezcla incorrecta)";
    }
}

class Program
{
    static void Main(string[] args)
    {
        string rutaEntrada = "Datos/fasta1.txt";
        string rutaSalida = "Datos/output.txt";

        FastaProcessor.ProcesarArchivoFASTA(rutaEntrada, rutaSalida);
    }
}


*/