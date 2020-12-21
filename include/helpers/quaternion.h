#ifndef QUATERNION
#define QUATERNION

#include<iostream>
#include"helpers/vector3.h"
#include"math.h"

template<typename T>
struct Quaternion
{
    Vector3<T> i_vector;
    T w;
    Quaternion<T> Conj();
    Quaternion<T> Rotate(T angle, Vector3<T> direction);
    void operator=(Quaternion<T> b);
    void operator=(Vector3<T> b);
    Quaternion<T> operator*(Quaternion<T> b);
    Quaternion<T> operator*(Vector3<T> b);
    Quaternion(T i, T j, T k, T w);
};

#endif