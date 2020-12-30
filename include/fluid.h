#ifndef FLUID
#define FLUID

#include<iostream>
#include"field.h"
#include"rigidbody.h"

class Fluid
{
    public:
    unsigned int x_size, y_size, z_size;
    VectorField* u;
    ScalarField* p;
    RigidBody* object;
    void InitialiseVortex();
    void InitialiseFlow(Vector3<double> flow_velocity);
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