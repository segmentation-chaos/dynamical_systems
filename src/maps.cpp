#include "maps.hpp"
#include <math.h>
#include <iostream>

int Std_map::evolve()
{
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
    out[0] = par[0] - in[0] * in[0] + par[1] * in[1];
    out[1] = in[0];
    return 0;
}

int Hal_map::evolve()
{
    /**
     * Halley map as Eq. 36 of Shevchenko (2011)
     * 
     * y_{n+1} = y_{n} + sin x_{n}
     * x_{n+1} = x_{n} + \lambda * |y_{n+1}|^{-3/2}
     * 
     */
    out[1] = in[1] + sin(in[0]);
    out[0] = fmod(in[0] + par * pow(fabs(out[1]), -1.5), 2.*M_PI);
    if (out[0] < 0.0)
    {
        out[0] += 2.0 * M_PI;
    }
    return 0;
}