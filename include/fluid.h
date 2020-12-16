#ifndef FLUID
#define FLUID

#include<iostream>
#include"field.h"

class Fluid
{
    public:
    // void InitialiseFluid();
    void FiniteDifference();
    Fluid(unsigned int x_dim, unsigned int y_dim, unsigned int z_dim, double k_visc);
    ~Fluid();
    private:
    unsigned int x_size, y_size, z_size;
    double k_visc;
    VectorField* u;
    ScalarField* density;

};

#endif