#include "vacua/gates/gates.hpp"

#include <cmath>
#include <numbers>

namespace vacua::gates {

Matrix2 getX() {
  return {0, 1, 1, 0};
}

Matrix2 getY() {
  using namespace std::complex_literals;
  return {0, -1i, 1i, 0};
}

Matrix2 getZ() {
  return {1, 0, 0, -1};
}

Matrix2 getH() {
  constexpr double inv_sqrt2{1.0 / std::numbers::sqrt2};
  return {inv_sqrt2, inv_sqrt2, inv_sqrt2, -inv_sqrt2};
}

Matrix2 getS() {
  using namespace std::complex_literals;
  return {1, 0, 0, 1i};
}

Matrix2 getSdg() {
  using namespace std::complex_literals;
  return {1, 0, 0, -1i};
}

Matrix2 getT() {
  using namespace std::complex_literals;
  constexpr double inv_sqrt2{1.0 / std::numbers::sqrt2};
  return {1, 0, 0, inv_sqrt2 + inv_sqrt2 * 1i};
}

Matrix2 getTdg() {
  using namespace std::complex_literals;
  constexpr double inv_sqrt2{1.0 / std::numbers::sqrt2};
  return {1, 0, 0, inv_sqrt2 - inv_sqrt2 * 1i};
}

Matrix2 getRX(double theta) {
  using namespace std::complex_literals;
  const double c{std::cos(theta / 2.0)};
  const double s{std::sin(theta / 2.0)};
  return {c, -s * 1i, -s * 1i, c};
}

Matrix2 getRY(double theta) {
  const double c{std::cos(theta / 2.0)};
  const double s{std::sin(theta / 2.0)};
  return {c, -s, s, c};
}

Matrix2 getRZ(double theta) {
  using namespace std::complex_literals;
  const double half{theta / 2.0};
  return {std::exp(-half * 1i), 0, 0, std::exp(half * 1i)};
}

}  // namespace vacua::gates
