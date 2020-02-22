#ifndef SYSTEM_H
#define SYSTEM_H

#include "canvas.hpp"
#include "features.hpp"

class System
{
    private:
        Maps_2d *map;
        Analysis *analy;
        Canvas canvas;
    
    public:
        System(unsigned int scrWidth, unsigned int scrHeight,
                Maps_2d *map_type, Analysis anal_type);

        void setMap(Maps_2d *map_in);
        // void setAnalysis(Analysis anal_in); -- arrumar

        int run();
};

#endif