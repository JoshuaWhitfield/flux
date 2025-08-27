// === Comments ===
// Single-line comment
/* Multi-line
   comment */

// === Declarations ===
const pi = 3.14159;
let counter = 0;

// === Functions ===
func add(a, b) => a fadd b;
func multiply(x, y) => x fmul y;

// === Core functions ===
print("Pi value:");
print(pi);
print(add(2, 3));
print(multiply(4, 5));

// === Declaratives / control flow ===
fif feq(counter, 0) {
    print("Counter is zero");
} else {
    print("Counter is not zero");
}

fwhile flt(counter, 10) {
    counter finc;
}

// === Arrays / Objects / Tuples ===
let numbers = [1, 2, 3, 4];
let user = { name: "Alice", age: 30 };
let pair = (pi, counter);

// === Declarative methods ===
numbers.ffor(fadd);       // add elements functionally
let concatenated = numbers.concat([5, 6]);

// === Assertions / callable / type ===
assert feq(len(numbers), 4);
callable(add);
type(user);

// === References ===
@ref_counter = counter;

// === Selectors ===
let truthy_vals = [1, 0, 2].and;  // selects truthy only
let falsey_vals = [0, null].or;   // selects whichever is truthy
let xor_test = [1, 0].xor;

// === Normalizers ===
let negated = not(true); 
let direct = det(1); 
let new_object = new(); 
let constrained = constr("value");

// === Assignments / children ===
x = 10;
y: 20;
user.name = "Bob";

// === Classes / Namespaces ===
class MyClass {
    func method() => print("Hello");
}

&MyNamespace

// === Graph references / runtime ===
runtime.serialize(user);
runtime.deserialize(user);
depend();
express();
write();

// === End of file ===
