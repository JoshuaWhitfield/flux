package syntax.lexer;

import java.util.function.BooleanSupplier;
import java.util.function.Supplier;
import java.util.Arrays;

import functional.control_flow.Bincase;
import functional.control_flow.FFor;
import functional.models.IterNode;
import syntax.lexer.ObjectCategories.ObjectCategory;
import syntax.tokens.headers.LexerToken;

public class Operation {
    
    Boolean match_object(
        ObjectCategory[] objects,
        String input,
        LexerToken[] output,
        Integer current_line,
        Runnable consume_chars
    ) {

        return Bincase.<Boolean>bincase(
            (BooleanSupplier) () -> { return objects.length != 0; },
            (Supplier<Boolean>) () -> {
                Boolean results = FFor.<ObjectCategories.ObjectCategory, Boolean>ffor(
                    Arrays.asList(objects), 
                    (IterNode node) -> {
                        return true;
                    },
                    -1,
                    1
                );
                return true;
            },
            (Supplier<Boolean>) () -> { return true; }
        );


        return false;
    }

}
