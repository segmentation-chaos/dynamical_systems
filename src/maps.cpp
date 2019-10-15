#include "maps.hpp"
#include <math.h>
#include <iostream>

int Std_map::evolve()
{
    /** 
    * Standard map (p,T)
    * p(n+1) = p(n) + K sin(T(n))  (mod 1)
    * T(n+1) = T(n) + p(n+1)       (mod 1)
    */
    out[0] = in[0] + in[1] + (par / (2. * M_PI)) * sin(2. * M_PI * in[0]);
    out[0] = (out[0] > 0.0) ? fmod(out[0], 1.) : fmod(out[0], 1.) + 1.;
	out[1] = in[1] + (par / (2. * M_PI)) * sin(2. * M_PI * in[0]);
    out[1] = fmod(out[1], 1.);
    if (out[1] > 0.5)
    {
        out[1] -= 1.0;
    }
    else if (out[1] < -0.5)
    {
        out[1] += 1.0;
    }
    return 0;
}

int Hen_map::evolve()
{
    /** 
    * Henon Map (x,y)
    * x(n+1) = a - x(n)^2 + b y(n)
    * y(n+1) = x(n)
    */
    out[0] = par[0] - in[0] * in[0] + par[1] * in[1];
    out[1] = in[0];
    return 0;
}