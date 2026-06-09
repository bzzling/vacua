#pragma once

#include <string>

#include "vacua/circuit/circuit.hpp"

namespace vacua {

Circuit bell();
Circuit ghz(std::size_t n);
Circuit qft(std::size_t n);
Circuit grover(std::size_t n, const std::string& target_bitstring);
Circuit qaoa_ansatz(std::size_t num_qubits, std::size_t layers);

}  // namespace vacua
