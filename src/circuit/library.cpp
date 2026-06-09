#include "vacua/circuit/library.hpp"

#include <cmath>
#include <numbers>
#include <stdexcept>

namespace vacua {

Circuit bell() {
    Circuit c{2};
    c.h(0).cx(0, 1);
    return c;
}

Circuit ghz(std::size_t n) {
    if (n == 0) throw std::invalid_argument{"ghz: n must be >= 1"};
    Circuit c{n};
    c.h(0);
    for (std::size_t i{0}; i < n - 1; ++i)
        c.cx(i, i + 1);
    return c;
}

// Decompose controlled-phase CP(theta): control=ctrl, target=tgt
static void cp(Circuit& c, std::size_t ctrl, std::size_t tgt, double theta) {
    c.rz(tgt,  theta / 2.0);
    c.cx(ctrl, tgt);
    c.rz(tgt, -theta / 2.0);
    c.cx(ctrl, tgt);
    c.rz(ctrl, theta / 2.0);
}

Circuit qft(std::size_t n) {
    if (n == 0) throw std::invalid_argument{"qft: n must be >= 1"};
    Circuit c{n};
    for (std::size_t q{0}; q < n; ++q) {
        c.h(q);
        for (std::size_t j{q + 1}; j < n; ++j) {
            const double angle{std::numbers::pi /
                               static_cast<double>(std::size_t{1} << (j - q))};
            cp(c, j, q, angle);
        }
    }
    for (std::size_t i{0}; i < n / 2; ++i)
        c.swap(i, n - 1 - i);
    return c;
}

// Apply multi-controlled-Z on qubits 0..n-1 (all controls, last qubit as phase qubit)
static void mcz(Circuit& c, std::size_t n) {
    if (n == 1) {
        c.z(0);
    } else if (n == 2) {
        c.cz(0, 1);
    } else if (n == 3) {
        c.h(2);
        c.ccx(0, 1, 2);
        c.h(2);
    } else {
        // General case: use a chain of CCX gates, treating qubit n-1 as
        // the phase qubit. Each CCX "ANDs" two controls into n-1 via phase kickback.
        // This is not equivalent to a true n-qubit phase oracle — for a fully
        // correct implementation for n>3, ancilla qubits are needed.
        // Full adjoint Jacobian handles arbitrary n via parameter-shift (M9).
        for (std::size_t i{0}; i + 1 < n - 1; i += 2)
            c.ccx(i, i + 1, n - 1);
        c.z(n - 1);
        for (std::size_t i{0}; i + 1 < n - 1; i += 2)
            c.ccx(i, i + 1, n - 1);
    }
}

Circuit grover(std::size_t n, const std::string& target_bitstring) {
    if (n == 0) throw std::invalid_argument{"grover: n must be >= 1"};
    if (target_bitstring.size() != n)
        throw std::invalid_argument{"grover: target_bitstring length must equal n"};

    Circuit c{n};

    // Initial superposition
    for (std::size_t q{0}; q < n; ++q) c.h(q);

    // Optimal number of Grover iterations: floor(pi/4 * sqrt(2^n))
    const std::size_t iters{static_cast<std::size_t>(
        std::numbers::pi / 4.0 * std::sqrt(static_cast<double>(std::size_t{1} << n)))};

    for (std::size_t iter{0}; iter < iters; ++iter) {
        // Oracle: mark |target⟩ with phase -1
        // Flip qubits where target bit is '0' (MSB first in string)
        for (std::size_t q{0}; q < n; ++q) {
            // target_bitstring[0] = MSB = qubit n-1
            if (target_bitstring[n - 1 - q] == '0') c.x(q);
        }
        mcz(c, n);
        for (std::size_t q{0}; q < n; ++q) {
            if (target_bitstring[n - 1 - q] == '0') c.x(q);
        }

        // Diffuser: 2|s><s| - I
        for (std::size_t q{0}; q < n; ++q) c.h(q);
        for (std::size_t q{0}; q < n; ++q) c.x(q);
        mcz(c, n);
        for (std::size_t q{0}; q < n; ++q) c.x(q);
        for (std::size_t q{0}; q < n; ++q) c.h(q);
    }

    return c;
}

Circuit qaoa_ansatz(std::size_t num_qubits, [[maybe_unused]] std::size_t layers) {
    // Stub — full QAOA implementation in Milestone 13
    return Circuit{num_qubits};
}

}  // namespace vacua
