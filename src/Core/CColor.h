#pragma once

#include "CoreBase.h"

class CColor {
public:
   uint8 r;
   uint8 g;
   uint8 b;
   uint8 a;

   CColor () : r(0), g(0), b(0), a(255) { };
   CColor (uint8 r, uint8 g, uint8 b) : r(r), g(g), b(b), a(255) { };
   CColor (uint8 r, uint8 g, uint8 b, uint8 a) : r(r), g(g), b(b), a(a) { };

   bool operator== (const CColor& rhs) {
      return (r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a = rhs.a);
   }

   void ToC3Vector (C3Vector& vector) {
       vector.Set(r / 255.0f, g / 255.0f, b / 255.0f);
   }

   uint ToInt () {
        uint color;
        color = ((color & 0xffffff00) | r);
        color = ((color & 0xffff00ff) | (g << 8));
        color = ((color & 0xff00ffff) | (b << 16));
        color = ((color & 0x00ffffff) | (a << 24));
        return color;
   }

   void FromC3Vector (const C3Vector& vector) {
       Set((uint8)(vector.x * 255),
           (uint8)(vector.y * 255),
           (uint8)(vector.z * 255));
   }

   void Set (real32 r, real32 g, real32 b, real32 a) {
       Set(r, g, b);
       this->a = (uint8)(a * 255);
   }
   void Set (real32 r, real32 g, real32 b) {
       C3Vector vector(r, g, b);
       FromC3Vector(vector);
   }
   void Set (uint8 r, uint8 g, uint8 b, uint8 a) {
        Set(r, g, b);
        this->a = a;
   }
   void Set (uint8 r, uint8 g, uint8 b) {
        this->r = r;
        this->g = g;
        this->b = b;
   }
};