#include "vacua/cpu/statevector.hpp"

#include <complex>
#include <stdexcept>

#include "vacua/gates/gates.hpp"

using namespace vacua::gates;

namespace vacua::cpu {

StateVector::StateVector(std::size_t num_qubits)
    : vacua::StateVectorBase<StateVector>{num_qubits},
      amplitudes_(std::size_t{1} << num_qubits, std::complex<double>{0.0}) {
  amplitudes_[0] = std::complex<double>{1.0};
}

void StateVector::reset_impl() {
  std::fill(amplitudes_.begin(), amplitudes_.end(), std::complex<double>{0.0});
  amplitudes_[0] = std::complex<double>{1.0};
}

void StateVector::apply_single_qubit_gate(const Matrix2& u, std::size_t target) {
  const std::size_t n{size()};
  const std::size_t bit{std::size_t{1} << target};
  for (std::size_t i{0}; i < n; ++i) {
    if ((i & bit) == 0) {
      const std::size_t j{i | bit};
      auto a0{amplitudes_[i]};
      auto a1{amplitudes_[j]};
      amplitudes_[i] = u.a00 * a0 + u.a01 * a1;
      amplitudes_[j] = u.a10 * a0 + u.a11 * a1;
    }
  }
}

void StateVector::apply_cx(std::size_t control, std::size_t target) {
  const std::size_t n{size()};
  const std::size_t cbit{std::size_t{1} << control};
  const std::size_t tbit{std::size_t{1} << target};
  for (std::size_t i{0}; i < n; ++i) {
    if ((i & cbit) != 0 && (i & tbit) == 0) {
      const std::size_t j{i | tbit};
      std::swap(amplitudes_[i], amplitudes_[j]);
    }
  }
}

void StateVector::apply_cz(std::size_t control, std::size_t target) {
  const std::size_t n{size()};
  const std::size_t cbit{std::size_t{1} << control};
  const std::size_t tbit{std::size_t{1} << target};
  for (std::size_t i{0}; i < n; ++i) {
    if ((i & cbit) != 0 && (i & tbit) != 0) amplitudes_[i] = -amplitudes_[i];
  }
}

void StateVector::apply_swap(std::size_t q0, std::size_t q1) {
  const std::size_t n{size()};
  const std::size_t b0{std::size_t{1} << q0};
  const std::size_t b1{std::size_t{1} << q1};
  for (std::size_t i{0}; i < n; ++i) {
    if ((i & b0) == 0 && (i & b1) != 0) {
      const std::size_t j{(i | b0) & ~b1};
      std::swap(amplitudes_[i], amplitudes_[j]);
    }
  }
}

void StateVector::apply_ccx(std::size_t c0, std::size_t c1, std::size_t target) {
  const std::size_t n{size()};
  const std::size_t b0{std::size_t{1} << c0};
  const std::size_t b1{std::size_t{1} << c1};
  const std::size_t tbit{std::size_t{1} << target};
  for (std::size_t i{0}; i < n; ++i) {
    if ((i & b0) != 0 && (i & b1) != 0 && (i & tbit) == 0) {
      const std::size_t j{i | tbit};
      std::swap(amplitudes_[i], amplitudes_[j]);
    }
  }
}

void StateVector::apply_impl(const Instruction& instr, const ParameterMap& params) {
  switch (instr.op) {
    case OpType::X:
      apply_single_qubit_gate(getX(), instr.qubits[0]);
      break;
    case OpType::Y:
      apply_single_qubit_gate(getY(), instr.qubits[0]);
      break;
    case OpType::Z:
      apply_single_qubit_gate(getZ(), instr.qubits[0]);
      break;
    case OpType::H:
      apply_single_qubit_gate(getH(), instr.qubits[0]);
      break;
    case OpType::S:
      apply_single_qubit_gate(getS(), instr.qubits[0]);
      break;
    case OpType::Sdg:
      apply_single_qubit_gate(getSdg(), instr.qubits[0]);
      break;
    case OpType::T:
      apply_single_qubit_gate(getT(), instr.qubits[0]);
      break;
    case OpType::Tdg:
      apply_single_qubit_gate(getTdg(), instr.qubits[0]);
      break;

    case OpType::RX:
      apply_single_qubit_gate(getRX(instr.param->evaluate(params)), instr.qubits[0]);
      break;
    case OpType::RY:
      apply_single_qubit_gate(getRY(instr.param->evaluate(params)), instr.qubits[0]);
      break;
    case OpType::RZ:
      apply_single_qubit_gate(getRZ(instr.param->evaluate(params)), instr.qubits[0]);
      break;

    case OpType::CX:
      apply_cx(instr.qubits[0], instr.qubits[1]);
      break;
    case OpType::CZ:
      apply_cz(instr.qubits[0], instr.qubits[1]);
      break;
    case OpType::SWAP:
      apply_swap(instr.qubits[0], instr.qubits[1]);
      break;
    case OpType::CCX:
      apply_ccx(instr.qubits[0], instr.qubits[1], instr.qubits[2]);
      break;

    case OpType::Measure:
    case OpType::MeasureAll:
    case OpType::Barrier:
      throw std::runtime_error{
          "use Device::run() for measurement — do not apply "
          "measurement instructions directly to StateVector"};
  }
}

std::vector<double> StateVector::probabilities_impl() const {
  std::vector<double> probs(amplitudes_.size());
  for (std::size_t i{0}; i < amplitudes_.size(); ++i) probs[i] = std::norm(amplitudes_[i]);
  return probs;
}

}  // namespace vacua::cpu
