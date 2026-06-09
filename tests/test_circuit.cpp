#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include "vacua/vacua.hpp"

using namespace vacua;

TEST_CASE("num_qubits returns correct value", "[circuit]") {
  Circuit c(3);
  REQUIRE(c.num_qubits() == 3);
}

TEST_CASE("instructions are recorded in order", "[circuit]") {
  Circuit c(2);
  c.h(0).cx(0, 1).measure_all();
  const auto& instr = c.instructions();
  REQUIRE(instr.size() == 3);
  REQUIRE(instr[0].op == OpType::H);
  REQUIRE(instr[1].op == OpType::CX);
  REQUIRE(instr[2].op == OpType::MeasureAll);
}

TEST_CASE("invalid qubit index throws std::out_of_range", "[circuit]") {
  Circuit c(2);
  REQUIRE_THROWS_AS(c.x(2), std::out_of_range);
  REQUIRE_THROWS_AS(c.cx(0, 5), std::out_of_range);
  REQUIRE_THROWS_AS(c.barrier(99), std::out_of_range);
}

TEST_CASE("parameterized RY stores symbol correctly", "[circuit]") {
  Circuit c(1);
  c.ry(0, ParameterExpr::from_symbol("theta"));
  const auto& instr = c.instructions();
  REQUIRE(instr.size() == 1);
  REQUIRE(instr[0].op == OpType::RY);
  REQUIRE(instr[0].param.has_value());
  REQUIRE(instr[0].param->is_symbol());
  REQUIRE(instr[0].param->name() == "theta");
}

TEST_CASE("ParameterExpr::evaluate resolves constants", "[parameter]") {
  auto p = ParameterExpr::from_constant(3.14);
  ParameterMap empty;
  REQUIRE(p.evaluate(empty) == 3.14);
}

TEST_CASE("ParameterExpr::evaluate resolves symbols from map", "[parameter]") {
  auto p = ParameterExpr::from_symbol("alpha");
  ParameterMap params{{"alpha", 1.57}};
  REQUIRE(p.evaluate(params) == 1.57);
}

TEST_CASE("ParameterExpr::evaluate throws when symbol missing", "[parameter]") {
  auto p = ParameterExpr::from_symbol("beta");
  ParameterMap empty;
  REQUIRE_THROWS_AS(p.evaluate(empty), std::runtime_error);
}

TEST_CASE("measure_all records MeasureAll instruction", "[circuit]") {
  Circuit c(4);
  c.measure_all();
  REQUIRE(c.instructions().size() == 1);
  REQUIRE(c.instructions()[0].op == OpType::MeasureAll);
}

TEST_CASE("barrier records Barrier instruction", "[circuit]") {
  Circuit c(2);
  c.barrier(1);
  REQUIRE(c.instructions().size() == 1);
  REQUIRE(c.instructions()[0].op == OpType::Barrier);
  REQUIRE(c.instructions()[0].qubits[0] == 1);
}
