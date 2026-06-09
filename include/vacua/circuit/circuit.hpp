#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

#include "vacua/circuit/instruction.hpp"
#include "vacua/circuit/parameter.hpp"

namespace vacua {

class Circuit {
 public:
  explicit Circuit(std::size_t num_qubits) : num_qubits_{num_qubits} {}

  std::size_t num_qubits() const { return num_qubits_; }
  const std::vector<Instruction>& instructions() const { return instructions_; }

  // Single-qubit gates
  Circuit& x(std::size_t q) { return add1(OpType::X, q); }
  Circuit& y(std::size_t q) { return add1(OpType::Y, q); }
  Circuit& z(std::size_t q) { return add1(OpType::Z, q); }
  Circuit& h(std::size_t q) { return add1(OpType::H, q); }
  Circuit& s(std::size_t q) { return add1(OpType::S, q); }
  Circuit& sdg(std::size_t q) { return add1(OpType::Sdg, q); }
  Circuit& t(std::size_t q) { return add1(OpType::T, q); }
  Circuit& tdg(std::size_t q) { return add1(OpType::Tdg, q); }

  // Rotation gates — double overload
  Circuit& rx(std::size_t q, double angle) {
    return add_rot(OpType::RX, q, ParameterExpr::from_constant(angle));
  }
  Circuit& ry(std::size_t q, double angle) {
    return add_rot(OpType::RY, q, ParameterExpr::from_constant(angle));
  }
  Circuit& rz(std::size_t q, double angle) {
    return add_rot(OpType::RZ, q, ParameterExpr::from_constant(angle));
  }

  // Rotation gates — ParameterExpr overload
  Circuit& rx(std::size_t q, ParameterExpr p) { return add_rot(OpType::RX, q, std::move(p)); }
  Circuit& ry(std::size_t q, ParameterExpr p) { return add_rot(OpType::RY, q, std::move(p)); }
  Circuit& rz(std::size_t q, ParameterExpr p) { return add_rot(OpType::RZ, q, std::move(p)); }

  // Two-qubit gates
  Circuit& cx(std::size_t ctrl, std::size_t tgt) { return add2(OpType::CX, ctrl, tgt); }
  Circuit& cz(std::size_t ctrl, std::size_t tgt) { return add2(OpType::CZ, ctrl, tgt); }
  Circuit& swap(std::size_t q0, std::size_t q1) { return add2(OpType::SWAP, q0, q1); }

  // Three-qubit gate
  Circuit& ccx(std::size_t c0, std::size_t c1, std::size_t tgt) {
    check(c0);
    check(c1);
    check(tgt);
    instructions_.push_back({OpType::CCX, {c0, c1, tgt}, {}, std::nullopt});
    return *this;
  }

  // Measure / barrier
  Circuit& measure(std::size_t qubit, std::size_t bit) {
    check(qubit);
    instructions_.push_back({OpType::Measure, {qubit}, {bit}, std::nullopt});
    return *this;
  }

  Circuit& measure_all() {
    instructions_.push_back({OpType::MeasureAll, {}, {}, std::nullopt});
    return *this;
  }

  Circuit& barrier(std::size_t q) {
    check(q);
    instructions_.push_back({OpType::Barrier, {q}, {}, std::nullopt});
    return *this;
  }

 private:
  std::size_t num_qubits_;
  std::vector<Instruction> instructions_;

  void check(std::size_t q) const {
    if (q >= num_qubits_)
      throw std::out_of_range("qubit index " + std::to_string(q) + " out of range for " +
                              std::to_string(num_qubits_) + "-qubit circuit");
  }

  Circuit& add1(OpType op, std::size_t q) {
    check(q);
    instructions_.push_back({op, {q}, {}, std::nullopt});
    return *this;
  }

  Circuit& add2(OpType op, std::size_t q0, std::size_t q1) {
    check(q0);
    check(q1);
    instructions_.push_back({op, {q0, q1}, {}, std::nullopt});
    return *this;
  }

  Circuit& add_rot(OpType op, std::size_t q, ParameterExpr p) {
    check(q);
    instructions_.push_back({op, {q}, {}, std::move(p)});
    return *this;
  }
};

}  // namespace vacua
