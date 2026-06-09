#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace vacua {

using ParameterMap = std::unordered_map<std::string, double>;

class ParameterExpr {
 public:
  static ParameterExpr from_constant(double value) {
    ParameterExpr p{};
    p.kind_ = Kind::Constant;
    p.value_ = value;
    return p;
  }

  static ParameterExpr from_symbol(std::string name) {
    ParameterExpr p{};
    p.kind_ = Kind::Symbol;
    p.name_ = std::move(name);
    return p;
  }

  double evaluate(const ParameterMap& params) const {
    if (kind_ == Kind::Constant) return value_;
    auto it = params.find(name_);
    if (it == params.end())
      throw std::runtime_error("ParameterExpr: symbol '" + name_ + "' not found in map");
    return it->second;
  }

  bool is_constant() const { return kind_ == Kind::Constant; }
  bool is_symbol() const { return kind_ == Kind::Symbol; }
  const std::string& name() const { return name_; }
  double constant_value() const { return value_; }

 private:
  enum class Kind { Constant, Symbol };
  Kind kind_{Kind::Constant};
  double value_{0.0};
  std::string name_{};
};

}  // namespace vacua
