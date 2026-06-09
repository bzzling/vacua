#pragma once

#include "vacua/base/matrix.hpp"

namespace vacua::gates {

Matrix2 getX();
Matrix2 getY();
Matrix2 getZ();
Matrix2 getH();
Matrix2 getS();
Matrix2 getSdg();
Matrix2 getT();
Matrix2 getTdg();
Matrix2 getRX(double theta);
Matrix2 getRY(double theta);
Matrix2 getRZ(double theta);

}  // namespace vacua::gates
