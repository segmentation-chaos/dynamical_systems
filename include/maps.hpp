#ifndef MAPS_H
#define MAPS_H

class Maps_2d
{
    public:
        double out[2], in[2];
        virtual int check_id(){return 0;}
        virtual int evolve(){return 0;};
};

class Std_map : public Maps_2d
{
    public:
        double par;
        int check_id(){return 1;}
        int evolve();
};

class Hen_map : public Maps_2d
{
    public:
        double par[2];
        int check_id(){return 2;}
        int evolve();
};

class Hal_map : public Maps_2d
{
    public:
        double par;
        int check_id(){return 10;}
        int evolve();
};

#endif