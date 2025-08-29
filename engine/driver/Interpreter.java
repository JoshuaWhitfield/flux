package engine.driver;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

import syntax.lexer.Lexer;
import syntax.tokens.LexerToken;
import syntax.types.LexerType;

public class Interpreter {
    
    // Read a .fx file into a string
    private static String readFxFile(String relativePath) {
        try {
            return Files.readString(Paths.get(relativePath));
        } catch (IOException e) {
            System.err.println("Error: Could not open file " + relativePath);
            return "";
        }
    }

    // rite tokens to results file
    private static void writeTokens(List<LexerTokens> tokens, String outPath) {
        try {
            StringBuilder sb = new StringBuilder();
            sb.append("Tokenization results:\n\n");
            for (LexerToken token : tokens) {
                sb.append("Token(text=\"");
                    .append(token.value)
                    .append("\", type=\"")
                    .append(token.type)
                    .append("\", line=\"")
                    .append(token.line)
                    .append("\")\n");
            }
            Files.writeString(Paths.get(outPath), sb.toString());
            System.out.println("Tokens written to" + outPath);
        } catch (IOException e) {
            System.err.println("Error: Could not write to file " + outPath);
        }
    }

    public static void main(String[] args) {
        String inputPath = "test/test.fx";
        String outputPath = "test/results.txt";

        StringBuilder content = new StringBuilder(readFxFile(inputPath));

        if (!content.isEmpty()) {
            Lexer lexer = new Lexer();
            lexer.setInput(content);
            lexer.tokenize();
            writeTokens(lexer.get_output(), outputPath);
        }
    }

}