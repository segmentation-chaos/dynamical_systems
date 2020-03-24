#ifndef SYSTEM_H
#define SYSTEM_H

#include "canvas.hpp"
#include "features.hpp"

class System
{
    private:
        Maps_1d *map_1d;
        Maps_2d *map;
        Analysis *analy;
        Canvas canvas;
    
    public:
        System(unsigned int scrWidth, unsigned int scrHeight);

        void setMap(Maps_2d *map_in);
        void setAnalysis(Analysis anal_in);

        void setMap_1d(Maps_1d *map_in);

        int run_map_2d();
        int run_cobweb();
        int run_bifurc_diagram();
        int sys_quit();

        int running_sys;
        int running_map_2d;
        int running_cobweb;
        int running_bifurc_diagram;
};

#endif