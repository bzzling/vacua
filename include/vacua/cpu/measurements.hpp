#pragma once

#include <string>
#include <vector>

#include "vacua/measures/measurements_base.hpp"
#include "vacua/cpu/statevector.hpp"

namespace vacua::cpu {

class Measurements : public vacua::measures::MeasurementsBase<StateVector, Measurements> {
 public:
  explicit Measurements(const StateVector& sv);

  // CRTP interface
  std::vector<double> probabilities_impl() const;
  double expval_impl(const vacua::Hamiltonian& h) const;
  Result sample_impl(Shots shots, std::uint64_t seed) const;

 private:
  std::string to_bitstring(std::size_t index, std::size_t num_qubits) const;
};

}  // namespace vacua::cpu
