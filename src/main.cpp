#include <iostream>
#include <cstdlib>
#include <cmath>

#include "canvas.hpp"
#include "maps.hpp"
#include "features.hpp"
#include "system.hpp"

int main(int argc, char **argv)
{
	Maps_2d *map_1, *map_2, *map_3, *map_4, *map_5;
	
	Std_map std;
	Hen_map hen;
	Std_ntwist_map ntwst;
	Hal_map hal;
	Sfum_map sfum;
	map_1 = &std;
	map_2 = &hen;
	map_3 = &ntwst;
	map_4 = &hal;
	map_5 = &sfum;

	/* Map settings */
	std.par[0] = 1.5;
	hen.par[0] = 1.4;
	hen.par[1] = 0.3;
	ntwst.par[0] = 0.615;
	ntwst.par[1] = 0.4;
	ntwst.par[2] = 2;
	hal.par[0] = 20.;
	sfum.par[0] = 0.001;

	/* Orbit settings */
	Analysis orb_1, orb_2, orb_3, orb_4, orb_5;

	orb_1.x0 = 0.9;
	orb_1.y0 = 0.0;
	orb_1.iter_num = 1e5;
	orb_1.orbit_2d(map_1);

	orb_2.x0 = 0.0;
	orb_2.y0 = 0.0;
	orb_2.iter_num = 1e5;
	orb_2.orbit_2d(map_2);

	orb_3.x0 = 0.0;
	orb_3.y0 = -0.18;
	orb_3.iter_num = 1e5;
	orb_3.orbit_2d(map_3);

	orb_4.x0 = 3.03366;
	orb_4.y0 = 2.52012;
	orb_4.iter_num = 1e5;
	orb_4.orbit_2d(map_4);

	orb_5.x0 = 3.14;
	orb_5.y0 = 0.01;
	orb_5.iter_num = 1e4;
	orb_5.orbit_2d(map_5);

	/* Map settings */
	std.par[0] = 1.5;
	hen.par[0] = 0.3;
	hen.par[1] = 1.0;

	/* Phase space settings*/
	Analysis ps_1, ps_2, ps_3, ps_4, ps_5;

	ps_1.x_min = 0.0;
	ps_1.x_max = 1.0;
	ps_1.y_min = -0.5;
	ps_1.y_max = 0.5;
	ps_1.num_x = 10;
	ps_1.num_y = 10;
	ps_1.phase_space_2d(map_1);

	ps_2.x_min = -2.0;
	ps_2.x_max = 2.0;
	ps_2.y_min = -2.0;
	ps_2.y_max = 2.0;
	ps_2.num_x = 20;
	ps_2.num_y = 20;
	ps_2.phase_space_2d(map_2);

	ps_3.x_min = -0.5;
	ps_3.x_max = 0.5;
	ps_3.y_min = -0.5;
	ps_3.y_max = 0.5;
	ps_3.num_x = 20;
	ps_3.num_y = 20;
	ps_3.phase_space_2d(map_3);

	ps_4.x_min = 0.0;
	ps_4.x_max = 2.0 * M_PI;
	ps_4.y_min = 2.0;
	ps_4.y_max = 6.0;
	ps_4.num_x = 10;
	ps_4.num_y = 10;
	ps_4.phase_space_2d(map_4);

	ps_5.x_min = 0.0;
	ps_5.x_max = 2.0 * M_PI;
	ps_5.y_min = 0.0;
	ps_5.y_max = 0.2;
	ps_5.num_x = 10;
	ps_5.num_y = 10;
	ps_5.phase_space_2d(map_5);

	/* Winding number */
	Analysis wnd_3;

	wnd_3.wind_x0 = 0.0;
	wnd_3.wind_xf = 0.0;
	wnd_3.wind_y0 = -0.3;	
	wnd_3.wind_yf = 0.2;
	wnd_3.pts_num = 800;
	wnd_3.winding_number(map_3);


	/******************************/
	/*** ClickSpace application ***/
	/******************************/
	int running = 1;
	const int SCR_WIDTH = 1000;
	const int SCR_HEIGHT = 800;
	System sys(SCR_WIDTH, SCR_HEIGHT, map_1, ps_1);

	// Set new map and Analysis features (x,y limits)
	sys.setMap(map_1);
	sys.setAnalysis(ps_1);

	while (running)
	{
		running = sys.run();
	} 

	return 0;
}
