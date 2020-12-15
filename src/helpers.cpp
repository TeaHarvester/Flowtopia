#include"helpers.h"

double Vector3::Dot(Vector3& operand)
{
    return data[0]*operand.data[0] + data[1]*operand.data[1] + data[2]*operand.data[2];
}

Vector3::Vector3()
{
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
}

Vector3::Vector3(int i, int j, int k)
{
    data[0] = i;
    data[1] = j;
    data[2] = k;
}

Vector3 Vector3::operator+(Vector3& a)
{
    return Vector3((*this)(1) + a(1), (*this)(2) + a(2), (*this)(3) + a(3));
}

// element-wise multiplication
Vector3 Vector3::operator*(Vector3& a)
{
    return Vector3((*this)(1) * a(1), (*this)(2) * a(2), (*this)(3) * a(3));
}

// element-wise division
Vector3 Vector3::operator/(Vector3& a)
{
    return Vector3((*this)(1) / a(1), (*this)(2) / a(2), (*this)(3) / a(3));
}