#include<iostream>
#include<iomanip>
#include"math.h"
#include"fluid.h"

void Fluid::InitialiseVortex()
{
    double param = 1.0/(double)x_size;

    for (unsigned int i = 0; i < x_size; ++i)
    {
        for (unsigned int j = 0; j < y_size; ++j)
        {
            for (unsigned int k = 0; k < z_size; ++k)
            {
                if (i != 0 && j != 0 && k != 0 && i != x_size-1 && j != y_size-1 && k != z_size-1)
                {
                    double param2 = 1.0/(pow((double)i - (((double)x_size - 1.0)/2.0), 2.0) +
                    pow((double)j - (((double)y_size - 1.0)/2.0),2.0) + 1.0);

                    (*(*u)(i, j, k))(0) = param*param2*((((double)y_size - 1.0)/2.0) - (double)j);
                    (*(*u)(i, j, k))(1) = param*param2*((double)i - (((double)x_size - 1.0)/2.0));
                }
            }
        }
    }
}

void Fluid::FiniteDifference()
{
    // solves the Navier-Stokes equation numerically and advances to the next timestep
    // assumes incompressible flow, uniform pressure, 0 gravity
    // just like a real fluid
    // evaluates gradients in the direction of flow

    const double t_step = 0.0001;
    const double x_step = 0.01;
    const double a = t_step/x_step;

    VectorField* new_u = (new VectorField(*u));

    for (unsigned int i = 0; i < x_size; ++i)
    {
        for (unsigned int j = 0; j < y_size; ++j)
        {
            for (unsigned int k = 0; k < z_size; ++k)
            {
                if (i != 0 && i != x_size - 1)
                {                  
                    double& u_0 = (*(*u)(i, j, k))(0);
                    int sign = u_0 < 0 ? -1 : 1;
                    double& u_1 = (*(*new_u)(i, j, k))(0);
                    double& u_a = (*(*u)(i+sign, j, k))(0);
                    double& a_u = (*(*u)(i-sign, j, k))(0);
                    u_1 += a*u_0*(u_a - u_0) + (pow(a,2.0)/t_step)*k_visc*(u_a - 2*u_0 + a_u);
                }

                if (j != 0 && j != y_size - 1)
                {
                    double& v_0 = (*(*u)(i, j, k))(1);
                    int sign = v_0 < 0 ? -1 : 1;
                    double& v_1 = (*(*new_u)(i, j, k))(1);
                    double& v_a = (*(*u)(i, j+sign, k))(1);
                    double& a_v = (*(*u)(i, j-sign, k))(1);
                    v_1 += a*v_0*(v_a - v_0) + (pow(a,2.0)/t_step)*k_visc*(v_a - 2*v_0 + a_v);
                }

                if (k != 0 && k != z_size - 1)
                {
                    double& w_0 = (*(*u)(i, j, k))(2);
                    int sign = w_0 < 0 ? -1 : 1;
                    double& w_1 = (*(*new_u)(i, j, k))(2);
                    double& w_a = (*(*u)(i, j, k+sign))(2);
                    double& a_w = (*(*u)(i, j, k-sign))(2);
                    w_1 += a*w_0*(w_a - w_0) + (pow(a,2.0)/t_step)*k_visc*(w_a - 2*w_0 + a_w); 
                }
            }
        }
    }

    delete u;
    u = new_u;
}

void Fluid::PrintFluid()
{
    std::cout << std::fixed;
    std::cout << std::setprecision(7);


    for (unsigned int k = 0; k < z_size; ++k)
    {
        for (unsigned int i = 0; i < x_size; ++i)
        {
            for (unsigned int j = 0; j < y_size; ++j)
            {
                std::cout << std::sqrt((*(*u)(i, j, k)).Dot((*(*u)(i, j, k)))) << " ";
            }

            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}

Fluid::Fluid(unsigned int x_dim, unsigned int y_dim, unsigned int z_dim, double k_v = 0.000001)
:
x_size(x_dim),
y_size(y_dim),
z_size(z_dim),
k_visc(k_v)
{
    u = new VectorField(x_size, y_size, z_size);
}

Fluid::~Fluid()
{
}