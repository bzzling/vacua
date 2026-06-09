#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <numbers>

#include "vacua/vacua.hpp"

using namespace vacua;
using namespace Catch::Matchers;

static constexpr double TOL = 1e-10;

TEST_CASE("Bell state over 1000 shots only produces '00' and '11'", "[device]") {
  Circuit c{2};
  c.h(0).cx(0, 1);
  Device dev{};
  auto result{dev.run(c, Shots{1000})};
  for (const auto& [bits, count] : result.counts()) {
    REQUIRE((bits == "00" || bits == "11"));
    (void)count;
  }
}

TEST_CASE("Bell state counts sum to 1000", "[device]") {
  Circuit c{2};
  c.h(0).cx(0, 1);
  Device dev{};
  auto result{dev.run(c, Shots{1000})};
  std::size_t total{0};
  for (const auto& [bits, count] : result.counts()) total += count;
  REQUIRE(total == 1000);
}

TEST_CASE("Bell state both '00' and '11' appear with seed 42", "[device]") {
  Circuit c{2};
  c.h(0).cx(0, 1);
  Device dev{};
  auto result{dev.run(c, Shots{1000})};
  REQUIRE(result.counts().count("00") > 0);
  REQUIRE(result.counts().count("11") > 0);
}

TEST_CASE("deterministic |0> state gives only '0' every shot", "[device]") {
  Circuit c{1};
  Device dev{};
  auto result{dev.run(c, Shots{100})};
  REQUIRE(result.counts().size() == 1);
  REQUIRE(result.counts().at("0") == 100);
}

TEST_CASE("X gate gives only '1' every shot", "[device]") {
  Circuit c{1};
  c.x(0);
  Device dev{};
  auto result{dev.run(c, Shots{100})};
  REQUIRE(result.counts().size() == 1);
  REQUIRE(result.counts().at("1") == 100);
}

TEST_CASE("statevector() returns correct amplitudes for Bell state", "[device]") {
  Circuit c{2};
  c.h(0).cx(0, 1);
  Device dev{};
  auto sv{dev.statevector(c)};
  const auto& a{sv.amplitudes()};
  const double inv_sqrt2{1.0 / std::numbers::sqrt2};
  CHECK_THAT(a[0].real(), WithinAbs(inv_sqrt2, TOL));
  CHECK_THAT(a[0].imag(), WithinAbs(0.0, TOL));
  CHECK_THAT(a[3].real(), WithinAbs(inv_sqrt2, TOL));
  CHECK_THAT(a[3].imag(), WithinAbs(0.0, TOL));
  CHECK_THAT(a[1].real(), WithinAbs(0.0, TOL));
  CHECK_THAT(a[2].real(), WithinAbs(0.0, TOL));
}

TEST_CASE("same seed produces identical results across two runs", "[device]") {
  Circuit c{2};
  c.h(0).cx(0, 1);
  Device dev1{DeviceType::CPU, 99};
  Device dev2{DeviceType::CPU, 99};
  auto r1{dev1.run(c, Shots{500})};
  auto r2{dev2.run(c, Shots{500})};
  REQUIRE(r1.counts() == r2.counts());
}

TEST_CASE("Shots{1000} count total equals 1000", "[device]") {
  Circuit c{2};
  c.h(0).h(1);
  Device dev{};
  auto result{dev.run(c, Shots{1000})};
  std::size_t total{0};
  for (const auto& [bits, count] : result.counts()) total += count;
  REQUIRE(total == 1000);
}
