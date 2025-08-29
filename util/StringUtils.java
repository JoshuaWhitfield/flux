package util;

public class StringUtils {

    // Overload for String
    public static String slice(String input, int start, int end) {
        return input.substring(start, end);
    }

    // Overload for StringBuilder
    public static StringBuilder slice(StringBuilder input, int start, int end) {
        return new StringBuilder(input.substring(start, end));
    }

    public static void main(String[] args) {
        String str = "HelloWorld";
        StringBuilder sb = new StringBuilder("HelloWorld");

        String slicedStr = StringUtils.slice(str, 0, 5); // "Hello"
        StringBuilder slicedSb = StringUtils.slice(sb, 5, 10); // "World"

        System.out.println(slicedStr);
        System.out.println(slicedSb);
    }
}
