#include "maps.hpp"

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

int Ntwst_map::evolve()
{
    /**
    * Standard Nontwist map (x,y) (Castillo-Negrete Phys. D(1996))
    * x(n+1) = x(n) + a (1 - y(n+1)^p)
    * y(n+1) = y(n) - b sin(2 pi x(n))
    * 
    * Parameters: [par[0] = a] [par[1] = b] [par[2] = p>1]
    * Domain: x -> (-0.5, 0.5) mod 1 || y -> (-inf, +inf)
    */
    out[1] = in[1] - par[1] * sin(2 * M_PI * in[0]);
    out[0] = in[0] + par[0] * (1 - pow(out[1], par[2]));
    out[0] = (out[0] > 0.0) ? fmod(out[0], 1.0) : fmod(out[0], 1.0) + 1.0;
    if (out[0] > 0.5)
    {
        out[0] -= 1.0;
    }
    else if (out[0] < -0.5)
    {
        out[0] += 1.0;
    }
    return 0;
}