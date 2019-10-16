#ifndef FEATURES_H
#define FEATURES_H

#include <typeinfo>
#include <sys/stat.h>

#include "maps.hpp"

class Analysis
{
    private:
        int make_dir(string, string);
        int make_file(FILE**, FILE**, string, string);
    public:
        int iter_num;
        double x0, y0;
        double x_min, x_max, y_min, y_max, num_x, num_y;
        int orbit_2d(Maps_2d*);
        int phase_space_2d(Maps_2d*);
};

#endif