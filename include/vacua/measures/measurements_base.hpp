#pragma once

#include <cstdint>
#include <vector>

#include "vacua/device/result.hpp"

namespace vacua {
class Hamiltonian;
}

namespace vacua::measures {

template <class StateVectorT, class Derived>
class MeasurementsBase {
 public:
  explicit MeasurementsBase(const StateVectorT& sv) : sv_{sv} {}

  std::vector<double> probabilities() const {
    return static_cast<const Derived*>(this)->probabilities_impl();
  }

  double expval(const vacua::Hamiltonian& h) const {
    return static_cast<const Derived*>(this)->expval_impl(h);
  }

  Result sample(Shots shots, std::uint64_t seed) const {
    return static_cast<const Derived*>(this)->sample_impl(shots, seed);
  }

 protected:
  const StateVectorT& sv_;
};

}  // namespace vacua::measures
