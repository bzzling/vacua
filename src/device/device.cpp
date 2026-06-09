#include "vacua/device/device.hpp"

#include <stdexcept>

#include "vacua/cpu/jacobian.hpp"

namespace vacua {

Device::Device([[maybe_unused]] DeviceType type, std::uint64_t seed) : seed_{seed} {}

cpu::StateVector Device::execute(const Circuit& circuit, const ParameterMap& params) {
  cpu::StateVector sv{circuit.num_qubits()};
  const auto& instrs{circuit.instructions()};

  for (std::size_t i{0}; i < instrs.size(); ++i) {
    const auto& instr{instrs[i]};
    const bool is_measure{instr.op == OpType::Measure || instr.op == OpType::MeasureAll};
    const bool is_last{i == instrs.size() - 1};

    if (is_measure && !is_last)
      throw std::runtime_error{
          "mid-circuit measurement not supported: "
          "Measure/MeasureAll must be the final instruction"};

    if (instr.op == OpType::Barrier || is_measure) continue;

    sv.apply(instr, params);
  }

  return sv;
}

Result Device::run(const Circuit& circuit, Shots shots, const ParameterMap& params) {
  auto sv{execute(circuit, params)};
  cpu::Measurements m{sv};
  return m.sample(shots, seed_);
}

cpu::StateVector Device::statevector(const Circuit& circuit, const ParameterMap& params) {
  return execute(circuit, params);
}

double Device::expectation(const Circuit& circuit, const Hamiltonian& h,
                           const ParameterMap& params) {
  auto sv{execute(circuit, params)};
  cpu::Measurements m{sv};
  return m.expval(h);
}

Gradient Device::gradient(const Circuit& circuit, const Hamiltonian& h,
                          const ParameterMap& params) {
  cpu::Jacobian jac{};
  return jac.compute(circuit, h, params);
}

}  // namespace vacua
