# 🧠 Tiny C Compiler — Flex & Bison Based Intermediate Code Generator

## 📌 Overview

This repository implements a **mini C compiler front-end** using **Lex (Flex)** and **Yacc (Bison)**. The compiler parses a subset of the C programming language and generates **Three Address Code (TAC)** as intermediate representation.

The project demonstrates key compiler construction concepts including:

* Lexical analysis
* Syntax parsing
* Semantic analysis
* Symbol table management
* Type checking and casting
* Intermediate code generation (TAC)
* Control flow handling with backpatching

It is designed for **academic rigor** while maintaining **industry-style structure, CI integration, and reproducibility**.

---

## 🎯 Features

### 🔹 Language Support

* Primitive types: `int`, `long`, `float`, `double`
* Arrays (multi-dimensional supported)
* Structures (`struct`)
* Arithmetic expressions
* Relational and logical expressions
* Control flow:

  * `if`, `if-else`
  * `while`
* Assignment statements

---

### 🔹 Compiler Capabilities

* ✔ Lexical analysis via Flex
* ✔ Syntax parsing via Bison
* ✔ Type checking and implicit casting
* ✔ Symbol table with scope handling
* ✔ Type table for user-defined types
* ✔ Memory layout tracking
* ✔ Backpatching for control flow
* ✔ Three-address code generation

---

## 🏗️ Project Structure

```text
tiny-c-compiler/
├── src/
│   ├── main.c                # Driver + semantic routines + TAC generation
│   ├── basiccodegen.l        # Lexer (Flex)
│   ├── basiccodegen.y        # Parser (Bison)
│
├── tests/
│   ├── decl.txt              # Sample input program
│   ├── expected.txt          # Expected TAC output
│
├── build/                    # Generated artifacts (ignored)
│
├── .github/workflows/
│   └── compiler.yml          # CI pipeline
│
├── Makefile
├── .gitignore
└── README.md
```

---

## ⚙️ Build Instructions

### 🔹 Prerequisites

* Flex
* Bison
* GCC

Install (Ubuntu):

```bash
sudo apt-get update
sudo apt-get install -y flex bison gcc
```

---

### 🔹 Build

```bash
flex -o src/lex.yy.c src/basiccodegen.l
bison -d -o src/y.tab.c src/basiccodegen.y
gcc -o compiler src/main.c src/lex.yy.c src/y.tab.c -Isrc
```

---

### 🔹 Run

```bash
./compiler tests/decl.txt
```

---

## 🧪 Sample Input

The compiler processes programs like:

```c
long a, b;
float c;

a = 10;
b = a;
c = a + b;

if (c > a) {
    a = a + 5;
}
```

(Full example available in `tests/decl.txt`)

---

## 📤 Output

The compiler generates **Three Address Code (TAC)**:

```
+++ Three Address Code
        0  : [int] t1 = 10
        1  : [int] t2 = t1 + t1
        ...
```

---

## 🧠 Core Components

### 🔹 1. Lexer (Flex)

* Tokenizes input source code
* Identifies keywords, identifiers, literals

---

### 🔹 2. Parser (Bison)

* Defines grammar rules
* Builds semantic actions
* Triggers TAC generation

---

### 🔹 3. Symbol Table

Tracks:

* Variable names
* Types
* Memory locations
* Scope information

---

### 🔹 4. Type Table

Stores:

* Primitive types
* Array types
* Struct types

---

### 🔹 5. Intermediate Code Generator

Implements:

* Temporary variable creation
* Type casting
* Arithmetic operations
* Memory access abstraction

Example from code: 

* `createOperation()` → generates TAC for expressions
* `backpatch()` → resolves control flow jumps
* `makeSTEntry()` → symbol table insertion

---

## 🔄 Control Flow Handling

Uses **backpatching** technique:

* Maintains lists of unresolved jumps
* Fixes them once target labels are known

---

## 🧮 Type System

Supports:

* Type promotion (`int → float`, etc.)
* Explicit casting
* Type compatibility checks

---

## 🤖 Continuous Integration (CI)

### 🔹 GitHub Actions Workflow

Located at:

```text
.github/workflows/compiler.yml
```

---

### 🔹 CI Pipeline Steps

1. Install dependencies (Flex, Bison, GCC)
2. Generate lexer and parser
3. Compile the compiler
4. Run test program
5. Compare output with expected result

---

### 🔹 Pass/Fail Mechanism

```bash
./compiler tests/decl.txt > output.txt
diff output.txt tests/expected.txt
```

* ✅ Match → PASS
* ❌ Mismatch → FAIL

---

## 🧪 Testing Strategy

* Deterministic output comparison
* Multiple test inputs supported
* Automated validation via CI

---

## 🧠 Design Philosophy

* **Modularity** → Lexer, parser, and semantics separated
* **Determinism** → Same input → same output
* **Reproducibility** → CI ensures consistent builds
* **Extensibility** → Easy to add new language features

---

## ⚠️ Known Limitations

* No Abstract Syntax Tree (AST) representation
* No optimization phase
* Limited error recovery
* Global state-heavy design
* Memory leaks (malloc without free)

---

## 🚀 Future Improvements

* Add AST layer
* Implement optimization passes
* Improve error diagnostics
* Add function support
* Introduce register allocation
* Convert TAC → assembly

---

## 📜 License

For academic and educational use.

---

## ⭐ Final Note

This project represents a **complete compiler front-end pipeline**, demonstrating:

* Theory → Implementation
* Parsing → Code Generation
* Manual testing → CI-driven validation

It aligns with **industry practices in compiler engineering and build automation**, making it a strong foundation for advanced compiler design or systems programming work.

---
