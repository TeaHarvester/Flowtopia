#ifndef HELPERS
#define HELPERS

#include<iostream>

struct Vector3
{
    double data[3];

    double Dot(Vector3& operand);
    double& operator()(unsigned int i);
    void operator+=(Vector3& a);
    Vector3 operator+(Vector3& a);
    Vector3 operator*(Vector3& a);
    Vector3 operator/(Vector3& a);
    Vector3();
    Vector3(int i, int j, int k);
    Vector3(const Vector3& copy);
    ~Vector3();
};

#endif