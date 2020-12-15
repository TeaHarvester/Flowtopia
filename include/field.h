#ifndef FIELD
#define FIELD

// for storing vector fields and scalar fields

#include<iostream>
#include<vector>
#include"helpers.h"

class VectorField
{
    public:
    VectorField(unsigned int x_dim, unsigned int y_dim, unsigned int z_dim, unsigned int t_dim);
    ~VectorField();

    private:
    unsigned int x_size, y_size, z_size, t_size;
    Vector3** map;
    Vector3*& operator() (unsigned int x, unsigned int y, unsigned int z, unsigned int t);

};

class ScalarField
{
    public:
    ScalarField(unsigned int x_dim, unsigned int y_dim, unsigned int z_dim, unsigned int t_dim);
    ~ScalarField();

    private:
    unsigned int x_size, y_size, z_size, t_size;
    double* map;
    double& operator() (unsigned int x, unsigned int y, unsigned int z, unsigned int t);

};

#endif