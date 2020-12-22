#ifndef FLUID
#define FLUID

#include<iostream>
#include"field.h"

class Fluid
{
    public:
    unsigned int x_size, y_size, z_size;
    VectorField* u;
    ScalarField* p;
    void InitialiseVortex();
    void PressureSpiral(double t);
    void FiniteDifference();
    void PrintFluid();
    Fluid(unsigned int x_dim, unsigned int y_dim, unsigned int z_dim, double k_visc);
    ~Fluid();

    private:
    double k_visc;
    double g;
    double p_atm;
    double density;
    double dl;
    double dt;
    double t_elapsed;
};

#endif