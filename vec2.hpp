#ifndef VEC2_DEFINED_H
#define VEC2_DEFINED_H

#include <iostream>

struct Vec2 {
    float x = 0;
    float y = 0;
};

Vec2 operator+(const Vec2 &v1, const Vec2 &v2);
Vec2 operator-(const Vec2 &v1, const Vec2 &v2);
Vec2 operator*(const Vec2 &v1, const Vec2 &v2);
Vec2 operator/(const Vec2 &v1, const Vec2 &v2);
void operator+=(Vec2 &v1, const Vec2 &v2);
void operator-=(Vec2 &v1, const Vec2 &v2);
void operator*=(Vec2 &v1, const Vec2 &v2);
void operator/=(Vec2 &v1, const Vec2 &v2);

Vec2 operator+(const Vec2 &v, float f);
Vec2 operator-(const Vec2 &v, float f);
Vec2 operator*(const Vec2 &v, float f);
Vec2 operator/(const Vec2 &v, float f);
Vec2 operator+(float f, const Vec2 &v);
Vec2 operator-(float f, const Vec2 &v);
Vec2 operator*(float f, const Vec2 &v);
Vec2 operator/(float f, const Vec2 &v);
void operator+=(Vec2 &v, float f);
void operator-=(Vec2 &v, float f);
void operator*=(Vec2 &v, float f);
void operator/=(Vec2 &v, float f);

float dot(const Vec2 &v1, const Vec2 &v2);

bool operator==(const Vec2 &v, float f);
bool operator==(const Vec2 &v1, const Vec2 &v2);
bool operator!=(const Vec2 &v, float f);
bool operator!=(const Vec2 &v1, const Vec2 &v2);
bool operator<(const Vec2 &v1, const Vec2 &v2);
bool operator>(const Vec2 &v1, const Vec2 &v2);

void operator-(Vec2 &v);

float length(const Vec2 &v);
float length_sq(const Vec2 &v);

void normalize(Vec2 &v);
Vec2 normalized(const Vec2 &v);

// rounds to 5 digits
Vec2 round(const Vec2 &v);

std::ostream &operator<<(std::ostream &os, const Vec2 &v);

#endif