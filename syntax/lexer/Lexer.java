package syntax.lexer;

import static functional.controlFlow.Fif.fif;

import java.util.List;
import java.util.Optional;
import java.util.function.BooleanSupplier;
import java.util.function.Consumer;

import functional.controlFlow.Fif;
import syntax.tokens.LexerToken;


public class Lexer {
    
    StringBuilder input;
    List<LexerToken> output;
    Integer current_line;

    private Boolean add_token(LexerToken token) {
        output.add(token);
        return true;
    }

    private Optional<Character> get_current() {
        return fif(
            (BooleanSupplier) () -> { return input.length() == 0; }, 
            () -> { return Optional.empty(); },
            () -> { return Optional.of(input.charAt(0)); }    
        );
    }

    private Optional<Character> consume() {
        return Fif.fif(
            () -> { return input.length() == 0; },
            () -> {
                char c = input.charAt(0);
                input.deleteCharAt(0);
                return Optional.of(c);
            },
            () -> { return Optional.empty(); }
        );
    }

    // Consume 'n' characters from the start of the input
    private void consumeChars(int n) {
        int lengthToRemove = Math.min(n, input.length());
        input.delete(0, lengthToRemove);
    }

    // Set new input and reset current line
    public void setInput(String newInput) {
        input = new StringBuilder(newInput);
        current_line = 0;
    }

    public Optional<LexerToken> tokenize() {
        return fwhile() {
            
        }
    }


}
