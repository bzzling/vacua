#pragma once

#include <complex>
#include <vector>

#include "vacua/base/matrix.hpp"
#include "vacua/base/statevector_base.hpp"

namespace vacua::cpu {

class StateVector : public vacua::StateVectorBase<StateVector> {
 public:
  explicit StateVector(std::size_t num_qubits);

  // CRTP interface
  void apply_impl(const Instruction& instr, const ParameterMap& params);
  std::vector<double> probabilities_impl() const;
  void reset_impl();

  // CPU-specific — direct amplitude access for power users and tests
  const std::vector<std::complex<double>>& amplitudes() const { return amplitudes_; }
  std::vector<std::complex<double>>& amplitudes() { return amplitudes_; }

 private:
  std::vector<std::complex<double>> amplitudes_;

  void apply_single_qubit_gate(const Matrix2& u, std::size_t target);
  void apply_cx(std::size_t control, std::size_t target);
  void apply_cz(std::size_t control, std::size_t target);
  void apply_swap(std::size_t q0, std::size_t q1);
  void apply_ccx(std::size_t c0, std::size_t c1, std::size_t target);
};

}  // namespace vacua::cpu
