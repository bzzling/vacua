#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "vacua/circuit/parameter.hpp"

namespace vacua {

enum class OpType {
  X,
  Y,
  Z,
  H,
  S,
  Sdg,
  T,
  Tdg,
  RX,
  RY,
  RZ,
  CX,
  CZ,
  SWAP,
  CCX,
  Measure,
  MeasureAll,
  Barrier
};

struct Instruction {
  OpType op;
  std::vector<std::size_t> qubits;
  std::vector<std::size_t> bits;
  std::optional<ParameterExpr> param;
};

}  // namespace vacua
