// used for precomputing numerical operations.
// used for uniform function/call representation
// used to differentiate between method calls and attribute lookups

/*

    Flux Source (.fx)

User writes high-level Flux code.

Parser/Lexer → Abstract Syntax Tree (AST).

Intermediate Representation (IR)

AST lowered into IR (your “engine-friendly” format).

This IR is what the engine executes.

Engine Core Execution

The engine consumes IR instructions one by one (or in batches).

This is usually where you’d have your main loop (e.g. a while (running) loop).

Inside that loop, you might:

Dispatch IR opcodes to engine core functions (control flow, evaluation, memory mgmt).

Call into the Flux standard library (math, string ops, etc).

Potentially reach into C++ STL when you want optimized containers, algorithms, etc.

*/