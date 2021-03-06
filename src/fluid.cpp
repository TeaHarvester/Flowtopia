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

void Fluid::InitialiseFlow(Vector3<double> flow_velocity)
{
    // turbulence
    srand(3849302);

    for (unsigned int j = 1; j < y_size - 1; ++j)
    {
        for (unsigned int k = 1; k < z_size - 1; ++k)
        {
            (*(*u)(0, j, k))(0) = flow_velocity(0) + (((rand() % 100) - 50) / 1000);
            (*(*u)(0, j, k))(1) = flow_velocity(1) + (((rand() % 100) - 50) / 1000); 
            (*(*u)(0, j, k))(2) = flow_velocity(2) + (((rand() % 100) - 50) / 1000);
            // (*(*u)(x_size - 1, j, k))(0) = flow_velocity(0);
            // (*(*u)(x_size - 1, j, k))(1) = flow_velocity(1); 
            // (*(*u)(x_size - 1, j, k))(2) = flow_velocity(2);

            for (unsigned int i = 1; i < x_size - 1; ++i)
            {
                (*(*u)(i, j, k))(0) = (flow_velocity(0) + (((rand() % 100) - 50) / 1000))/10.0;
                (*(*u)(i, j, k))(1) = (flow_velocity(1) + (((rand() % 100) - 50) / 1000))/10.0; 
                (*(*u)(i, j, k))(2) = (flow_velocity(2) + (((rand() % 100) - 50) / 1000))/10.0;
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

    // boundary condition: u_inf = u_inf-1
    for (unsigned int j = 0; j < y_size; ++j)
    {
        for (unsigned int k = 0; k < z_size; ++k)
        {
            (*(*new_u)(x_size - 1, j, k))(0) = (*(*new_u)(x_size - 2, j, k))(0);
            (*(*new_u)(x_size - 1, j, k))(1) = (*(*new_u)(x_size - 2, j, k))(1);
            (*(*new_u)(x_size - 1, j, k))(2) = (*(*new_u)(x_size - 2, j, k))(2);
        }
    }

    for (unsigned int i = 0; i < x_size; ++i)
    {
        for (unsigned int j = 0; j < y_size; ++j)
        {
            for (unsigned int k = 0; k < z_size; ++k)
            {
                // manage rigidbody boundary
                Vector3<double>* normal = NULL;

                if (object != NULL &&
                    i <= object->centre(0) + object->radius && i >= object->centre(0) - object->radius &&
                    j <= object->centre(1) + object->radius && j >= object->centre(1) - object->radius &&
                    k <= object->centre(2) + object->radius && k >= object->centre(2) - object->radius)
                
                {
                    // coordinates in object-space
                    unsigned int obj_i = i - (object->centre(0) - object->radius);
                    unsigned int obj_j = j - (object->centre(1) - object->radius);
                    unsigned int obj_k = k - (object->centre(2) - object->radius);

                    if ((*object)(obj_i, obj_j, obj_k) == NULL)
                    {
                    }

                    else if ((*(*object)(obj_i, obj_j, obj_k))(0) == 0.0 &&
                        (*(*object)(obj_i, obj_j, obj_k))(1) == 0.0 &&
                        (*(*object)(obj_i, obj_j, obj_k))(2) == 0.0)
                        {
                            (*(*new_u)(i, j, k))(0) = 0.0;
                            (*(*new_u)(i, j, k))(1) = 0.0;
                            (*(*new_u)(i, j, k))(2) = 0.0;
                            continue;
                        }

                    else 
                    {
                        normal = (*object)(obj_i, obj_j, obj_k);
                    }
                }

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

                int u_sign = -1;
                int v_sign = -1;
                int w_sign = -1;

                if (i != 0 && i != x_size - 1
                    && j != 0 && j != y_size - 1
                    && k != 0 && k != z_size - 1)
                {               

                    // new x velocity components 
                    double& u_1 = (*(*new_u)(i, j, k))(0);
                    double& v_1 = (*(*new_u)(i, j, k))(1);
                    double& w_1 = (*(*new_u)(i, j, k))(2);

                    // x pressure gradients
                    double& px_a = (*p)(i+u_sign, j, k);
                    double& py_a = (*p)(i, j+v_sign, k);
                    double& pz_a = (*p)(i, j, k+w_sign);

                    // local velocity profile
                    double& u_a = (*(*u)(i+u_sign, j, k))(0);
                    double& a_u = (*(*u)(i-u_sign, j, k))(0);
                    double& u_b = (*(*u)(i, j+v_sign, k))(0);
                    double& b_u = (*(*u)(i, j-v_sign, k))(0);
                    double& u_c = (*(*u)(i, j, k+w_sign))(0);
                    double& c_u = (*(*u)(i, j, k-w_sign))(0);

                    double& v_a = (*(*u)(i+u_sign, j, k))(1);
                    double& a_v = (*(*u)(i-u_sign, j, k))(1);
                    double& v_b = (*(*u)(i, j+v_sign, k))(1);
                    double& b_v = (*(*u)(i, j-v_sign, k))(1);
                    double& v_c = (*(*u)(i, j, k+w_sign))(1);
                    double& c_v = (*(*u)(i, j, k-w_sign))(1);

                    double& w_a = (*(*u)(i+u_sign, j, k))(2);
                    double& a_w = (*(*u)(i-u_sign, j, k))(2);
                    double& w_b = (*(*u)(i, j+v_sign, k))(2);
                    double& b_w = (*(*u)(i, j-v_sign, k))(2);
                    double& w_c = (*(*u)(i, j, k+w_sign))(2);
                    double& c_w = (*(*u)(i, j, k-w_sign))(2);

                    u_1 += -a*(u_sign*u_a*(u_a - u_0) +
                            v_sign*v_0*(u_b - u_0) +
                            w_sign*w_0*(u_c - u_0) + 
                            u_sign*(px_a - p_0)/d) + 
                    (pow(a,2.0)/dt)*k_visc*(
                            (u_a - 2*u_0 + a_u) + 
                            (u_b - 2*u_0 + b_u) + 
                            (u_c - 2*u_0 + c_u));

                    v_1 += -a*(u_sign*u_0*(v_a - v_0) +
                            v_sign*v_0*(v_b - v_0) +
                            w_sign*w_0*(v_c - v_0) + 
                            v_sign*(py_a - p_0)/d) + 
                    (pow(a,2.0)/dt)*k_visc*(
                            (v_a - 2*v_0 + a_v) + 
                            (v_b - 2*v_0 + b_v) + 
                            (v_c - 2*v_0 + c_v));

                    w_1 += -a*(u_sign*u_0*(w_a - w_0) +
                            v_sign*v_0*(w_b - w_0) +
                            w_sign*w_0*(w_c - w_0)) + 
                           // w_sign*(pz_a - p_0)/d) + 
                    (pow(a,2.0)/dt)*k_visc*(
                            (w_a - 2*w_0 + a_w) + 
                            (w_b - 2*w_0 + b_w) + 
                            (w_c - 2*w_0 + c_w)) //-
                           // g*dt
                           ; 
                }

                double magnitude = (*new_u)(i, j, k)->Magnitude();

                // solve object boundary flow
                if (normal != NULL)
                {
                    Vector3<double> tangent_flow = (*new_u)(i, j, k)->Cross(*normal);
                    Vector3<double> boundary_flow = normal->Cross(tangent_flow);

                    if (boundary_flow.Magnitude() != 0)
                    {
                        boundary_flow *= magnitude/boundary_flow.Magnitude();
                    }

                    *(*new_u)(i, j, k) = boundary_flow;
                }

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
                std::cout << (*(*u)(i, j, k))(0) << " ";
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
object(NULL),
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

    if (object != NULL)
    {
        delete object;
    }
}