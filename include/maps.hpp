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

#endif