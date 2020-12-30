#include<iostream>
#include"helpers/quaternion.h"

template<typename T>
Quaternion<T> Quaternion<T>::Conj()
{
    return Quaternion<T>(w, -i_vector(0), -i_vector(1), -i_vector(2));
}

template<typename T>
void Quaternion<T>::operator=(Quaternion<T> b)
{
    w = b.w;
    i_vector = b.i_vector;
}

template<typename T>
void Quaternion<T>::operator=(Vector3<T> b)
{
    w = 0;
    i_vector = b;
}

template<typename T>
Quaternion<T> Quaternion<T>::Rotate(T angle, Vector3<T> dir)
{
    Quaternion<T> rotation = Quaternion<T>(cos(angle), sin(angle)*dir(0), sin(angle)*dir(1), sin(angle)*dir(2));

    return (rotation*dir)*rotation.Conj();
}

template<typename T>
Quaternion<T> Quaternion<T>::operator*(Quaternion<T> b)
{
    T new_w = w*b.w - i_vector(0)*b.i_vector(0) - i_vector(1)*b.i_vector(1) - i_vector(2)*b.i_vector(2);
    T new_i = w*b.i_vector(0) + i_vector(0)*b.w + i_vector(1)*b.i_vector(2) - i_vector(2)*b.i_vector(1);
    T new_j = w*b.i_vector(1) - i_vector(0)*b.i_vector(2) + i_vector(1)*b.w + i_vector(2)*b.i_vector(0);
    T new_k = w*b.i_vector(2) + i_vector(0)*b.i_vector(1) - i_vector(1)*b.i_vector(0) + i_vector(2)*b.w;

    return Quaternion<T>(new_w, new_i, new_j, new_k);
}

template<typename T>
Quaternion<T> Quaternion<T>::operator*(Vector3<T> b)
{
    return *this*Quaternion(0, b(0), b(1), b(2));
}

template<typename T>
Quaternion<T>::Quaternion(T w, T i, T j, T k)
:
i_vector(i, j, k),
w(w)
{}

template class Quaternion<float>;
template class Quaternion<double>;