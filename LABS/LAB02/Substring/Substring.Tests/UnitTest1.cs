using Xunit;

public class SubstringAnalyzerTests
{
    [Theory]
    [InlineData("ATCGTACGAGTCGATCGA", "TACGAGT", "La segunda secuencia es substring de la primera.")]
    [InlineData("GGCATGCTAGCTAGTACG", "GGCATG", "La segunda secuencia es substring de la primera.")]
    [InlineData("CGGATCGTAGCATGCTAA", "TAGCATGCT", "La segunda secuencia es substring de la primera.")]
    [InlineData("AACGTAGCTAGCTAGCTA", "AACGTAGCTAGC", "La segunda secuencia es substring de la primera.")]
    [InlineData("AGCTAGCTAGCTA", "GCTA", "La segunda secuencia es substring de la primera.")]
    [InlineData("AUGCUUAGCGUACGUA", "CGUACGUA", "La segunda secuencia es substring de la primera.")]
    [InlineData("UUACGAUGCAUGCUAGC", "AUGCUAG", "La segunda secuencia es substring de la primera.")]
    [InlineData("CGUAGCUGACGUAGCUGUAUGAUAGUAGUAG", "GACGUAG", "La segunda secuencia es substring de la primera.")]
    [InlineData("ARNDCEQGHILKMFPSTVWYCEQILH", "CEQGHIL", "La segunda secuencia es substring de la primera.")]
    [InlineData("ILKMFPSTVWYARNDCEQGHTVYYW", "STVWY", "La segunda secuencia es substring de la primera.")]
    [InlineData("FPSTVWYARNDCEQGHILKM", "ARNDC", "La segunda secuencia es substring de la primera.")]
    [InlineData("AAAAAATTTTTCCCCCACTAGGGGGCCCCCTTTTT", "GGGGGCCC", "La segunda secuencia es substring de la primera.")]
    [InlineData("UUUUUUAAAAAAGGGGGA", "GGGGGA", "La segunda secuencia es substring de la primera.")]
    [InlineData("MFPSTVWYARNDCEQGHILK", "MFPSTV", "La segunda secuencia es substring de la primera.")]
    [InlineData("ATCGTACGAGTCGATCGACTCTCTAA", "CTAGCTA", "Ninguna secuencia es substring de la otra.")]
    [InlineData("GGCATGCTAGCTAGTACG", "UUUUUUUU", "Ninguna secuencia es substring de la otra.")]
    [InlineData("ARNDCEQGHILKMFPSTVWY", "XYZ", "Ninguna secuencia es substring de la otra.")]
    [InlineData("ACGUACGUACGUACGUTTGUATGAU", "TTATT", "Ninguna secuencia es substring de la otra.")]
    [InlineData("GATUGATGGTUGGUUAGGGGGGGGAGGGG", "GGGGA", "La segunda secuencia es substring de la primera.")]
    [InlineData("ATCGTACGAGTCGATCGA", "ATCGTACGAGTCGATCGA", "Las secuencias son substring entre ambas.")]
    [InlineData("TTTTTTTTTTTT", "TTTTTT", "La segunda secuencia es substring de la primera.")]
    [InlineData("CGUAGCUGACGUAGCU", "CGUAGCUGACGUAGCU", "Las secuencias son substring entre ambas.")]
    [InlineData("MFPSTVWYARNDCEQGHILK", "MFPSTVWYARNDCEQGHILK", "Las secuencias son substring entre ambas.")]
    [InlineData("", "", "Una o ambas secuencias están vacías o son nulas.")]
    [InlineData("ACGTACGTACGT", "", "Una o ambas secuencias están vacías o son nulas.")]
    [InlineData("", "AUGCUA", "Una o ambas secuencias están vacías o son nulas.")]
    [InlineData("ACGTACGT", "ACGTACGTAATUGTTAUUGGTTTAA", "La primera secuencia es substring de la segunda.")]
    [InlineData("AUGCUAGCUAGCUAGC", "AUG", "La segunda secuencia es substring de la primera.")]
    [InlineData("ACGUACGUACGUACGU", "GUACGUAC", "La segunda secuencia es substring de la primera.")]
    [InlineData("GATTACAGATTACAGATTA", "ACAGATTA", "La segunda secuencia es substring de la primera.")]
    [InlineData("AAGGTTCC", "AAGGTTCCAA", "La primera secuencia es substring de la segunda.")]
    [InlineData("CGTACGTACGTA", "GTACG", "La segunda secuencia es substring de la primera.")]
    [InlineData("AUGGCUACGAUGCG", "AUGGCUAC", "La segunda secuencia es substring de la primera.")]
    [InlineData("PROTEINARNDCEQGH", "CEQGH", "La segunda secuencia es substring de la primera.")]
    [InlineData("UUUAGCAUGACGUAAGCT", "AUGAC", "La segunda secuencia es substring de la primera.")]
    [InlineData("UUUAGCAUGACGUAAGCT", "AUGAC", "La segunda secuencia es substring de la primera.")]
    public void TestVerificarSubstring(string seq1, string seq2, string esperado)
    {
        //string resultado = SubstringAnalyzer.VerificarSubstring(seq1, seq2);
        string resultado = SubstringAnalyzerKMP.VerificarSubstring(seq1, seq2);
        Assert.Equal(esperado, resultado);
    }
}