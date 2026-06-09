#pragma once

#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace vacua::cpu {
class Measurements;
}

namespace vacua {

struct Shots {
  explicit Shots(std::size_t n) : value{n} {}
  std::size_t value;
};

using Gradient = std::unordered_map<std::string, double>;

class Result {
 public:
  using Counts = std::map<std::string, std::size_t>;

  const Counts& counts() const { return counts_; }
  const std::vector<double>& probabilities() const { return probabilities_; }

  void print_counts(std::ostream& os = std::cout) const {
    for (const auto& [bitstring, count] : counts_) os << bitstring << ": " << count << "\n";
  }

 private:
  friend class vacua::cpu::Measurements;
  Counts counts_{};
  std::vector<double> probabilities_{};
};

}  // namespace vacua
