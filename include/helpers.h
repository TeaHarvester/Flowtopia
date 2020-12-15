#ifndef HELPERS
#define HELPERS

#include<iostream>

struct Vector3
{
    double data[3];

    double Dot(Vector3& operand);
    Vector3 operator+(Vector3& a);
    Vector3 operator*(Vector3& a);
    Vector3 operator/(Vector3& a);
    Vector3();
    Vector3(int i, int j, int k);
    ~Vector3();

    double& operator()(unsigned int i)
    {
        if (i < 0 || i > 2)
        {
            std::cout << "index out of bounds" << std::endl;            
        }

        return data[i];
    }
};

#endif