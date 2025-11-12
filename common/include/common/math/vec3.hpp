#pragma once
#include <cmath>
#include <ostream>

namespace common::math {
struct Vec3 {
    float x{0.f};
    float y{0.f};
    float z{0.f};

    Vec3() = default;
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator+(const Vec3& rhs) const { return Vec3{x + rhs.x, y + rhs.y, z + rhs.z}; }
    Vec3 operator-(const Vec3& rhs) const { return Vec3{x - rhs.x, y - rhs.y, z - rhs.z}; }
    Vec3 operator*(float s) const { return Vec3{x * s, y * s, z * s}; }
    Vec3 operator/(float s) const { return Vec3{x / s, y / s, z / s}; }
    Vec3& operator+=(const Vec3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
    Vec3& operator-=(const Vec3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
    Vec3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }

    float length() const { return std::sqrt(x * x + y * y + z * z); }
};

inline float Dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 Normalize(const Vec3& v) {
    float len = v.length();
    if (len <= 0.0001f) return Vec3{};
    return v / len;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}
} // namespace common::math
