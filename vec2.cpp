#include "vec2.hpp"

#include <math.h>

Vec2 operator+(const Vec2 &v1, const Vec2 &v2) {
    Vec2 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    return result;
}

Vec2 operator-(const Vec2 &v1, const Vec2 &v2) {
    Vec2 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    return result;
}

Vec2 operator*(const Vec2 &v1, const Vec2 &v2) {
    Vec2 result;
    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    return result;
}

Vec2 operator/(const Vec2 &v1, const Vec2 &v2) {
    Vec2 result;
    result.x = v1.x / v2.x;
    result.y = v1.y / v2.y;
    return result;
}

void operator+=(Vec2 &v1, const Vec2 &v2) {
    v1.x += v2.x;
    v1.y += v2.y;
}

void operator-=(Vec2 &v1, const Vec2 &v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
}

void operator*=(Vec2 &v1, const Vec2 &v2) {
    v1.x *= v2.x;
    v1.y *= v2.y;
}

void operator/=(Vec2 &v1, const Vec2 &v2) {
    v1.x /= v2.x;
    v1.y /= v2.y;
}

Vec2 operator+(const Vec2 &v, float f) {
    Vec2 result;
    result.x = v.x + f;
    result.y = v.y + f;
    return result;
}

Vec2 operator-(const Vec2 &v, float f) {
    Vec2 result;
    result.x = v.x - f;
    result.y = v.y - f;
    return result;
}

Vec2 operator*(const Vec2 &v, float f) {
    Vec2 result;
    result.x = v.x * f;
    result.y = v.y * f;
    return result;
}

Vec2 operator/(const Vec2 &v, float f) {
    Vec2 result;
    result.x = v.x / f;
    result.y = v.y / f;
    return result;
}

Vec2 operator+(float f, const Vec2 &v) {
    return v + f;
}

Vec2 operator-(float f, const Vec2 &v) {
    return v - f;
}

Vec2 operator*(float f, const Vec2 &v) {
    return v * f;
}

Vec2 operator/(float f, const Vec2 &v) {
    return v / f;
}

void operator+=(Vec2 &v, float f) {
    v.x += f;
    v.y += f;
}

void operator-=(Vec2 &v, float f) {
    v.x -= f;
    v.y -= f;
}

void operator*=(Vec2 &v, float f) {
    v.x *= f;
    v.y *= f;
}

void operator/=(Vec2 &v, float f) {
    v.x /= f;
    v.y /= f;
}

float dot(const Vec2 &v1, const Vec2 &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

bool operator==(const Vec2 &v, float f) {
    return (v.x == f && v.y == f);
}

bool operator==(const Vec2 &v1, const Vec2 &v2) {
    return (v1.x == v2.x && v1.y == v2.y);
}

bool operator!=(const Vec2 &v, float f) {
    return (v.x != f || v.y != f);
}

bool operator!=(const Vec2 &v1, const Vec2 &v2) {
    return (v1.x != v2.x || v1.y != v2.y);
}

bool operator<(const Vec2 &v1, const Vec2 &v2) {
    return length_sq(v1) < length_sq(v2);
}

bool operator>(const Vec2 &v1, const Vec2 &v2) {
    return length_sq(v1) > length_sq(v2);
}

void operator-(Vec2 &v) {
    v.x = -v.x;
    v.y = -v.y;
}

float length(const Vec2 &v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

float length_sq(const Vec2 &v) {
    return v.x * v.x + v.y * v.y;
}

void normalize(Vec2 &v) {
    float l = length(v);
    if (l != 0)
        v /= l;
}

Vec2 normalized(const Vec2 &v) {
    float l = length(v);
    Vec2 result = (l != 0) ? v / l : v;
    return result;
}

Vec2 round(const Vec2 &v) {
    Vec2 result;
    result.x = ((int)(v.x * 100000 + 0.5f)) / 100000.f;
    result.y = ((int)(v.y * 100000 + 0.5f)) / 100000.f;
    return result;
}

std::ostream &operator<<(std::ostream &os, const Vec2 &v) {
    os << "x:" << v.x << " y:" << v.y;
    return os;
}