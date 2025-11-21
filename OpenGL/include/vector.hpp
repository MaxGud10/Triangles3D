#pragma once

#include "point.hpp"

namespace triangle 
{

template <typename PointTy = double> struct Vector 
{
    PointTy x = NAN, y = NAN, z = NAN;

    void print() const 
    {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    void normalize() 
    {
        PointTy normal_length = std::sqrt(x * x + y * y + z * z);

        if (cmp(normal_length, 0.0))
        return;

        x = x / normal_length;
        y = y / normal_length;
        z = z / normal_length;
    }

    bool operator==(const Vector<PointTy> &other) const = default;
};

template <typename PointTy = double> bool valid(const Vector<PointTy> &vector) 
{
    return !std::isnan(vector.x) && !std::isnan(vector.y) &&
            !std::isnan(vector.z);
}

template <typename PointTy = double>
bool equal(const Vector<PointTy> &vector1, const Vector<PointTy> &vector2) 
{
    if (!valid(vector1) || !valid(vector2))
        return false;

    return cmp(vector1.x, vector2.x) && cmp(vector1.y, vector2.y) &&
            cmp(vector1.z, vector2.z);
}

template <typename PointTy = double>
PointTy dot(const Vector<PointTy> &vector1, const Vector<PointTy> &vector2) 
{
    return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

template <typename PointTy = double>
Vector<PointTy> cross(const Vector<PointTy> &vector1,
                      const Vector<PointTy> &vector2) 
{
    Vector<PointTy> vector(vector1.y * vector2.z - vector1.z * vector2.y,
                            vector1.z * vector2.x - vector1.x * vector2.z,
                            vector1.x * vector2.y - vector1.y * vector2.x);
    return vector;
}
} // namespace triangle