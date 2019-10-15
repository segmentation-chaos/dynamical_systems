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

class Std_map : public Maps_2d
{
    private:
        string name = "standard_map";
    public:
        double par;
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

class Ntwst_map : public Maps_2d
{
    private:
        string name = "nontwist_std_map";
    public:
        double par[3];
        string check_id(){ return name; }
        int evolve();
};

#endif