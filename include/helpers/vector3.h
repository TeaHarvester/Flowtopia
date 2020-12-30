#ifndef VECTOR3
#define VECTOR3

#include<iostream>
#include"math.h"

template<typename T>
struct Vector3
{
    T data[3];

    double Dot(Vector3<T>& operand);
    Vector3<T> Cross(Vector3<T>& operand);
    double Magnitude();
    T& operator()(unsigned int i);
    void operator=(Vector3<T>& a);
    void operator+=(Vector3<T>& a);
    Vector3<T> operator+(Vector3<T>& a);
    void operator*=(T a);
    Vector3<T> operator*(Vector3<T>& a);
    Vector3<T> operator/(T a);
    Vector3<T> operator/(Vector3<T>& a);
    Vector3();
    Vector3(T i, T j, T k);
    Vector3(const Vector3<T>& copy);
};

#endif