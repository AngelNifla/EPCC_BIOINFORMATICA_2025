using Xunit;

public class BioStringAnalyzerTests
{
    [Theory]
    [InlineData("ATCG", "ADN")]
    [InlineData("AUCG", "ARN")]
    [InlineData("ARNDCEQGHILKMFPSTWYV", "Prote�na - Amino�cidos: ARNDCEQGHILKMFPSTWYV")]
    [InlineData("ATUG", "Cadena no v�lida")]
    [InlineData("", "Cadena vac�a o nula")]
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
    [InlineData("", "Cadena vac�a o nula")]
    [InlineData("ATCG", "Tipo: ADN")]
    [InlineData("AUGC", "Tipo: ARN")]
    [InlineData("ATUG", "Cadena no v�lida (mezcla incorrecta o caracteres desconocidos)")]
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

        Assert.Contains("Tipo: Prote�na", result);
        Assert.Contains("- A (Alanina): 1", result);
        Assert.Contains("- R (Arginina): 1", result);
        Assert.Contains("- Y (Tirosina): 1", result);
        Assert.Contains("Amino�cidos encontrados:", result);
    }
}*/
