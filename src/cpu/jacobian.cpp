#include "vacua/cpu/jacobian.hpp"

#include <stdexcept>

namespace vacua::cpu {

Gradient Jacobian::compute_impl([[maybe_unused]] const Circuit& circuit,
                                [[maybe_unused]] const vacua::Hamiltonian& h,
                                [[maybe_unused]] const ParameterMap& params) const {
  throw std::runtime_error{"Jacobian not yet implemented — coming in Milestone 9"};
}

}  // namespace vacua::cpu
