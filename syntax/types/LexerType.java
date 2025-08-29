package syntax.types;

public class LexerType {
    
    /* ===== SPECIALIZED FUNCTIONS ===== */
    public static String NORMALIZER()  { return "NORMALIZER"; }
    public static String SELECTOR()    { return "SELECTOR"; }
    public static String CORE_FUNC()   { return "CORE_FUNCTION"; }
    public static String DECL_METHOD() { return "DECL_METHOD"; }
    public static String DECL()        { return "DECLARATIVE"; }
    
    /* ===== KEYWORDS ===== */
    public static String KEYWORD() { return "KEYWORD"; }
    public static String CONST()   { return "CONST"; }
    public static String LET()     { return "LET"; }

    /* ===== TERMINATORS ===== */
    public static String ENDL()  { return "END_OF_LINE"; }
    public static String ENDF()  { return "END_OF_FILE"; }
    public static String UNEXP() { return "UNEXPECTED"; }
    
    /* ===== NAMESPACES ===== */
    public static String NAMESPACE()  { return "NAMESPACE"; }
    public static String GRAPH_REF()  { return "GRAPH_REFERENCE"; }
    
    /* ===== LITERALS ===== */
    public static String INTEGER() { return "INTEGER"; }
    public static String FLOAT()   { return "FLOAT"; }
    public static String STRING()  { return "STRING"; }
    public static String BOOL()    { return "BOOL"; }
    
    /* ===== CONTAINERS ===== */
    public static String OBJ_START()   { return "OBJECT_START"; }
    public static String OBJ_END()     { return "OBJECT_END"; }
    public static String ARRAY_START() { return "ARRAY_START"; }
    public static String ARRAY_END()   { return "ARRAY_END"; }
    public static String TUPLE_START() { return "TUPLE_START"; }
    public static String TUPLE_END()   { return "TUPLE_END"; }    

    /* ===== OBJECT PROPERTIES ===== */
    public static String CHILD()     { return "CHILD"; }
    
    /* ===== FUNCTIONS ===== */
    public static String FUNC()      { return "FUNCTION"; }
    public static String ANON_FUNC() { return "ANON_FUNCTION"; }
    
    /* ===== OPERATORS ===== */
    public static String ASSIGN()    { return "ASSIGN"; }
    
    /* ===== COMMENTS ===== */
    public static String COMMENT()       { return "COMMENT"; }
    public static String COMMENT_START() { return "COMMENT_START"; }
    public static String COMMENT_END()   { return "COMMENT_END"; }
    public static String COMMA()         { return "COMMA"; }
    
}
