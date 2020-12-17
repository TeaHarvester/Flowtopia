#ifndef FIELD
#define FIELD

// for storing vector fields and scalar fields

#include<iostream>
#include<vector>
#include"helpers.h"

struct VectorField
{
    unsigned int x_size, y_size, z_size;
    double norm_coeff;
    Vector3** map;
    Vector3*& operator() (unsigned int x, unsigned int y, unsigned int z);
    VectorField(unsigned int x_dim, unsigned int y_dim, unsigned int z_dim);
    VectorField(const VectorField& copy);
    ~VectorField();
};

struct ScalarField
{
    unsigned int x_size, y_size, z_size;
    double* map;
    double& operator() (unsigned int x, unsigned int y, unsigned int z);
    ScalarField(unsigned int x_dim, unsigned int y_dim, unsigned int z_dim);
    ~ScalarField();
};

#endif