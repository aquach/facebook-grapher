#pragma once

#include "CoreBase.h"

class C2Vector {
public:
   real32 x;
   real32 y;

   C2Vector() : x(0.0f), y(0.0f) { };
   C2Vector(real32 x, real32 y) : x(x), y(y) { };

};