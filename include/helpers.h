#ifndef HELPERS
#define HELPERS

#include<iostream>
#include"math.h"

struct Vector3
{
    double data[3];

    double Dot(Vector3& operand);
    double Magnitude();
    double& operator()(unsigned int i);
    void operator+=(Vector3& a);
    Vector3 operator+(Vector3& a);
    Vector3 operator*(Vector3& a);
    Vector3 operator/(double a);
    Vector3 operator/(Vector3& a);
    Vector3();
    Vector3(double i, double j, double k);
    Vector3(const Vector3& copy);
    ~Vector3();
};

#endif