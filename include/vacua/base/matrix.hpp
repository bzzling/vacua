#pragma once

#include <complex>

namespace vacua {

struct Matrix2 {
  std::complex<double> a00, a01, a10, a11;

  Matrix2 adjoint() const {
    return {std::conj(a00), std::conj(a10), std::conj(a01), std::conj(a11)};
  }
};

}  // namespace vacua
