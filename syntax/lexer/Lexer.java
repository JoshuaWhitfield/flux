package syntax.lexer;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.function.BooleanSupplier;
import java.util.function.Consumer;
import java.util.regex.*;

import functional.controlFlow.Fif;
import functional.models.Extractor;
import functional.models.IterEdge;
import functional.models.IterNode;
import functional.controlFlow.Bincase;
import functional.controlFlow.FFor;
import functional.controlFlow.FWhile;
import syntax.lexer.ObjectCategories.ObjectCategory;
import syntax.tokens.LexerToken;
import syntax.types.LexerType;
import util.StringUtils;


public class Lexer {
    
    StringBuilder input;
    List<LexerToken> output;
    Integer current_line;

    private Boolean add_token(LexerToken token) {
        output.add(token);
        return true;
    }

    private Optional<Character> get_current() {
        return Fif.fif(
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
    private void consume_chars(int n) {
        int lengthToRemove = Math.min(n, input.length());
        input.delete(0, lengthToRemove);
    }

    // Set new input and reset current line
    public void set_input(String newInput) {
        input = new StringBuilder(newInput);
        current_line = 0;
    }

    public Optional<LexerToken> tokenize() {
        return FWhile.fwhile(
            () -> { return input.length() != 0; },
            () -> {
                return Bincase.<Boolean>bincase(
                    () -> { return get_current().isPresent(); },
                    () -> {
                        // skip whitespace
                        return Fif.<Boolean>fif(
                            () -> { return input.charAt(0) == ' ' || input.charAt(0) == '\t'; },
                            () -> { consume(); return true; },
                            () -> {
                                // capture newline and statement end
                                return Fif.<Boolean>fif(
                                    () -> { return input.charAt(0) == '\n' || input.charAt(0) == ';'; },
                                    () -> {
                                        add_token(new LexerToken(Character.toString(input.charAt(0)), LexerType.ENDL(), current_line++));
                                        consume(); return true;
                                    },
                                    () -> {
                                        // capture multi-line comments
                                        return Fif.<Boolean>fif(
                                            () -> { return input.length() >= 2 && input.charAt(0) == '/' && input.charAt(1) == '*'; },
                                            () -> {
                                                Integer char_count = 2;
                                                Integer newline_count = 0;
                                                FWhile.fwhile(
                                                    () -> { return (char_count + 1 < input.length()) && (!(input.charAt(char_count) == '*' && input.charAt(char_count + 1) == '/')); },
                                                    () -> {
                                                        return Fif.<Boolean>fif(
                                                            () -> { return input.charAt(char_count) == '\n'; },
                                                            () -> {
                                                                newline_count++;
                                                                return true;
                                                            },
                                                            () -> {
                                                                char_count++;
                                                                return false;
                                                            }
                                                        );
                                                    }
                                                );

                                                add_token(new LexerToken("/*", LexerType.COMMENT_START(), current_line));
                                                add_token(new LexerToken(StringUtils.slice(input, 2, char_count).toString(), LexerType.COMMENT(), current_line));
                                                current_line += newline_count;
                                                return Fif.<Boolean>fif(
                                                    () -> { return ((char_count = 1) < input.length()); },
                                                    () -> {
                                                        add_token(new LexerToken("*/", LexerType.COMMENT_END(), current_line));
                                                        consume_chars(char_count + 2);
                                                        return true;
                                                    },
                                                    () -> {
                                                        consume_chars(input.length());
                                                        return false;
                                                    }
                                                );
                                            },
                                            () -> {
                                                // capture single line comments
                                                return Fif.<Boolean>fif(
                                                    () -> { return (input.length() >= 2) && (input.charAt(0) == '/' && input.charAt(1) == '/'); },
                                                    () -> {
                                                        Integer char_count;
                                                        List<Integer> indices = new ArrayList<>();

                                                        for (; char_count < input.length(); char_count++) { indices.add(char_count); }
                                                        
                                                        FFor.<Integer>ffor(
                                                            indices,
                                                            (IterNode node) -> {
                                                                Optional<IterEdge<Integer>> indexOpt = Extractor.extractEdge(node, "index");
                                                                IterEdge<Integer> index;
                                                                return Fif.<Integer>fif(
                                                                    () -> { return indexOpt.isPresent(); },
                                                                    () -> {
                                                                        index = indexOpt.get();
                                                                        return 1;
                                                                    },  
                                                                    () -> { return 0; } // will never run
                                                                );
                                                            },
                                                            -1,
                                                            1
                                                        );
                                                        
                                                        add_token(new LexerToken("//", LexerType.COMMENT_START(), current_line));
                                                        add_token(new LexerToken(StringUtils.slice(input, 2, char_count).toString(), LexerType.COMMENT(), current_line))
                                                        consume_chars(char_count);
                                                        return true;
                                                    },
                                                    () -> {
                                                        // capture strings 
                                                        return Fif.<Boolean>fif(
                                                            () -> { return input.charAt(0) == '"' || input.charAt(0) == '\''; },
                                                            () -> {
                                                                Pattern pattern = Pattern.compile("\"(.*?)\"|'(.*?)'"); // same regex
                                                                Matcher matcher = pattern.matcher(input.toString());

                                                                return Fif.<Boolean>fif(
                                                                    () -> { return matcher.find() && matcher.start() == 0; },
                                                                    () -> {
                                                                        String str = Fif.<String>fif(
                                                                            () -> { return matcher.group(1) != null; },
                                                                            () -> { return matcher.group(1); },
                                                                            () -> { return matcher.group(2); }
                                                                        );
                                                                        add_token(new LexerToken(str, LexerType.STRING(), current_line));
                                                                        consume_chars(str.length() + 2); // +2 for the quotes
                                                                        return true;
                                                                    },
                                                                    () -> { return false; } // UNEXP() ?
                                                                );

                                                            },
                                                            () -> {
                                                                // capture floats and integers
                                                                return Bincase.<Boolean>bincase(
                                                                    () -> { return Integer.valueOf(Character.toString(input.charAt(0))) instanceof Integer || input.charAt(0) == '.'; },
                                                                    () -> {
                                                                        Pattern float_pattern = Pattern.compile("(\\d*\\.\\d+)");
                                                                        Matcher float_matcher = float_pattern.matcher(input.toString());
                                                                        
                                                                        Pattern int_pattern = Pattern.compile("(\\d+)");
                                                                        Matcher int_matcher = int_pattern.matcher(input.toString());

                                                                        return Fif.<Boolean>fif(
                                                                            () -> { return float_matcher.find() && float_matcher.start() == 0; },
                                                                            () -> {
                                                                                add_token(new LexerToken(float_matcher.group(0), LexerType.FLOAT(), current_line));
                                                                                consume_chars(float_matcher.group(0).length());
                                                                                return true;
                                                                            },
                                                                            () -> {
                                                                                add_token(new LexerToken(int_matcher.group(0), LexerType.INTEGER(), current_line));
                                                                                consume_chars(int_matcher.group(0).length());
                                                                                return false;
                                                                            }
                                                                        );

                                                                    },
                                                                    () -> {
                                                                        return Fif.<Boolean>fif(
                                                                            () -> {
                                                                                return Operation.matchObject(
                                                                                    ObjectCategories.BOOLEANS,
                                                                                    input,
                                                                                    output,
                                                                                    current_line,
                                                                                    (n) -> consume_chars(n)
                                                                                );
                                                                            },
                                                                            () -> { return true; },
                                                                            () -> {
                                                                                return Fif.<Boolean>fif(
                                                                                    () -> {
                                                                                        return Operation.matchObject(
                                                                                            ObjectCategories.ASSIGNMENTS,
                                                                                            input,
                                                                                            output,
                                                                                            current_line,
                                                                                            (n) -> consume_chars(n)
                                                                                        );
                                                                                    },
                                                                                    () -> { return true; },
                                                                                    () -> {
                                                                                        return Fif.<Boolean>fif(
                                                                                            () -> {
                                                                                                return Operation.matchObject(
                                                                                                    ObjectCategories.SELECTORS,
                                                                                                    input,
                                                                                                    output,
                                                                                                    current_line,
                                                                                                    (n) -> consume_chars(n)
                                                                                                );
                                                                                            },
                                                                                            () -> { return true; },
                                                                                            () -> {
                                                                                                return Fif.<Boolean>fif(
                                                                                                    () -> {
                                                                                                        return Operation.matchObject(
                                                                                                            ObjectCategories.KEYWORDS,
                                                                                                            input,
                                                                                                            output,
                                                                                                            current_line,
                                                                                                            (n) -> consume_chars(n)
                                                                                                        );
                                                                                                    },
                                                                                                    () -> { return true; },
                                                                                                    () -> {
                                                                                                        return Fif.<Boolean>fif(
                                                                                                            () -> {
                                                                                                                return Operation.matchObject(
                                                                                                                    ObjectCategories.NORMALIZERS,
                                                                                                                    input,
                                                                                                                    output,
                                                                                                                    current_line,
                                                                                                                    (n) -> consume_chars(n)
                                                                                                                );
                                                                                                            },
                                                                                                            () -> { return true; },
                                                                                                            () -> {
                                                                                                                return Fif.<Boolean>fif(
                                                                                                                    () -> {
                                                                                                                        return Operation.matchObject(
                                                                                                                            ObjectCategories.CORE_FUNCS,
                                                                                                                            input,
                                                                                                                            output,
                                                                                                                            current_line,
                                                                                                                            (n) -> consume_chars(n)
                                                                                                                        );
                                                                                                                    },
                                                                                                                    () -> { return true; },
                                                                                                                    () -> {
                                                                                                                        return Fif.<Boolean>fif(
                                                                                                                            () -> {
                                                                                                                                return Operation.matchObject(
                                                                                                                                    ObjectCategories.GRAPHS,
                                                                                                                                    input,
                                                                                                                                    output,
                                                                                                                                    current_line,
                                                                                                                                    (n) -> consume_chars(n)
                                                                                                                                );
                                                                                                                            },
                                                                                                                            () -> { return true; },
                                                                                                                            () -> {
                                                                                                                                return Fif.<Boolean>fif(
                                                                                                                                    () -> {
                                                                                                                                        return Operation.matchObject(
                                                                                                                                            ObjectCategories.DECL_METHODS,
                                                                                                                                            input,
                                                                                                                                            output,
                                                                                                                                            current_line,
                                                                                                                                            (n) -> consume_chars(n)
                                                                                                                                        );
                                                                                                                                    },
                                                                                                                                    () -> { return true; },
                                                                                                                                    () -> {
                                                                                                                                        return Fif.<Boolean>fif(
                                                                                                                                            () -> {
                                                                                                                                                return Operation.matchObject(
                                                                                                                                                    ObjectCategories.DECLARATIVES,
                                                                                                                                                    input,
                                                                                                                                                    output,
                                                                                                                                                    current_line,
                                                                                                                                                    (n) -> consume_chars(n)
                                                                                                                                                );
                                                                                                                                            },
                                                                                                                                            () -> { return true; },
                                                                                                                                            () -> {
                                                                                                                                                boolean matched_function = false;
                                                                                                                                                FFor.ffor(
                                                                                                                                                    ObjectCategories.FUNCTIONS,
                                                                                                                                                    (IterNode node) -> {
                                                                                                                                                        Optional<IterEdge<T>> elemOpt = Extractor.extractEdge(node, "elem");
                                                                                                                                                        IterEdge<T> elem;
                                                                                                                                                        
                                                                                                                                                        Fif.<Boolean>fif(
                                                                                                                                                            () -> { return elemOpt.isPresent(); },
                                                                                                                                                            () -> {
                                                                                                                                                                elem = elemOpt.get();
                                                                                                                                                                return true;
                                                                                                                                                            },  
                                                                                                                                                            () -> { return true; } // will never run
                                                                                                                                                        );
                                                                                                                                                        
                                                                                                                                                        
                                                                                                                                                        return Bincase.bincase(
                                                                                                                                                            () -> {
                                                                                                                                                                return Fif.fif(
                                                                                                                                                                    () -> { return StringUtils.slice(input, 0, elem.text.size()) == elem.text.size(); },
                                                                                                                                                                    () -> { return true; },
                                                                                                                                                                    () -> { return false; }
                                                                                                                                                                );
                                                                                                                                                            },
                                                                                                                                                            () -> {
                                                                                                                                                                return Fif.fif(
                                                                                                                                                                    () -> { return elem.text == "func "; },
                                                                                                                                                                    () -> {
                                                                                                                                                                        String after_func = StringUtils.slice(input, 0, elem.text.size()).toString();
                                                                                                                                                                        Pattern func_pattern = Pattern.compile("([A-Za-z_][A-Za-z0-9_]*)");
                                                                                                                                                                        Matcher matcher = func_pattern.matcher(input.toString());

                                                                                                                                                                        return Fif.fif(
                                                                                                                                                                            () -> { return matcher.find() && matcher.start() == 0; },
                                                                                                                                                                            () -> {
                                                                                                                                                                                String function = matcher.group(0);
                                                                                                                                                                                add_token(new LexerToken(function, elem.type, current_line));

                                                                                                                                                                                Integer total_matched = elem.text.size() + function.size();
                                                                                                                                                                                consume_chars(total_matched);
                                                                                                                                                                                matched_function = true;
                                                                                                                                                                                return matched_function;
                                                                                                                                                                            },
                                                                                                                                                                            () -> { return false; }
                                                                                                                                                                        );
                                                                                                                                                                    },
                                                                                                                                                                    () -> {
                                                                                                                                                                        add_token(LexerToken(elem.text, elem.type, current_line));
                                                                                                                                                                        consume_chars(2);
                                                                                                                                                                        matched_function = true;
                                                                                                                                                                        return matched_function;
                                                                                                                                                                    }
                                                                                                                                                                );
                                                                                                                                                            },
                                                                                                                                                            () -> { return false; }
                                                                                                                                                        );
                                                                                                                                                    },
                                                                                                                                                    -1, 
                                                                                                                                                    1
                                                                                                                                                );
                                                                                                                                            }
                                                                                                                                        );
                                                                                                                                    }
                                                                                                                                );
                                                                                                                            }
                                                                                                                        );
                                                                                                                    }
                                                                                                                );
                                                                                                            }
                                                                                                        );
                                                                                                    }
                                                                                                );
                                                                                            }
                                                                                        );
                                                                                    }
                                                                                );
                                                                            }
                                                                        );
                                                                    }
                                                                );
                                                            }
                                                        );
                                                    }
                                                );
                                            }
                                        );
                                    }
                                );
                            }
                        );
                    },
                    () -> { consume(); return true; }
                );
            }
        );
    }


}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
