#ifndef RIGIDBODY
#define RIGIDBODY

#include<iostream>
#include"field.h"

// currently defaults to sphere shape
struct RigidBody : VectorField
{
    unsigned int radius;
    Vector3<double> centre;
    RigidBody(Vector3<double> cntr, double rad);
};

#endif