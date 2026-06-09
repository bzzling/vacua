#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <numbers>

#include "vacua/vacua.hpp"

using namespace vacua;
using namespace Catch::Matchers;

static constexpr double TOL = 1e-10;

// Returns U * U†
static Matrix2 mul_dagger(const Matrix2& u) {
  auto conj = [](std::complex<double> z) { return std::conj(z); };
  // U† col0 = conj(row0 of U), U† col1 = conj(row1 of U)
  std::complex<double> r00 = u.a00 * conj(u.a00) + u.a01 * conj(u.a01);
  std::complex<double> r01 = u.a00 * conj(u.a10) + u.a01 * conj(u.a11);
  std::complex<double> r10 = u.a10 * conj(u.a00) + u.a11 * conj(u.a01);
  std::complex<double> r11 = u.a10 * conj(u.a10) + u.a11 * conj(u.a11);
  return {r00, r01, r10, r11};
}

static void check_unitary(const Matrix2& m) {
  auto id = mul_dagger(m);
  CHECK_THAT(id.a00.real(), WithinAbs(1.0, TOL));
  CHECK_THAT(id.a00.imag(), WithinAbs(0.0, TOL));
  CHECK_THAT(id.a01.real(), WithinAbs(0.0, TOL));
  CHECK_THAT(id.a01.imag(), WithinAbs(0.0, TOL));
  CHECK_THAT(id.a10.real(), WithinAbs(0.0, TOL));
  CHECK_THAT(id.a10.imag(), WithinAbs(0.0, TOL));
  CHECK_THAT(id.a11.real(), WithinAbs(1.0, TOL));
  CHECK_THAT(id.a11.imag(), WithinAbs(0.0, TOL));
}

// ---- StateVector ----

TEST_CASE("1-qubit StateVector has size 2", "[statevector]") {
  cpu::StateVector sv(1);
  REQUIRE(sv.size() == 2);
}

TEST_CASE("3-qubit StateVector has size 8", "[statevector]") {
  cpu::StateVector sv(3);
  REQUIRE(sv.size() == 8);
}

TEST_CASE("initial state is |00...0>", "[statevector]") {
  cpu::StateVector sv(3);
  const auto& a = sv.amplitudes();
  CHECK_THAT(a[0].real(), WithinAbs(1.0, TOL));
  CHECK_THAT(a[0].imag(), WithinAbs(0.0, TOL));
  for (std::size_t i = 1; i < sv.size(); ++i) {
    CHECK_THAT(a[i].real(), WithinAbs(0.0, TOL));
    CHECK_THAT(a[i].imag(), WithinAbs(0.0, TOL));
  }
}

TEST_CASE("reset() restores |00...0> after modification", "[statevector]") {
  cpu::StateVector sv(2);
  sv.amplitudes()[0] = 0.0;
  sv.amplitudes()[1] = 1.0;
  sv.reset();
  const auto& a = sv.amplitudes();
  CHECK_THAT(a[0].real(), WithinAbs(1.0, TOL));
  CHECK_THAT(a[0].imag(), WithinAbs(0.0, TOL));
  for (std::size_t i = 1; i < sv.size(); ++i) {
    CHECK_THAT(a[i].real(), WithinAbs(0.0, TOL));
    CHECK_THAT(a[i].imag(), WithinAbs(0.0, TOL));
  }
}

// ---- Matrix entries ----

TEST_CASE("X matrix entries are correct", "[matrix]") {
  auto m = gates::getX();
  CHECK_THAT(m.a00.real(), WithinAbs(0.0, TOL));
  CHECK_THAT(m.a01.real(), WithinAbs(1.0, TOL));
  CHECK_THAT(m.a10.real(), WithinAbs(1.0, TOL));
  CHECK_THAT(m.a11.real(), WithinAbs(0.0, TOL));
}

TEST_CASE("H matrix entries correct within 1e-10", "[matrix]") {
  auto m = gates::getH();
  const double inv_sqrt2 = 1.0 / std::numbers::sqrt2;
  CHECK_THAT(m.a00.real(), WithinAbs(inv_sqrt2, TOL));
  CHECK_THAT(m.a01.real(), WithinAbs(inv_sqrt2, TOL));
  CHECK_THAT(m.a10.real(), WithinAbs(inv_sqrt2, TOL));
  CHECK_THAT(m.a11.real(), WithinAbs(-inv_sqrt2, TOL));
}

TEST_CASE("RX(pi) equals -i*X within 1e-10", "[matrix]") {
  auto m = gates::getRX(std::numbers::pi);
  // RX(π) = [[0, -i], [-i, 0]]
  CHECK_THAT(m.a00.real(), WithinAbs(0.0, TOL));
  CHECK_THAT(m.a00.imag(), WithinAbs(0.0, TOL));
  CHECK_THAT(m.a01.real(), WithinAbs(0.0, TOL));
  CHECK_THAT(m.a01.imag(), WithinAbs(-1.0, TOL));
  CHECK_THAT(m.a10.real(), WithinAbs(0.0, TOL));
  CHECK_THAT(m.a10.imag(), WithinAbs(-1.0, TOL));
  CHECK_THAT(m.a11.real(), WithinAbs(0.0, TOL));
  CHECK_THAT(m.a11.imag(), WithinAbs(0.0, TOL));
}

TEST_CASE("RY(pi) equals -i*Y within 1e-10", "[matrix]") {
  auto m = gates::getRY(std::numbers::pi);
  // RY(π) = [[0, -1], [1, 0]]
  CHECK_THAT(m.a00.real(), WithinAbs(0.0, TOL));
  CHECK_THAT(m.a01.real(), WithinAbs(-1.0, TOL));
  CHECK_THAT(m.a10.real(), WithinAbs(1.0, TOL));
  CHECK_THAT(m.a11.real(), WithinAbs(0.0, TOL));
}

TEST_CASE("RZ(pi) equals -i*Z within 1e-10", "[matrix]") {
  auto m = gates::getRZ(std::numbers::pi);
  // RZ(π) = [[-i, 0], [0, i]]
  CHECK_THAT(m.a00.real(), WithinAbs(0.0, TOL));
  CHECK_THAT(m.a00.imag(), WithinAbs(-1.0, TOL));
  CHECK_THAT(m.a11.real(), WithinAbs(0.0, TOL));
  CHECK_THAT(m.a11.imag(), WithinAbs(1.0, TOL));
}

TEST_CASE("all standard gate matrices are unitary", "[matrix]") {
  check_unitary(gates::getX());
  check_unitary(gates::getY());
  check_unitary(gates::getZ());
  check_unitary(gates::getH());
  check_unitary(gates::getS());
  check_unitary(gates::getSdg());
  check_unitary(gates::getT());
  check_unitary(gates::getTdg());
  check_unitary(gates::getRX(1.23));
  check_unitary(gates::getRY(0.77));
  check_unitary(gates::getRZ(2.01));
}
