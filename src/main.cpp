#include<iostream>
#include"fluid.h"

int main() 
{
    Fluid f(10, 10, 3, 0.001);

    f.InitialiseVortex();
    f.PrintFluid();
    for (int i = 0; i < 50000; ++i)
    {
        f.FiniteDifference();
    }

    f.PrintFluid();

    std::cout << "hellow fluid" << std::endl;

    return 0;
}

