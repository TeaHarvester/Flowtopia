#include"helpers.h"

double& Vector3::operator()(unsigned int i)
{
    if (i < 0 || i > 2)
    {
        std::cout << "index out of bounds" << std::endl;            
    }

    return data[i];
}

void Vector3::operator+=(Vector3& a)
{
    this->data[0] += a.data[0];
    this->data[1] += a.data[1];
    this->data[2] += a.data[2];
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

// division by scalar
Vector3 Vector3::operator/(double a)
{
    return Vector3((*this)(1) / a, (*this)(2) / a, (*this)(3) / a);
}

// element-wise division
Vector3 Vector3::operator/(Vector3& a)
{
    return Vector3((*this)(1) / a(1), (*this)(2) / a(2), (*this)(3) / a(3));
}

double Vector3::Dot(Vector3& operand)
{
    return data[0]*operand.data[0] + data[1]*operand.data[1] + data[2]*operand.data[2];
}

double Vector3::Magnitude()
{
    return sqrt(this->Dot(*this));
}

Vector3::Vector3()
{
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
}

Vector3::Vector3(double i, double j, double k)
{
    data[0] = i;
    data[1] = j;
    data[2] = k;
}

Vector3::Vector3(const Vector3& copy)
{
    data[0] = copy.data[0];
    data[1] = copy.data[1];
    data[2] = copy.data[2];
}