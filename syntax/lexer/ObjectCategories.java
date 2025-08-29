package syntax.lexer;
import java.util.*;
import static syntax.types.LexerType.*;

public class ObjectCategories {
    // Define the record INSIDE the class
    public static class ObjectCategory {
        public final String text;
        public final String type;
        
        public ObjectCategory(
            String _text,
            String _type
        ) {
            this.text = _text;
            this.type = _type;
        }
    }
    
    // Now use it in your static lists
    public static final List<ObjectCategory> DECLARATIVES = Arrays.asList(
        new ObjectCategory("fif", DECL()),
        new ObjectCategory("ffor", DECL()),
        new ObjectCategory("fwhile", DECL()),
        new ObjectCategory("fim", DECL()),
        new ObjectCategory("feq", DECL()),
        new ObjectCategory("fneq", DECL()),
        new ObjectCategory("fgeq", DECL()),
        new ObjectCategory("fleq", DECL()),
        new ObjectCategory("fgt", DECL()),
        new ObjectCategory("flt", DECL()),
        new ObjectCategory("fpeq", DECL()),
        new ObjectCategory("fseq", DECL()),
        new ObjectCategory("fmeq", DECL()),
        new ObjectCategory("fdeq", DECL()),
        new ObjectCategory("fmdeq", DECL()),
        new ObjectCategory("fxeq", DECL()),
        new ObjectCategory("fmin", DECL()),
        new ObjectCategory("fadd", DECL()),
        new ObjectCategory("fmul", DECL()),
        new ObjectCategory("fdiv", DECL()),
        new ObjectCategory("fmod", DECL()),
        new ObjectCategory("fexp", DECL()),
        new ObjectCategory("finc", DECL()),
        new ObjectCategory("fdec", DECL()),
        new ObjectCategory("bincase", DECL()),
        new ObjectCategory("assert", DECL()),
        new ObjectCategory("callable", DECL()),
        new ObjectCategory("type", DECL()),
        new ObjectCategory("fcat", DECL())
    );
    
    public static final List<ObjectCategory> DECL_METHODS = Arrays.asList(
        new ObjectCategory(".ffor", DECL_METHOD()),   // functor map operation w/ backtracking
        new ObjectCategory(".ffltr", DECL_METHOD()),  // functor filter by comparator expression
        new ObjectCategory(".fred", DECL_METHOD()),   // functor reduce values
        new ObjectCategory(".ffind", DECL_METHOD()),  // functional find first-match predicate functor
        new ObjectCategory(".fallm", DECL_METHOD()),  // functional all match predicate functor
        new ObjectCategory(".fnonm", DECL_METHOD()),  // functional none match predicate functor
        new ObjectCategory(".fsort", DECL_METHOD()),  // functional reorder by comparator
        new ObjectCategory(".fmini", DECL_METHOD()),  // functional get maximum from set w/ functor
        new ObjectCategory(".fmaxi", DECL_METHOD()),  // functional get minimum from set w/ functor
        new ObjectCategory(".last", DECL_METHOD()),   // functional get last array index (every type is iterable)
        new ObjectCategory(".first", DECL_METHOD()),  // functional get first array index (every type is iterable)
        new ObjectCategory(".pipe", DECL_METHOD())    // functional operate on one value with chain of functions. produce value. requires new iterator
    );
    
    public static final List<ObjectCategory> GRAPHS = Arrays.asList(
        new ObjectCategory("runtime", GRAPH_REF()),
        new ObjectCategory("express", GRAPH_REF()),
        new ObjectCategory("deserial", GRAPH_REF()),
        new ObjectCategory("serial", GRAPH_REF()),
        new ObjectCategory("write", GRAPH_REF())
        /* non forward-facing graphs 
        new ObjectCategory("depend", GRAPH_REF()),
        new ObjectCategory("namespace", GRAPH_REF()),
        new ObjectCategory("scope", GRAPH_REF())
        */
    );
    
    public static final List<ObjectCategory> CORE_FUNCS = Arrays.asList(
        new ObjectCategory("print", CORE_FUNC()),  // print statement to std::cout
        new ObjectCategory("input", CORE_FUNC()),  // input statement to std::cin
        new ObjectCategory("len", CORE_FUNC()),    // length statement returned
        new ObjectCategory("type", CORE_FUNC()),   // type statement returned
        new ObjectCategory("panic", CORE_FUNC())   // exit statement with std::exit(0)
    );
    
    public static final List<ObjectCategory> NORMALIZERS = Arrays.asList(
        new ObjectCategory("constr", NORMALIZER()), // class constructor (function in frontend)
        new ObjectCategory("not", NORMALIZER()),    // evaluates the opposite of the truthiness of its passed parameter
        new ObjectCategory("det", NORMALIZER()),    // evaluates the direct truthiness of its passed parameter
        new ObjectCategory("new", NORMALIZER())     // new normalizes class from definition to a live object instance
    );
    
    public static final List<ObjectCategory> KEYWORDS = Arrays.asList(
        new ObjectCategory("return", KEYWORD()),   // return statement (optional do to assert() variation)
        new ObjectCategory("case", KEYWORD()),     // case: function call under hood (ffltr), linear switch case
        new ObjectCategory("concede", KEYWORD())   // in the case selectors placed in params return falsey, you may concede (void return)
        // new ObjectCategory("default", KEYWORD()) // default case unnecessary, just goes last
    );
    
    public static final List<ObjectCategory> SELECTORS = Arrays.asList(
        new ObjectCategory("or", SELECTOR()),   // allows 'or' selection of values by truthiness. bitwise with selector(..., true) (function frontend & backend)
        new ObjectCategory("and", SELECTOR()),  // allows 'and' selection of values by truthiness. bitwise with selector(..., true) (function frontend & backend)
        new ObjectCategory("xor", SELECTOR())   // allows 'xor' selection of values by truthiness. bitwise with selector(..., true) (function frontend & backend)
    );
    
    public static final List<ObjectCategory> FUNCTIONS = Arrays.asList(
        new ObjectCategory("func ", FUNC()),     // standard function declaration
        new ObjectCategory("=>", ANON_FUNC())    // anonymous function declaration
    );
    
    public static final List<ObjectCategory> IDENTIFIERS = Arrays.asList( // custom
        new ObjectCategory("const ", CONST()),   // constant identifier declaration
        new ObjectCategory("let ", LET())        // mutable identifier declaration
    );
    
    public static final List<ObjectCategory> ASSIGNMENTS = Arrays.asList(
        new ObjectCategory("=", ASSIGN()),       // scope assignment operator
        new ObjectCategory(":", ASSIGN())        // object assignment operator
    );
    
    public static final List<ObjectCategory> BOOLEANS = Arrays.asList(
        new ObjectCategory("true", BOOL()),      // true boolean
        new ObjectCategory("false", BOOL())      // false boolean
    );

    public static final List<ObjectCategory> CONTAINERS = Arrays.asList(
        new ObjectCategory("[", ARRAY_START()),      // array start
        new ObjectCategory("]", ARRAY_END()),      // array end
        new ObjectCategory("{", OBJ_START()),      // object start
        new ObjectCategory("}", OBJ_END()),      // object end
        new ObjectCategory("(", TUPLE_START()),      // tuple start
        new ObjectCategory(")", TUPLE_END()),      // tuple end
        new ObjectCategory(",", COMMA())      // comma
    );
}
