#ifndef FEATURES_H
#define FEATURES_H

#include <typeinfo>
#include <sys/stat.h>
#include <vector>

#include "maps.hpp"

class Analysis
{
    private:
        int make_dir(std::string, std::string);
        int make_file(FILE**, FILE**, std::string, std::string);
        
    public:
        int iter_num;
        double x0, y0;
        double x_min, x_max, y_min, y_max, num_x, num_y;
        double wind_x0, wind_y0, wind_xf, wind_yf, pts_num;
        int orbit_1d(Maps_1d*);
        int orbit_2d(Maps_2d*);
        int phase_space_2d(Maps_2d*);
        int winding_number(Maps_2d*);

        int save_orbit(Maps_2d *map, 
                       std::vector<std::vector<double>> orbit_pts, 
                       std::vector<std::vector<double>> orbit_ics);
};

#endif