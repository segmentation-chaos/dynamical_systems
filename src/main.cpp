#include <iostream>
#include <cstdlib>
#include <cmath>
#include "maps.hpp"
#include "features.hpp"

int main(int argc, char **argv)
{
	Maps_2d *map_hal;
	Hal_map hal;
	map_hal = &hal;

	Analysis ps, orb;

	hal.par = 20.;

	ps.x_min = 0.0;
	ps.x_max = 2.0 * M_PI;
	ps.y_min = 2.0;
	ps.y_max = 6.0;
	ps.num_x = 10;
	ps.num_y = 10;
	ps.phase_space_2d(map_hal);

	orb.x0 = 3.03366;
	orb.y0 = 2.52012;
	orb.iter_num = 1e5;
	orb.orbit_2d(map_hal);

	/*Maps_2d *map_1, *map_2;
	
	Std_map std;
	Hen_map hen;
	map_1 = &std;
	map_2 = &hen;

	std.par = 1.5;
	hen.par[0] = 1.4;
	hen.par[1] = 0.3;

	Analysis orb_1, orb_2;

	orb_1.x0 = 0.9;
	orb_1.y0 = 0.0;
	orb_1.iter_num = 1e5;
	orb_1.orbit_2d(map_1);

	orb_2.x0 = 0.0;
	orb_2.y0 = 0.0;
	orb_2.iter_num = 1e5;
	orb_2.orbit_2d(map_2);

	std.par = 1.5;
	hen.par[0] = 0.3;
	hen.par[1] = 1.0;

	Analysis ps_1, ps_2;

	ps_1.x_min = 0.0;
	ps_1.x_max = 1.0;
	ps_1.y_min = -0.5;
	ps_1.y_max = 0.5;
	ps_1.num_x = 10;
	ps_1.num_y = 10;
	ps_1.phase_space_2d(map_1);

	ps_2.x_min = -1.2;
	ps_2.x_max = 1.2;
	ps_2.y_min = -1.2;
	ps_2.y_max = 1.2;
	ps_2.num_x = 20;
	ps_2.num_y = 20;
	ps_2.phase_space_2d(map_2);*/

	return 0;
}
