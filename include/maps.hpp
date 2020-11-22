#ifndef MAPS_H
#define MAPS_H

#include <cmath>
#include <iostream>
#include <string>

class Maps_2d
{
    private:
        std::string name = "map_base_class";
    public:
        bool mod = true;
        double out[2], in[2]; 
        virtual std::string check_id(){ return name; }
        virtual int evolve(){ return 0; };
};

class Hal_map : public Maps_2d
{
    private:
        std::string name = "halley_map";
    public:
        double par[1];
        std::string check_id(){ return name; }
        int evolve();
};

class Hen_map : public Maps_2d
{
    private:
        std::string name = "henon_map";
    public:
        double par[2];
        std::string check_id(){ return name; }
        int evolve();
};

class Std_map : public Maps_2d
{
    private:
        std::string name = "standard_map";
    public:
        double par[1];
        std::string check_id(){ return name; }
        int evolve();
};

class Std_ntwist_map : public Maps_2d
{
    private:
        std::string name = "std_nontwist_map";
    public:
        double par[3];
        std::string check_id(){ return name; }
        int evolve();
};

class Sfum_map : public Maps_2d
{
    private:
        std::string name = "sfum_map";
    public:
        double par[1];
        std::string check_id(){ return name; }
        int evolve();
};


class Maps_1d
{
    private:
        std::string name = "map_base_class";
    public:
        double *par;
        double par_min;
        double par_max;
        double out, in;
        virtual std::string check_id() { return name; }
        virtual int evolve() { return 0; }
        virtual int set_par(double) { return 0; }
        virtual double get_par() { return par[0]; }
        virtual double get_par_max() { return par_max; }
        virtual double get_par_min() { return par_min; }
};

class Log_map : public Maps_1d
{
    private:
        std::string name = "logistic_map";
    public:
        double par[1];
        double par_min = 0.0;
        double par_max = 4.0;
        std::string check_id() { return name; }
        int evolve();
        int set_par(double new_par);
        double get_par() { return par[0]; }
        double get_par_max() { return par_max; }
        double get_par_min() { return par_min; }
};

class Log_map_2ndO : public Maps_1d
{
    private:
        std::string name = "logistic_map_2ndO";
    public:
        double par[1];
        double par_min = 0.0;
        double par_max = 4.0;
        std::string check_id() { return name; }
        int evolve();
        int set_par(double new_par);
        double get_par() { return par[0]; }
        double get_par_max() { return par_max; }
        double get_par_min() { return par_min; }
};

class Moran_map : public Maps_1d
{
    private:
        std::string name = "moran_map";
    public:
        double par[1];
        double par_min = 0.0;
        double par_max = 10.0; // can be +inf
        std::string check_id() { return name; }
        int evolve();
        int set_par(double new_par);
        double get_par() { return par[0]; }
        double get_par_max() { return par_max; }
        double get_par_min() { return par_min; }
};

class Triang_map : public Maps_1d
{
    private:
        std::string name = "triangle_map";
    public:
        double par[1];
        double par_min = 0.0;
        double par_max = 2.0;
        std::string check_id() { return name; }
        int evolve(); 
        int set_par(double new_par);
        double get_par() { return par[0]; }
        double get_par_max() { return par_max; }
        double get_par_min() { return par_min; }
};

class Lin_sin_map : public Maps_1d
{
    private:
        std::string name = "linear_sine_map";
    public:
        double par[1];
        double par_min = 0.0;
        double par_max = 100.0;
        std::string check_id() { return name; }
        int evolve();
        int set_par(double new_par);
        double get_par() { return par[0]; }
        double get_par_max() { return par_max; }
        double get_par_min() { return par_min; }
};

#endif