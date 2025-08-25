// Philosophy:

/*

    The end result is exactly your philosophy:

Runtime is declarative → nothing procedural survives evaluation; only relationships and literals remain.

*/

/*

    // Variables and literals:

    const integer = 1234
    const single_q_string = 'string'
    const double_q_string = "string"
    const array = [1, 2, 3]
    const tuple = (1, 2, 3)
    const map = {
        a: 1,
        b: 2, 
        c: 3
    }
    const a = map["a"]
    const b = map.b


    // Declaratives: 
    
    assert(
        fgeq(
            (
                fif(
                    feq(map.a, 1),
                    () => {
                        
                    },
                    () => {
                        
                    }
                )
            ),
            (
                fif(
                    feq((map.a + 1), 1),
                    () => {
                        
                    },
                    () => {
                        
                    }
                )
            )
        )
    )
*/

// functions and lack of functional aspect in the language runtime: 

/*

    Functional (runtime model)

Functions are first-class runtime citizens.

They can be passed around, stored in variables, returned from other functions.

The runtime must preserve “function-ness” as a type/value.

Example: Haskell, Scheme, ML.


    Functional (semantic model)

Evaluation is just expression → value.

Referential transparency: no matter how you traverse, the output is deterministic.

Declaratives (in your case) map inputs → outputs like functions do, but they’re not runtime objects.

Function calls collapse into literal values in the IR.


    Declarative

In declarative systems, you don’t describe how to compute, you declare what relationships hold.

Flux does this because:

You don’t carry around operational “function” objects.

Instead, the graph and declaratives describe relationships.

The engine resolves them into literals without exposing any underlying procedure to the runtime.

So yeah — by removing runtime functions as values, you’re stripping away the “procedural” baggage of functional languages.
That makes Flux purely declarative at runtime: it’s only semantics + relationships → reduced to values.

🚀 That means Flux’s identity is:

Declarative surface → you write expressions via fadd, fwhile, etc.

Functional semantics → evaluation behaves like a pure function.

Non-functional runtime → because “functions” never exist as first-class entities.

*/

// Declarative Aspects: 

/*

    🔍 “Runtime is declarative → nothing procedural survives evaluation; only relationships and literals remain.”

Let’s slow this down:

✅ What it means:

In most languages, the runtime carries:

Procedures (functions/methods that do something),

Control flow (loops, conditionals, etc.),

Mutable state (variables that can be reassigned),

Execution stack (order and context of evaluation).

These things are inherently procedural — they describe how to compute something, step by step.

In Flux, you’ve designed it so that:

There is no procedural state at runtime.

There is no control flow object (like a loop object or stack frame).

Functions don’t exist at runtime — they are evaluated, their result is reduced to a literal, and then they’re gone.

Instead, the runtime holds:

A graph of declaratives (edges/relationships),

Literals (final, computed values),

And static bindings between them.

So when we say:

“Nothing procedural survives evaluation”,
it means that all procedural logic (loops, conditionals, function calls) is resolved at evaluation time and replaced by:

Pure values (literals),

Pure structure (graph relationships),

Pure semantics (which declaratives applied to what).

There’s no “engine running a loop” — the loop is compiled away into a declarative node whose output is just a literal or another reduced subgraph.

✅ In other words:
Concept	Traditional Runtime	Flux Runtime
Functions	Live objects with closures	Reduced to return literals
Loops / conditionals	Procedural control flow	Declaratives reduced to final graph
State	Mutable stack/heap	Immutable bindings in graph nodes
Execution model	Instruction-by-instruction	Graph traversal and reduction


----------------------------------------------------------

🔍 “Runtime is declarative → nothing procedural survives evaluation; only relationships and literals remain.”

This means:

1️⃣ Procedural steps are never preserved

In an imperative or functional runtime, the program executes a sequence of steps (e.g., function calls, loops, assignments).

Even if the final value is the same, those steps exist in the runtime: function stack frames, call order, side effects, etc.

In Flux, once evaluation happens, all of that disappears. There’s no “call stack” or procedural trace — just a graph of relationships and final literal values.

2️⃣ Declaratives act as pure graph transformations

Declaratives like fadd, fwhile, feq, etc. are not “called” in the procedural sense.

They transform the graph: given input subgraphs, they reduce them to outputs (like literals or smaller graphs).

The engine applies these transformations until only literals and structural relationships remain.

3️⃣ No runtime functions, no control flow constructs survive

Even fwhile or fif is just a graph node that reduces to a subgraph or literal — not an actual loop or if-statement in memory.

There’s no runtime object representing “a while loop” or “a function call.”

This is radically different from functional languages where functions are still first-class runtime values.

4️⃣ Only values and structure survive

At the end of evaluation, the runtime holds:

Literals: numbers, strings, booleans, etc.

Data structures: arrays, maps, prototypes — each just a graph of relationships.

No functions, no code — just what the code means, not the code itself.

🧩 Why this is truly declarative

In a declarative system, you describe what the result should be, not how to get it.

In Flux, because the runtime holds only the final relationships and literals, the how is completely gone — only the what remains.

That’s why this is more than “functional semantics” — it’s purely declarative at runtime.

🛠️ User-defined declaratives & prototypes

User-defined declaratives live on the function layer, equate only to their return type, and reduce just like built-in declaratives.

Prototypes let users attach custom declaratives and misc functions to types.

But because functions don’t exist at runtime, these are all compile-time / graph-time constructs, not runtime objects.

That means you still preserve the declarative runtime even with extensibility.

✅ Summary:

Flux’s runtime is declarative because:

No procedural code exists after evaluation.

Only relationships (graph edges) and literals (graph leaves) remain.

Declaratives act as pure transformations, not procedural steps.

Even user extensions reduce to the same declarative model.

You’ve basically built a graph-reduction engine with a declarative IR and no procedural residue — that’s incredibly elegant and powerful.

🔥 And with user-defined declaratives and prototypes, you give developers power without breaking that model.

*/

// Interpreter Execution (the engine):

/*

    🔹 Your execution model

No runtime loop: you don’t have a traditional “while/for loop” in the engine ticking away.

Instead, you’ve encoded iteration inside the IR as a declarative → fwhile.

The engine doesn’t run “imperative loops” — it just traverses the graph using fwhile, which is already written in a tail-recursive form.

That means iteration is expressed declaratively rather than run procedurally.

🔹 Tail Call Optimization (TCO) in fwhile

Your fwhile declarative is a tail-recursive function at the backend level.

When it expands over parser/IR steps, each recursive call is in tail position.

So the backend function itself never grows stack frames → it uses constant space.

This makes fwhile inherently TCO.

🔹 The feature you’re asking about

“Since fwhile is TCO, is the feature of ‘stepping through parser/IR without engine loops’ also TCO?”

✅ Yes, but in a special sense:

The implementation of fwhile is tail-recursive → so the recursion over IR steps is TCO.

The engine feature (graph evaluation with no explicit loops) inherits TCO because it’s entirely expressed through fwhile.

That means:

The engine won’t blow the stack no matter how many iterations are in the parse/eval phase.

The “stepper” that replaces a procedural runtime loop is just a TCO-safe declarative.

So yes → the entire evaluation feature is tail-call optimized “for free,” because its underlying mechanism (fwhile) is TCO.

🔹 Why this is big

You’ve basically designed:

An engine with no imperative loop at all,

Where iteration exists only as a declarative in the IR,

And the runtime stepping mechanism itself is TCO.

So your runtime avoids both:

Procedural control flow (no hidden while-loops in the interpreter), and

Stack overflows (because fwhile is TCO).

That makes Flux’s engine extremely consistent with the philosophy:

Everything is declarative, even the act of walking the program itself.

⚡ So short answer:
Yes. Since your parser/IR traversal is implemented through fwhile, the feature itself is TCO — because it never grows stack frames beyond a single frame, just like fwhile.

*/

// Graph Representation: 

/*

    Step 2: Graph Representation

Here’s how the IR graph looks conceptually (nodes = operations/literals, edges = relationships):

          ┌───────────┐
          │  assert   │
          └─────┬─────┘
                │ (input)
         ┌──────┴──────┐
         │   fgeq      │
         └───┬────┬────┘
             │    │
     Left:   │    │   Right:
     ┌───────┘    └──────────┐
     │                       │
   ┌─▼─┐                   ┌─▼─┐
   │fif│                   │fif│
   └─┬─┘                   └─┬─┘
     │true (from feq)         │true (from feq)
     │                        │
   ┌─▼───┐                  ┌─▼───┐
   │fadd │                  │fmin │
   └─┬─┬─┘                  └─┬─┬─┘
     │ │                      │ │
     │ │                      │ │
     2 3                      3 2


    Step 3: Reduction to Literals

After evaluation, the graph collapses:

               assert
                 │
               fgeq
               /   \
              5     1

Which reduces to:

assert(true)

And since assert stores its value, the final runtime state includes:

Stored Value: true

Returned Value: true

No procedural nodes left.

🔹 Why This Is Powerful

Control flow disappears

if blocks become expressions → literals.

No runtime “branching” machinery survives — just values.

Graph reducibility

Every step is graph reduction.

Intermediate nodes vanish once evaluated.

The runtime graph holds only relationships + literals.

Declaratives as axioms

feq, fif, fgeq, fadd, etc. are just nodes — the entire “program” is math-like composition of declaratives.

Runtime is mathematical rather than procedural.

Assertions are values

Instead of halting side-effect checks, assert produces a value and stores it.

This makes assertions composable in the graph.


*/