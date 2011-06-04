#pragma once

#include "C2Vector.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

const real32 c_pi = 3.14159265f;
const real32 c_e = 2.71828182845f;

const real32 c_deg2rad = 3.14159f / 180;
const real32 c_rad2deg = 1 / c_deg2rad;

class CMath {
public:

   static real32 Distance (real32 x1, real32 y1, real32 x2, real32 y2) {
      return sqrt((real32)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
   }

   static real32 Abs (real32 num) {
      return (num > 0) ? num : -num;
   }

   static int RandInteger (int low, int high) {
       if (low == high) return low;

      RandomStart();

       if (!randomized_yet) {
           srand((unsigned)time(0));
           randomized_yet = true;
       }
       return rand() % (high - low) + low;
   }

   static real32 Rand () {
      RandomStart();
      return (real32)rand() / RAND_MAX;
   }

   static real32 RandNumber (real32 low, real32 high) {
      RandomStart();

      return Rand() * (high - low) + low;
   }

   static real32 RandRange (real32 range) {
      return RandNumber(-range, range);
   }

   static real32 Clamp (real32 value, real32 low, real32 high) {
        if (value < low)
           return low;
       if (value > high)
           return high;
       return value;
   }

   static int Clamp (int value, int low, int high) {
        if (value < low)
           return low;
       if (value > high)
           return high;
       return value;
   }
    
   static real32 Min (real32 v1, real32 v2) {
      return (v1 < v2) ? v1 : v2;
   }

   static real32 Max (real32 v1, real32 v2) {
      return (v1 > v2) ? v1 : v2;
   }


   static int Round (real32 d) {
      return (int)(d + .5);
   }

   static real32 ToRadians (real32 deg) {
      return deg / 180 * c_pi;
   }

   static real32 ToDegrees (real32 rad) {
      return rad / c_pi * 180;
   }

   static real32 DistanceBetweenAngles (real32 a1, real32 a2) {
      real32 angle = abs(a1 - a2);
       if (angle >= 360) angle -= 360;

       if(angle > 180)
           angle = 360 - angle;

       return angle;
   }

    static real32 FastInvSqrt (float x) {
        union {
            float f;
            int i;
        } tmp;
        tmp.f = x;
        tmp.i = 0x5f3759df - (tmp.i >> 1);
        float y = tmp.f;
        return y * (1.5f - 0.5f * x * y * y);
    }
    
   static C2Vector ToC2Vector (real32 angle) {
      return C2Vector(cos(ToRadians(angle)), -sin(ToRadians(angle)));
   }

    // Returns a new vector pointing at some random angle
   // less than range degrees away from the dir.
   static C2Vector RandomAngle (C2Vector dir, real32 range) {
       real32 angle = -atan2(dir.y, dir.x) * 180 / c_pi;
       angle += RandRange(range);
       return ToC2Vector(angle);
   }

        //static const real32 SLUSH;
    private:
        static bool randomized_yet;

      static void RandomStart () {
          if (!randomized_yet) {
              srand((unsigned)time(0));
              randomized_yet = true;
          }
      }
};