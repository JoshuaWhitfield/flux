package syntax.lexer;

import functional.controlFlow.Bincase;
import functional.controlFlow.Fif;
import functional.controlFlow.FFor;
import functional.controlFlow.FFltr;
import syntax.tokens.LexerToken;
import functional.models.Extractor;
import functional.models.IterNode;

import java.util.*;
import java.util.function.Consumer;

public final class Operation {
    private Operation() {} // no instances

    public static <T extends ObjectCategories.ObjectCategory> boolean matchObject(
        List<T> objects,
        StringBuilder input,
        List<LexerToken> tokenOutput,
        int currentLine,
        Consumer<Integer> consumeChars
    ) {
        return Bincase.<Boolean>bincase(
            () -> { return !objects.isEmpty(); },     // BooleanSupplier ✅
            () -> {                       // Supplier<Boolean> ✅
                List<T> results = FFor.<T>ffor(
                    objects,
                    node -> {
                        Optional<T> elemOpt = Extractor.extractEdge(node, "elem");
                        if (!elemOpt.isPresent()) return null;  // or elemOpt.get() depending on context

                        T elem = elemOpt.get();

                        if (input.length() < elem.text.length()) return null;
                        if (!input.substring(0, elem.text.length()).equals(elem.text)) return null;

                        // Perform side-effects here
                        tokenOutput.add(new LexerToken(elem.text, elem.type, currentLine));
                        consumeChars.accept(elem.text.length());

                        return elem;  // return the element itself
                    },
                    -1,
                    1
                );

                List<T> filtered = FFltr.ffltr(
                    results,
                    (n, elem) -> elem != null ? elem : null, // <-- return T or null
                    new IterNode()
                );

                return Fif.fif(() -> !filtered.isEmpty(), () -> true, () -> false);
            },
            () -> false                     // Supplier<Boolean> ✅
        );


    }
}
