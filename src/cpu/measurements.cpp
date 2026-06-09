#include "vacua/cpu/measurements.hpp"

#include <random>

namespace vacua::cpu {

Measurements::Measurements(const StateVector& sv)
    : vacua::measures::MeasurementsBase<StateVector, Measurements>{sv} {}

std::vector<double> Measurements::probabilities_impl() const {
  return sv_.probabilities();
}

double Measurements::expval_impl([[maybe_unused]] const vacua::Hamiltonian& h) const {
  return 0.0;  // stub — full implementation in Milestone 8
}

std::string Measurements::to_bitstring(std::size_t index, std::size_t num_qubits) const {
  std::string s(num_qubits, '0');
  for (std::size_t q{0}; q < num_qubits; ++q) {
    if ((index >> (num_qubits - 1 - q)) & 1) s[q] = '1';
  }
  return s;
}

Result Measurements::sample_impl(Shots shots, std::uint64_t seed) const {
  auto probs{sv_.probabilities()};

  std::mt19937_64 rng{seed};
  std::discrete_distribution<std::size_t> dist{probs.begin(), probs.end()};

  Result result{};
  result.probabilities_ = probs;

  for (std::size_t s{0}; s < shots.value; ++s) {
    const std::size_t idx{dist(rng)};
    result.counts_[to_bitstring(idx, sv_.num_qubits())]++;
  }

  return result;
}

}  // namespace vacua::cpu
