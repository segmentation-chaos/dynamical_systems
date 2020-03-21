#ifndef MAPS_H
#define MAPS_H

#include <cmath>
#include <iostream>
#include <string>

using namespace std;

class Maps_2d
{
    private:
        string name = "map_base_class";
    public:
        bool mod = true;
        double out[2], in[2]; 
        virtual string check_id(){ return name; }
        virtual int evolve(){ return 0; };
};

class Hal_map : public Maps_2d
{
    private:
        string name = "halley_map";
    public:
        double par[1];
        string check_id(){ return name; }
        int evolve();
};

class Hen_map : public Maps_2d
{
    private:
        string name = "henon_map";
    public:
        double par[2];
        string check_id(){ return name; }
        int evolve();
};

class Std_map : public Maps_2d
{
    private:
        string name = "standard_map";
    public:
        double par[1];
        string check_id(){ return name; }
        int evolve();
};

class Std_ntwist_map : public Maps_2d
{
    private:
        string name = "std_nontwist_map";
    public:
        double par[3];
        string check_id(){ return name; }
        int evolve();
};

class Sfum_map : public Maps_2d
{
    private:
        string name = "sfum_map";
    public:
        double par[1];
        string check_id(){ return name; }
        int evolve();
};


class Maps_1d
{
    private:
        string name = "map_base_class";
    public:
        double *par;
        double par_min;
        double par_max;
        double out, in;
        virtual string check_id() { return name; }
        virtual int evolve() { return 0; }
        virtual int set_par(double) { return 0; }
        virtual double get_par() { return par[0]; }
        virtual double get_par_max() { return par_max; }
        virtual double get_par_min() { return par_min; }
};

class Log_map : public Maps_1d
{
    private:
        string name = "logistic_map";
    public:
        double par[1];
        double par_min = 0.0;
        double par_max = 4.0;
        string check_id() { return name; }
        int evolve();
        int set_par(double new_par);
        double get_par() { return par[0]; }
        double get_par_max() { return par_max; }
        double get_par_min() { return par_min; }
};

class Log_map_2ndO : public Maps_1d
{
    private:
        string name = "logistic_map_2ndO";
    public:
        double par[1];
        double par_min = 0.0;
        double par_max = 4.0;
        string check_id() { return name; }
        int evolve();
        int set_par(double new_par);
        double get_par() { return par[0]; }
        double get_par_max() { return par_max; }
        double get_par_min() { return par_min; }
};

class Moran_map : public Maps_1d
{
    private:
        string name = "moran_map";
    public:
        double par[1];
        double par_min = 0.0;
        double par_max = 10.0; // can be +inf
        string check_id() { return name; }
        int evolve();
        int set_par(double new_par);
        double get_par() { return par[0]; }
        double get_par_max() { return par_max; }
        double get_par_min() { return par_min; }
};

class Triang_map : public Maps_1d
{
    private:
        string name = "triangle_map";
    public:
        double par[1];
        double par_min = 0.0;
        double par_max = 2.0;
        string check_id() { return name; }
        int evolve(); 
        int set_par(double new_par);
        double get_par() { return par[0]; }
        double get_par_max() { return par_max; }
        double get_par_min() { return par_min; }
};

class Lin_sin_map : public Maps_1d
{
    private:
        string name = "linear_sine_map";
    public:
        double par[1];
        double par_min = 0.0;
        double par_max = 100.0;
        string check_id() { return name; }
        int evolve();
        int set_par(double new_par);
        double get_par() { return par[0]; }
        double get_par_max() { return par_max; }
        double get_par_min() { return par_min; }
};

#endif