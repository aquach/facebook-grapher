#pragma once

#include "CoreBase.h"
#include "CMath.h"
#include <cmath>

class C3Vector {
public:
    C3Vector () : x(0.0f), y(0.0f), z(0.0f) { };
    C3Vector (real32 x, real32 y, real32 z) : x(x), y(y), z(z) { };

    const real32 magnitude() const { return sqrt(x * x + y * y + z * z); }
    const real32 invMagnitude() const { return CMath::FastInvSqrt(x * x + y * y + z * z); }
    const real32 squareMagnitude () const { return x * x + y * y + z * z; }

    const C3Vector unit() const { return *this * invMagnitude(); }

    const C3Vector& operator+=(const C3Vector& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }

    const C3Vector operator+(const C3Vector& rhs) const {
        return C3Vector(*this) += rhs;
    }

    const C3Vector& operator-=(const C3Vector& rhs) {
        this->x += -1 * rhs.x;
        this->y += -1 * rhs.y;
        this->z += -1 * rhs.z;
        return *this;
    }

    const C3Vector operator-(const C3Vector& rhs) const {
        return C3Vector(*this) -= rhs;
    }

    const C3Vector& operator*=(real32 scale) {
        this->x *= scale;
        this->y *= scale;
        this->z *= scale;
        return *this;
    }

    const C3Vector operator-() const {
        return (*this) * -1;
    }

    const C3Vector operator*(real32 scale) const {
        return C3Vector(*this) *= scale;
    }

    const C3Vector operator*(const C3Vector& rhs) const {
        return C3Vector(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    const C3Vector& operator/=(real32 scale) {
        (*this) *= 1.0f / scale;
        return *this;
    }

    const C3Vector operator/(real32 scale) const {
        return C3Vector(*this) /= scale;
    }

    static real32 dot(const C3Vector& a, const C3Vector& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    void Set (real32 x, real32 y, real32 z) {
       this->x = x;
       this->y = y;
       this->z = z;
    }

    static C3Vector Cross (const C3Vector& a, const C3Vector& b) {
        C3Vector v;
        v.Set(
            a.y * b.z - a.z * b.y,
            -(a.x * b.z - a.z * b.x),
            a.x * b.y - a.y * b.x
            );
        return v;
    }
    static real32 Dist (const C3Vector& a, const C3Vector &b) {
        return sqrt( (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
    }
    void Normalize(){
        real32 total =  0;
        total += this->x * this->x + this->y * this->y + this->z * this->z;
        total = sqrt(total);
        this->x /= total;
        this->y /= total;
        this->z /= total;
    }

    real32 x;
    real32 y;
    real32 z;
};
