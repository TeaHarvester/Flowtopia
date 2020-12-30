#include"rigidbody.h"

RigidBody::RigidBody(Vector3<double> cntr, double rad)
: 
VectorField(2*rad + 1, 2*rad + 1, 2*rad + 1),
radius(rad),
centre(cntr)
{
    unsigned int length = 2*rad + 1;

    for (unsigned int i = 0; i < length; ++i)
    {
        for (unsigned int j = 0; j < length; ++j)
        {
            for (unsigned int k = 0; k < length; ++k)
            {
                double x = (double)i - (double)radius;
                double y = (double)j - (double)radius;
                double z = (double)k - (double)radius;
                double magnitude = Vector3<double>(x, y, z).Magnitude();

                if (pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0) < pow(double(radius), 2.0) + 0.5 &&
                    pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0) >= pow(double(radius), 2.0) - 0.5)
                {
                    (*map[i + (j*length) + (k*length*length)])(0) = x/magnitude;
                    (*map[i + (j*length) + (k*length*length)])(1) = y/magnitude;
                    (*map[i + (j*length) + (k*length*length)])(2) = z/magnitude;
                }

                else if (pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0) < pow(double(radius), 2.0) - 1)
                {
                    (*map[i + (j*length) + (k*length*length)])(0) = 0.0;
                    (*map[i + (j*length) + (k*length*length)])(1) = 0.0;
                    (*map[i + (j*length) + (k*length*length)])(2) = 0.0;
                }

                else 
                {
                    map[i + (j*length) + (k*length*length)] = NULL;
                }
            }
        }
    }
}