/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */

#pragma once
#include <math.h>

/** 3d vector class with extra utility functions and overloaded operators */
struct Vec3 {
    float x, y, z;
    //float& r = x, g = y, b = z;
    //float& w = x, l = y, h = z;

    // ============================================================================================
    // Additional functionality
    // ============================================================================================

    /** Returns the length of the vector */
    float length() {
        return sqrt(x * x + y * y + z * z);
    }

    // Returns the length squared of the vector
    float length2() {
        return x * x + y * y + z * z;
    }

    // Returns the distance between two vectors
    float dist(Vec3& b) {
        return (*this - b).length();
    }

    // Returns the distance squared between two vectors
    float dist2(Vec3& b) {
        return (*this - b).length2();
    }

    /** Returns the dot product of two vectors */
    float dot(const Vec3& b) {
        return x * b.x + y * b.y + z * b.z;
    }

    /** Normalizes the vector */
    Vec3 normalize() {
        return *this /= length();
    }

    // Returns a normalized version of a vector
    static Vec3 normalize(Vec3 b) {
        return b / b.length();
    }

    // ============================================================================================
    // Operator overloading
    // ============================================================================================

    Vec3 operator=(const Vec3& b) {
        return {b.x, b.y, b.z};
    }

    Vec3 operator+(const Vec3& b) {
        return {x+b.x, y+b.y, z+b.z};
    }

    Vec3 operator+=(const Vec3& b) {
        return {x += b.x, y += b.y, z += b.z};
    }

    Vec3 operator-() {
        return {-x, -y, -z};
    }

    Vec3 operator-(const Vec3& b) {
        return {x-b.x, y-b.y, z-b.z};
    }
    
    Vec3 operator -=(const Vec3& b) {
        return {x -= b.x, y -= b.y, z -= b.z};
    }

    Vec3 operator*(const Vec3& b) {
        return {x*b.x, y*b.y, z*b.z};
    }

    Vec3 operator*=(const Vec3& b) {
        return {x *= b.x, y *= b.y, z *= b.z};
    }

    Vec3 operator*(const float& b) {
        return {x*b, y*b, z*b};
    }

    Vec3 operator*=(const float& b) {
        return {x *= b, y *= b, z *= b};
    }

    Vec3 operator/(const Vec3& b) {
        return {x/b.x, y/b.y, z/b.z};
    }

    Vec3 operator/=(const Vec3& b) {
        return {x /= b.x, y /= b.y, z /= b.z};
    }

    Vec3 operator/(const float& b) {
        return {x/b, y/b, z/b};
    }

    Vec3 operator/=(const float& b) {
        return {x /= b, y /= b, z /= b};
    }

    bool operator==(const Vec3& b) {
        return x == b.x && y == b.y && z == b.z;
    }
};