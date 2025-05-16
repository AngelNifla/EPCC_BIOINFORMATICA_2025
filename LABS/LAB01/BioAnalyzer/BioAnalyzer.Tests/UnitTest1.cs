using Xunit;

public class BioStringAnalyzerTests
{
    [Theory]
    [InlineData("ATCG", "ADN")]
    [InlineData("AUCG", "ARN")]
    [InlineData("ARNDCEQGHILKMFPSTWYV", "Proteína - Aminoácidos: ARNDCEQGHILKMFPSTWYV")]
    [InlineData("ATUG", "Cadena no válida")]
    [InlineData("", "Cadena vacía o nula")]
    public void TestIdentificarCadena(string input, string expected)
    {
        string result = BioStringAnalyzer.IdentificarCadena(input);
        Assert.Equal(expected, result);
    }
}
/*
public class BioStringAnalyzerTests
{
    [Theory]
    [InlineData("", "Cadena vacía o nula")]
    [InlineData("ATCG", "Tipo: ADN")]
    [InlineData("AUGC", "Tipo: ARN")]
    [InlineData("ATUG", "Cadena no válida (mezcla incorrecta o caracteres desconocidos)")]
    public void TestIdentificarCadenaBasica(string input, string expected)
    {
        string result = BioStringAnalyzer.IdentificarCadena(input);
        Assert.Equal(expected, result);
    }

    [Fact]
    public void TestProteinaDeteccion()
    {
        string input = "ARNDCEQGHILKMFPSTVWY";
        string result = BioStringAnalyzer.IdentificarCadena(input);

        Assert.Contains("Tipo: Proteína", result);
        Assert.Contains("- A (Alanina): 1", result);
        Assert.Contains("- R (Arginina): 1", result);
        Assert.Contains("- Y (Tirosina): 1", result);
        Assert.Contains("Aminoácidos encontrados:", result);
    }
}*/
