#include<iostream>
#include"fluid.h"

int main() 
{
    Fluid f(8, 8, 8, 0.000001);
    f.FiniteDifference();
    f.FiniteDifference();
    f.FiniteDifference();
    f.FiniteDifference();
    f.FiniteDifference();

    std::cout << "hellow fluid" << std::endl;

    return 0;
}

