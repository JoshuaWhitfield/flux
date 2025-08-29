package functional.controlFlow;

import java.util.*;

public class Normalize {
    
    // Strings
    public static boolean normalize(String s) {
        return s != null && !s.isEmpty();
    }

    // Numbers
    public static boolean normalize(Number n) {
        return n != null && n.doubleValue() != 0.0;
    }

    // Collections
    public static boolean normalize(Collection<?> c) {
        return c != null && !c.isEmpty();
    }

    // Arrays
    public static boolean normalize(Object[] arr) {
        return arr != null && arr.length > 0;
    }

    // Fallback for Objects
    public static boolean normalize(Object o) {
        return o != null;
    }
}
