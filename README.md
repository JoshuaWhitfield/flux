# flux
Flux is a lightweight, declarative, semantically functional programming language designed for clarity, expressiveness, and safety. It combines a minimalistic core with powerful abstractions, enabling developers to write clean, declarative, and highly maintainable code.

Key Features

Functional-first: Functions are central, with expressions like fif, fwhile, and ffor enabling concise control flow and iteration.

Unified type system: Includes FluxObject, FluxArray, FluxTuple, FluxString, FluxInteger, and more for predictable and type-safe operations.

Declarative constructs: Control flow constructs are first-class, enabling stored and reusable logic with clear evaluation rules.

References and memory efficiency: Pass variables by reference to mutate shared state without unnecessary copying.

Single assignment operator: Simplicity at its core, with = as the sole operator for assignment.

Built-in runtime graph: Functions, objects, and arrays exist on a uniform runtime graph for introspection, error checking, and serialization.

Flexible built-ins: Standard functions like print, len, and type are initialized at runtime with parameter checks to prevent misuse.
