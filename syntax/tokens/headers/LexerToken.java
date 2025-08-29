package syntax.tokens.headers;

public class LexerToken {
    String value;
    String type;
    Integer line_number;

    LexerToken(
        String _value,
        String _type,
        Integer _line_number
    ) {
        this.value = _value;
        this.type = _type;
        this.line_number = _line_number;
    }

    public String get_value() { return this.value; }
    public String get_type()  { return this.type; }
    public Integer get_line() { return this.line_number; }
}
