#pragma once

#include <cstdint>

#include "vacua/device/result.hpp"
#include "vacua/circuit/circuit.hpp"
#include "vacua/circuit/parameter.hpp"
#include "vacua/cpu/measurements.hpp"
#include "vacua/cpu/statevector.hpp"

namespace vacua {
class Hamiltonian;
}

namespace vacua {

enum class DeviceType { CPU };

class Device {
 public:
  explicit Device(DeviceType type = DeviceType::CPU, std::uint64_t seed = 42);

  Result run(const Circuit& circuit, Shots shots, const ParameterMap& params = {});

  cpu::StateVector statevector(const Circuit& circuit, const ParameterMap& params = {});

  double expectation(const Circuit& circuit, const Hamiltonian& h, const ParameterMap& params = {});

  Gradient gradient(const Circuit& circuit, const Hamiltonian& h, const ParameterMap& params = {});

 private:
  std::uint64_t seed_;

  static cpu::StateVector execute(const Circuit& circuit, const ParameterMap& params);
};

}  // namespace vacua
