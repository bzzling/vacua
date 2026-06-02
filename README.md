# Vacua

> A from-scratch C++20 differentiable quantum simulation framework for variational quantum algorithms and ground state estimation.

## What it is

Vacua is a from-scratch quantum simulation framework with zero external 
dependencies. It provides a full state-vector simulator, parameterized 
circuits, and the building blocks for variational quantum algorithms like 
VQE and QAOA.

## Features

- Qiskit-inspired C++ circuit API
- State-vector simulation from scratch
- Full single, two, and three qubit gate set
- Parameterized circuits with symbolic parameters
- Measurement sampling with seeded randomness
- Zero external dependencies in the core library
- Circuit optimizer passes (gate cancellation, rotation merging)
- Pauli strings and Hamiltonians
- Exact expectation values
- Parameter-shift gradients
- SGD and Adam optimizers
- VQE and QAOA examples
- Optional CUDA backend

## Build

Requirements: CMake 3.25+, GCC 11+ or Clang 13+

```bash
git clone https://github.com/bzzling/vacua
cd vacua
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build
```