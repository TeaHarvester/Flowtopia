#include"helpers/vector3.h"

template<typename T>
T& Vector3<T>::operator()(unsigned int i)
{
    if (i < 0 || i > 2)
    {
        std::cout << "index out of bounds" << std::endl;            
    }

    return data[i];
}

template<typename T>
void Vector3<T>::operator=(Vector3<T>& a)
{
    this->data[0] = a.data[0];
    this->data[1] = a.data[1];
    this->data[2] = a.data[2];
}

template<typename T>
void Vector3<T>::operator+=(Vector3<T>& a)
{
    this->data[0] += a.data[0];
    this->data[1] += a.data[1];
    this->data[2] += a.data[2];
}

template<typename T>
Vector3<T> Vector3<T>::operator+(Vector3<T>& a)
{
    return Vector3<T>((*this)(1) + a(1), (*this)(2) + a(2), (*this)(3) + a(3));
}

template<typename T>
void Vector3<T>::operator*=(T a)
{
    this->data[0] *= a;
    this->data[1] *= a;
    this->data[2] *= a;
}

// element-wise multiplication
template<typename T>
Vector3<T> Vector3<T>::operator*(Vector3<T>& a)
{
    return Vector3<T>((*this)(1) * a(1), (*this)(2) * a(2), (*this)(3) * a(3));
}

// division by scalar
template<typename T>
Vector3<T> Vector3<T>::operator/(T a)
{
    return Vector3<T>((*this)(1) / a, (*this)(2) / a, (*this)(3) / a);
}

// element-wise division
template<typename T>
Vector3<T> Vector3<T>::operator/(Vector3<T>& a)
{
    return Vector3<T>((*this)(1) / a(1), (*this)(2) / a(2), (*this)(3) / a(3));
}

template<typename T>
double Vector3<T>::Dot(Vector3<T>& operand)
{
    return data[0]*operand.data[0] + data[1]*operand.data[1] + data[2]*operand.data[2];
}

template<typename T>
double Vector3<T>::Magnitude()
{
    return sqrt(this->Dot(*this));
}

template<typename T>
Vector3<T>::Vector3()
{
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
}

template<typename T>
Vector3<T>::Vector3(T i, T j, T k)
{
    data[0] = i;
    data[1] = j;
    data[2] = k;
}

template<typename T>
Vector3<T>::Vector3(const Vector3<T>& copy)
{
    data[0] = copy.data[0];
    data[1] = copy.data[1];
    data[2] = copy.data[2];
}

template class Vector3<float>;
template class Vector3<double>;