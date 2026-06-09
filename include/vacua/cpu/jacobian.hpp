#pragma once

#include "vacua/algorithms/jacobian_base.hpp"
#include "vacua/cpu/statevector.hpp"

namespace vacua::cpu {

class Jacobian : public vacua::algorithms::JacobianBase<StateVector, Jacobian> {
 public:
  Gradient compute_impl(const Circuit& circuit, const vacua::Hamiltonian& h,
                        const ParameterMap& params) const;
};

}  // namespace vacua::cpu
