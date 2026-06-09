#pragma once

#include <cstddef>
#include <vector>

#include "vacua/circuit/instruction.hpp"
#include "vacua/circuit/parameter.hpp"

namespace vacua {

template <class Derived>
class StateVectorBase {
 public:
  explicit StateVectorBase(std::size_t num_qubits) : num_qubits_{num_qubits} {}

  std::size_t num_qubits() const { return num_qubits_; }
  std::size_t size() const { return std::size_t{1} << num_qubits_; }

  void apply(const Instruction& instr, const ParameterMap& params) {
    static_cast<Derived*>(this)->apply_impl(instr, params);
  }

  std::vector<double> probabilities() const {
    return static_cast<const Derived*>(this)->probabilities_impl();
  }

  void reset() { static_cast<Derived*>(this)->reset_impl(); }

 private:
  std::size_t num_qubits_;
};

}  // namespace vacua
