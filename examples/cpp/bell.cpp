#include <vacua/vacua.hpp>

int main() {
  vacua::Circuit circuit{2};
  circuit.h(0);
  circuit.cx(0, 1);
  circuit.measure_all();

  vacua::Device sim{vacua::DeviceType::CPU, 42};
  auto result = sim.run(circuit, vacua::Shots{1000});
  result.print_counts();
}
