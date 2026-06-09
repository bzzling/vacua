#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <numbers>

#include "vacua/vacua.hpp"

using namespace vacua;
using namespace Catch::Matchers;

static constexpr double TOL = 1e-10;

// Helper: apply circuit to a fresh StateVector
static cpu::StateVector run(std::size_t num_qubits, const Circuit& c,
                            const ParameterMap& params = {}) {
  cpu::StateVector sv{num_qubits};
  for (const auto& instr : c.instructions()) sv.apply(instr, params);
  return sv;
}

// ---- single-qubit gate tests ----

TEST_CASE("X|0> = |1>", "[statevector]") {
  Circuit c(1);
  c.x(0);
  auto sv = run(1, c);
  CHECK_THAT(sv.amplitudes()[0].real(), WithinAbs(0.0, TOL));
  CHECK_THAT(sv.amplitudes()[1].real(), WithinAbs(1.0, TOL));
}

TEST_CASE("H|0> = (|0>+|1>)/sqrt(2)", "[statevector]") {
  Circuit c(1);
  c.h(0);
  auto sv = run(1, c);
  const double inv_sqrt2 = 1.0 / std::numbers::sqrt2;
  CHECK_THAT(sv.amplitudes()[0].real(), WithinAbs(inv_sqrt2, TOL));
  CHECK_THAT(sv.amplitudes()[1].real(), WithinAbs(inv_sqrt2, TOL));
}

TEST_CASE("H H |0> = |0>", "[statevector]") {
  Circuit c(1);
  c.h(0).h(0);
  auto sv = run(1, c);
  CHECK_THAT(sv.amplitudes()[0].real(), WithinAbs(1.0, TOL));
  CHECK_THAT(sv.amplitudes()[0].imag(), WithinAbs(0.0, TOL));
  CHECK_THAT(sv.amplitudes()[1].real(), WithinAbs(0.0, TOL));
}

TEST_CASE("Y|0> = i|1>", "[statevector]") {
  Circuit c(1);
  c.y(0);
  auto sv = run(1, c);
  CHECK_THAT(sv.amplitudes()[1].real(), WithinAbs(0.0, TOL));
  CHECK_THAT(sv.amplitudes()[1].imag(), WithinAbs(1.0, TOL));
}

TEST_CASE("Z|0> = |0> unchanged", "[statevector]") {
  Circuit c(1);
  c.z(0);
  auto sv = run(1, c);
  CHECK_THAT(sv.amplitudes()[0].real(), WithinAbs(1.0, TOL));
  CHECK_THAT(sv.amplitudes()[1].real(), WithinAbs(0.0, TOL));
}

TEST_CASE("Z|1> = -|1>", "[statevector]") {
  Circuit c(1);
  c.x(0).z(0);
  auto sv = run(1, c);
  CHECK_THAT(sv.amplitudes()[1].real(), WithinAbs(-1.0, TOL));
  CHECK_THAT(sv.amplitudes()[1].imag(), WithinAbs(0.0, TOL));
}

TEST_CASE("S|0>=|0>, S|1>=i|1>", "[statevector]") {
  {
    Circuit c(1);
    c.s(0);
    auto sv = run(1, c);
    CHECK_THAT(sv.amplitudes()[0].real(), WithinAbs(1.0, TOL));
  }
  {
    Circuit c(1);
    c.x(0).s(0);
    auto sv = run(1, c);
    CHECK_THAT(sv.amplitudes()[1].real(), WithinAbs(0.0, TOL));
    CHECK_THAT(sv.amplitudes()[1].imag(), WithinAbs(1.0, TOL));
  }
}

TEST_CASE("T|0>=|0>, T|1>=e^(i*pi/4)|1>", "[statevector]") {
  {
    Circuit c(1);
    c.t(0);
    auto sv = run(1, c);
    CHECK_THAT(sv.amplitudes()[0].real(), WithinAbs(1.0, TOL));
  }
  {
    Circuit c(1);
    c.x(0).t(0);
    auto sv = run(1, c);
    const double inv_sqrt2 = 1.0 / std::numbers::sqrt2;
    CHECK_THAT(sv.amplitudes()[1].real(), WithinAbs(inv_sqrt2, TOL));
    CHECK_THAT(sv.amplitudes()[1].imag(), WithinAbs(inv_sqrt2, TOL));
  }
}

// ---- two-qubit gate tests ----

TEST_CASE("CX on |10> gives |11>", "[statevector]") {
  Circuit c(2);
  c.x(1).cx(1, 0);
  auto sv = run(2, c);
  CHECK_THAT(sv.amplitudes()[3].real(), WithinAbs(1.0, TOL));
  CHECK_THAT(sv.amplitudes()[2].real(), WithinAbs(0.0, TOL));
}

TEST_CASE("CX on |00> unchanged", "[statevector]") {
  Circuit c(2);
  c.cx(0, 1);
  auto sv = run(2, c);
  CHECK_THAT(sv.amplitudes()[0].real(), WithinAbs(1.0, TOL));
  for (std::size_t i = 1; i < 4; ++i) CHECK_THAT(sv.amplitudes()[i].real(), WithinAbs(0.0, TOL));
}

TEST_CASE("Bell state from H+CX", "[statevector]") {
  Circuit c(2);
  c.h(0).cx(0, 1);
  auto sv = run(2, c);
  const double inv_sqrt2 = 1.0 / std::numbers::sqrt2;
  CHECK_THAT(sv.amplitudes()[0].real(), WithinAbs(inv_sqrt2, TOL));
  CHECK_THAT(sv.amplitudes()[0].imag(), WithinAbs(0.0, TOL));
  CHECK_THAT(sv.amplitudes()[3].real(), WithinAbs(inv_sqrt2, TOL));
  CHECK_THAT(sv.amplitudes()[3].imag(), WithinAbs(0.0, TOL));
  CHECK_THAT(sv.amplitudes()[1].real(), WithinAbs(0.0, TOL));
  CHECK_THAT(sv.amplitudes()[2].real(), WithinAbs(0.0, TOL));
}

TEST_CASE("CZ applies phase to |11> only", "[statevector]") {
  Circuit c(2);
  c.x(0).x(1).cz(0, 1);
  auto sv = run(2, c);
  CHECK_THAT(sv.amplitudes()[3].real(), WithinAbs(-1.0, TOL));
  CHECK_THAT(sv.amplitudes()[3].imag(), WithinAbs(0.0, TOL));
}

TEST_CASE("SWAP exchanges qubit states", "[statevector]") {
  Circuit c(2);
  c.x(1).swap(0, 1);
  auto sv = run(2, c);
  CHECK_THAT(sv.amplitudes()[1].real(), WithinAbs(1.0, TOL));
  CHECK_THAT(sv.amplitudes()[2].real(), WithinAbs(0.0, TOL));
}

// ---- rotation gate tests ----

TEST_CASE("RY(pi)|0> = |1>", "[statevector]") {
  Circuit c(1);
  c.ry(0, std::numbers::pi);
  auto sv = run(1, c);
  CHECK_THAT(sv.amplitudes()[0].real(), WithinAbs(0.0, TOL));
  CHECK_THAT(sv.amplitudes()[1].real(), WithinAbs(1.0, TOL));
}

// ---- probability tests ----

TEST_CASE("probabilities sum to 1.0", "[statevector]") {
  Circuit c(3);
  c.h(0).cx(0, 1).h(2);
  auto sv = run(3, c);
  auto probs = sv.probabilities();
  double sum{0.0};
  for (double p : probs) sum += p;
  CHECK_THAT(sum, WithinAbs(1.0, TOL));
}

TEST_CASE("Bell state probabilities: prob[0]=0.5, prob[3]=0.5", "[statevector]") {
  Circuit c(2);
  c.h(0).cx(0, 1);
  auto sv = run(2, c);
  auto probs = sv.probabilities();
  CHECK_THAT(probs[0], WithinAbs(0.5, TOL));
  CHECK_THAT(probs[3], WithinAbs(0.5, TOL));
  CHECK_THAT(probs[1], WithinAbs(0.0, TOL));
  CHECK_THAT(probs[2], WithinAbs(0.0, TOL));
}
