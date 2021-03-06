#include"field.h"

Vector3<double>*& VectorField::operator() (unsigned int x, unsigned int y, unsigned int z)
{
    return map[x + (y*x_size) + (z*x_size*y_size)];
}

VectorField::VectorField(unsigned int x_dim, unsigned int y_dim, unsigned int z_dim)
:
x_size(x_dim),
y_size(y_dim),
z_size(z_dim),
norm_coeff(0.0)
{
    map = new Vector3<double>*[x_size * y_size * z_size];

    for (unsigned int i = 0; i < x_size * y_size * z_size; ++i)
    {
        map[i] = new Vector3<double>();
    }
}

VectorField::VectorField(const VectorField& copy)
:
x_size(copy.x_size),
y_size(copy.y_size),
z_size(copy.z_size),
norm_coeff(copy.norm_coeff)
{
    map = new Vector3<double>*[x_size*y_size*z_size];

    for (unsigned int i = 0; i < x_size * y_size * z_size; ++i)
    {
        map[i] = new Vector3<double>(*copy.map[i]);
    }

}

VectorField::~VectorField()
{
    for (unsigned int i = 0; i < x_size*y_size*z_size; ++i)
    {
        delete map[i];
    }

    delete map;
}

double& ScalarField::operator() (unsigned int x, unsigned int y, unsigned int z)
{
    return map[x + (y*x_size) + (z*x_size*y_size)];
}

ScalarField::ScalarField(unsigned int x_dim, unsigned int y_dim, unsigned int z_dim)
:
x_size(x_dim),
y_size(y_dim),
z_size(z_dim)
{
    map = new double[x_dim * y_dim * z_dim];

    for (unsigned int i = 0; i < x_dim * y_dim * z_dim; ++i)
    {
        map[i] = 0;
    }
}

ScalarField::~ScalarField()
{
    delete map;
}