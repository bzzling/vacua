#pragma once

#include "vacua/device/result.hpp"
#include "vacua/circuit/circuit.hpp"
#include "vacua/circuit/parameter.hpp"

namespace vacua {
class Hamiltonian;
}

namespace vacua::algorithms {

template <class StateVectorT, class Derived>
class JacobianBase {
 public:
  Gradient compute(const Circuit& circuit, const vacua::Hamiltonian& h,
                   const ParameterMap& params) const {
    return static_cast<const Derived*>(this)->compute_impl(circuit, h, params);
  }
};

}  // namespace vacua::algorithms
