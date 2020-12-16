#include "maps.hpp"

int Hal_map::evolve()
{
    /**
     * Halley map (x,y) (I. I. Shevchenko, New Astronomy (2011))
     * y(n+1) = y(n) + sin(x(n))
     * x(n+1) = x(n) + lambda * |y(n+1)|^(-3/2)
     */
    out[1] = in[1] + sin(in[0]);
    out[0] = fmod(in[0] + par[0] * pow(fabs(out[1]), -1.5), 2.*M_PI);
    
    if (mod)
    {
        if (out[0] < 0.0)
        {
            out[0] += 2.0 * M_PI;
        }
    }

    return 0;
}

int Hen_map::evolve()
{
    /** 
    * Henon Map (x,y) (Hénon, Commun. Math. Phys (1976))
    * x(n+1) = a - x(n)^2 + b y(n)
    * y(n+1) = x(n)
    * 
    * Parameters: [par[0] = a] [par[1] = b
    */
    out[0] = par[0] - in[0] * in[0] + par[1] * in[1];
    out[1] = in[0];
    return 0;
}

int Std_map::evolve()
{
    /** 
    * Standard map (p,T)
    * p(n+1) = p(n) + K sin(T(n))  (mod 1)
    * T(n+1) = T(n) + p(n+1)       (mod 1)
    */
    out[0] = in[0] + in[1] + (par[0] / (2. * M_PI)) * sin(2. * M_PI * in[0]);
	out[1] = in[1] + (par[0] / (2. * M_PI)) * sin(2. * M_PI * in[0]);

    if (mod)
    {
        out[0] = (out[0] > 0.0) ? fmod(out[0], 1.) : fmod(out[0], 1.) + 1.;
        out[1] = fmod(out[1], 1.);

        if (out[1] > 0.5)
        {
            out[1] -= 1.0;
        }
        else if (out[1] < -0.5)
        {
            out[1] += 1.0;
        }
    }
    return 0;
}

int Std_ntwist_map::evolve()
{
    /**
    * Standard Nontwist map (x,y) (Castillo-Negrete, Phys. D (1996))
    * x(n+1) = x(n) + a (1 - y(n+1)^p)
    * y(n+1) = y(n) - b sin(2 pi x(n))
    * 
    * Parameters: [par[0] = a] [par[1] = b] [par[2] = p>1]
    * Domain: x -> (-0.5, 0.5) mod 1 || y -> (-inf, +inf)
    */
    out[1] = in[1] - par[1] * sin(2 * M_PI * in[0]);
    out[0] = in[0] + par[0] * (1 - pow(out[1], par[2]));
    
    if (mod)
    {
        out[0] = (out[0] > 0.0) ? fmod(out[0], 1.0) : fmod(out[0], 1.0) + 1.0;

        if (out[0] > 0.5)
        {
            out[0] -= 1.0;
        }
        else if (out[0] < -0.5)
        {
            out[0] += 1.0;
        }
    }

    return 0;
}

int Sfum_map::evolve()
{
    /** 
    * Simplified Fermi-Ulam map (sfum) (v,phi)
    * v(n+1) = |v(n) - 2eps sin(phi(n))|
    * phi(n+1) = phi(n) + 2/v(n+1)       (mod 2pi)
    */
    out[1] = fabs(in[1] - (2. * par[0]) * sin(in[0]));
	out[0] = fmod(in[0] + 2. / out[1], 2.*M_PI);
    
    if (mod)
    {
        if (out[0] < 0.0)
        {
            out[0] += 2.0 * M_PI;
        }
    }
    return 0;
}


int Log_map::evolve()
{
    /**
     * Logistic map (x) (May, Nature (1976))
     * x(n+1) = a x(n) (1 - x(n))
     * 
     * Parameters: [par[0] = a]
     * Domain: x -> (0.0, 1.0) 
     *         a -> [1, 4] (for non-trivial dynamic behavior)
     **/
    for (int i = 1; i <= iter_order; i += 1)
    {
        out = par[0] * in * (1.0 - in);
        if (i != iter_order) { in = out; }
    }

    return 0;
}

int Log_map_2ndO::evolve()
{
    /**
     * Logistic map (x) -- 2nd iteration (May, Nature (1976))
     * x(n+2) = a x(n+1) (1 - x(n+1))
     *        = a (a x(n) (1 - x(n))) (1 - a x(n) (1 - x(n)))
     * 
     * Parameters: [par[0] = a]
     * Domain: x -> (0.0, 1.0) 
     *         a -> [1, 4] (for non-trivial dynamic behavior)
     **/
    for (int i = 1; i <= iter_order; i += 1)
    {
        out = par[0] * (par[0] * in * (1.0 - in)) * (1.0 - par[0] * in * (1.0 - in));
        if (i != iter_order) { in = out; }
    }
    
    return 0;
}

int Moran_map::evolve()
{
    /**
     * Moran map (x) (May, Nature (1976))
     * x(n+1) = x(n) * exp(r * (1 - x))
     * 
     * Parmeters: [par[0] = r]
     * Domain: x -> (0.0, 1.0)
     *         r -> [0, inf]
     **/
    for (int i = 1; i <= iter_order; i += 1)
    {
        out = in * exp(par[0] * (1.0 - in));
        if (i != iter_order) { in = out; }
    }

    return 0;
}

int Triang_map::evolve()
{
    /**
     * Triangle map (x) (May, Nature (1976))
     * x(n+1) = a * x(n)       if (x < 1/2)
     *          a * (1-x(n))   if (x > 1/2)
     *
     * Parameters: [par[0] = a]
     * Domain: x -> (0.0, 1.0)
     *         a -> [0, 2.0]
     **/
    for (int i = 1; i <= iter_order; i += 1)
    {
        out = (in < 0.5) ? par[0] * in : par[0] * (1.0 - in);
        if (i != iter_order) { in = out; }
    }
    
    return 0;
}

int Lin_sin_map::evolve()
{
    /** 
     * Linear sine map (x)
     * x(n+1) = 0.5 (x(n) + sin(w * x(n))
     * 
     * Parameters: [par[0] = w]
     * Domain: x -> (0.0, 1.0)
     *         w -> [0, inf]
     **/
    for (int i = 1; i <= iter_order; i += 1)
    {
        out = in + 0.1 * sin(par[0] * in);
        if (i != iter_order) { in = out; }
    }

    return 0;
}


int Log_map::set_par(double new_par)
{
    par[0] = new_par;
    return 0;
}

int Log_map_2ndO::set_par(double new_par)
{
    par[0] = new_par;
    return 0;
}

int Moran_map::set_par(double new_par)
{
    par[0] = new_par;
    return 0;
}

int Triang_map::set_par(double new_par)
{
    par[0] = new_par;
    return 0;
}

int Lin_sin_map::set_par(double new_par)
{
    par[0] = new_par;
    return 0;
}