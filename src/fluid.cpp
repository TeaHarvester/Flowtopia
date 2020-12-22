#include<iostream>
#include<iomanip>
#include"math.h"
#include"fluid.h"

void Fluid::InitialiseVortex()
{
    double param = 1/(double)x_size;

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
                    (*(*u)(i, j, k)) *= pow(k, 0.5);
                    double magnitude = (*u)(i, j, k)->Magnitude();

                    if (magnitude > u->norm_coeff)
                    {
                        u->norm_coeff = magnitude;
                    }
                }
            }
        }
    }
}

void Fluid::PressureSpiral(double t)
{
    // generate oscillating pressure wave at z = 0 to simulate propeller
    double pi = 3.1415926535;
    unsigned int x = (int)((double(x_size)*((cos(t*pi/48.0)/2.0) + 2.0)/4.0));
    unsigned int y = (int)((double(y_size)*((sin(t*pi/48.0)/2.0) + 2.0)/4.0));

    for (unsigned int i = 0; i < x_size; ++i)
    {
        for (unsigned int j = 0; j < y_size; ++j)
        {
            if (i == x && j == y)
            {
                (*p)(i, j, 1) = 900.0;
            }

            else
            {
                (*p)(i, j, 1) = g*(z_size - 1.0)*dl*density;
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

    //PressureSpiral(t_elapsed);

    const double a = dt/dl;
    double& d = density;

    VectorField* new_u = (new VectorField(*u));

    for (unsigned int i = 0; i < x_size; ++i)
    {
        for (unsigned int j = 0; j < y_size; ++j)
        {
            for (unsigned int k = 0; k < z_size; ++k)
            {
                // pressure
                double& p_0 = (*p)(i, j, k);

                // initial flow velocity components
                double& u_0 = (*(*u)(i, j, k))(0);
                double& v_0 = (*(*u)(i, j, k))(1);
                double& w_0 = (*(*u)(i, j, k))(2);

                // flow direction components
                // int u_sign = u_0 < 0 ? -1 : 1;
                // int v_sign = v_0 < 0 ? -1 : 1;
                // int w_sign = w_0 < 0 ? -1 : 1;

                int u_sign = 1;
                int v_sign = 1;
                int w_sign = 1;

                if (i != 0 && i != x_size - 1
                    && j != 0 && j != y_size - 1
                    && k != 0 && k != z_size - 1)
                {                 
                    // new x velocity component 
                    double& u_1 = (*(*new_u)(i, j, k))(0);

                    // x pressure gradient
                    double& px_a = (*p)(i+u_sign, j, k);

                    // shear terms
                    double& u_a = (*(*u)(i+u_sign, j, k))(0);
                    double& a_u = (*(*u)(i-u_sign, j, k))(0);
                    double& u_b = (*(*u)(i, j+v_sign, k))(0);
                    double& b_u = (*(*u)(i, j-v_sign, k))(0);
                    double& u_c = (*(*u)(i, j, k+w_sign))(0);
                    double& c_u = (*(*u)(i, j, k-w_sign))(0);

                    u_1 += -a*(u_sign*u_0*(u_a - u_0) +
                            v_sign*v_0*(u_b - u_0) +
                            w_sign*w_0*(u_c - u_0) + 
                            u_sign*(px_a - p_0)/d) + 
                    (pow(a,2.0)/dt)*k_visc*(
                            u_sign*(u_a - 2*u_0 + a_u) + 
                            v_sign*(u_b - 2*u_0 + b_u) + 
                            w_sign*(u_c - 2*u_0 + c_u));

                    // new y velocity component
                    double& v_1 = (*(*new_u)(i, j, k))(1);

                    // y pressure gradient
                    double& py_a = (*p)(i, j+v_sign, k);

                    // stress and shear velocity terms
                    double& v_a = (*(*u)(i+u_sign, j, k))(1);
                    double& a_v = (*(*u)(i-u_sign, j, k))(1);
                    double& v_b = (*(*u)(i, j+v_sign, k))(1);
                    double& b_v = (*(*u)(i, j-v_sign, k))(1);
                    double& v_c = (*(*u)(i, j, k+w_sign))(1);
                    double& c_v = (*(*u)(i, j, k-w_sign))(1);

                    v_1 += -a*(u_sign*u_0*(v_a - v_0) +
                            v_sign*v_0*(v_b - v_0) +
                            w_sign*w_0*(v_c - v_0) + 
                            v_sign*(py_a - p_0)/d) + 
                    (pow(a,2.0)/dt)*k_visc*(
                            u_sign*(v_a - 2*v_0 + a_v) + 
                            v_sign*(v_b - 2*v_0 + b_v) + 
                            w_sign*(v_c - 2*v_0 + c_v));

                    // new z velocity component;
                    double& w_1 = (*(*new_u)(i, j, k))(2);

                    // z pressure gradient
                    double& pz_a = (*p)(i, j, k+w_sign);

                    // stress and shear velocity terms
                    double& w_a = (*(*u)(i+u_sign, j, k))(2);
                    double& a_w = (*(*u)(i-u_sign, j, k))(2);
                    double& w_b = (*(*u)(i, j+v_sign, k))(2);
                    double& b_w = (*(*u)(i, j-v_sign, k))(2);
                    double& w_c = (*(*u)(i, j, k+w_sign))(2);
                    double& c_w = (*(*u)(i, j, k-w_sign))(2);

                    w_1 += -a*(u_sign*u_0*(w_a - w_0) +
                            v_sign*v_0*(w_b - w_0) +
                            w_sign*w_0*(w_c - w_0) + 
                            w_sign*(pz_a - p_0)/d) + 
                    (pow(a,2.0)/dt)*k_visc*(
                            u_sign*(w_a - 2*w_0 + a_w) + 
                            v_sign*(w_b - 2*w_0 + b_w) + 
                            w_sign*(w_c - 2*w_0 + c_w)) -
                            g*dt;
                }

                double magnitude = (*new_u)(i, j, k)->Magnitude();

                if (magnitude > new_u->norm_coeff)
                {
                    new_u->norm_coeff = magnitude;
                }
            }
        }
    }

    delete u;
    u = new_u;
    t_elapsed += 1;
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
k_visc(k_v),
g(9.81),
p_atm(101325),
density(1000),
dl(0.01),
dt(0.001),
t_elapsed(0)
{
    u = new VectorField(x_size, y_size, z_size);
    p = new ScalarField(x_size, y_size, z_size);

    // hydrostatics
    for (unsigned int i = 0; i < x_size; ++i)
    {
        for (unsigned int j = 0; j < y_size; ++j)
        {
            for (unsigned int k = 0; k < z_size; ++k)
            {     
                (*p)(i, j, k) = g*(z_size - k)*dl*density;
            }
        }
    }
}

Fluid::~Fluid()
{
    delete u;
    delete p;
}