#include <iostream>
#include <cstdlib>
#include <cmath>

#include "canvas.hpp"
#include "maps.hpp"
#include "features.hpp"
#include "system.hpp"

int main(int argc, char **argv)
{
	/* 1D maps */
	Maps_1d *map_1d_1, *map_1d_2, *map_1d_3, *map_1d_4, *map_1d_5;
	
	Log_map log;
	Moran_map mor;
	Triang_map tri;
	Log_map_2ndO log_2;
	Lin_sin_map lsin;
	
	map_1d_1 = &log;
	map_1d_2 = &mor;
	map_1d_3 = &tri;
	map_1d_4 = &log_2;
	map_1d_5 = &lsin;
	log.par[0] = 3.0;
	mor.par[0] = 1.5;
	tri.par[0] = 1.0;
	log_2.par[0] = 3.0;
	lsin.par[0] = 1.0;

	/* Orbit settings */
	Analysis orb_1d_1, orb_1d_2, orb_1d_3, orb_1d_4, orb_1d_5;
	
	orb_1d_1.x0 = 0.1;
	orb_1d_1.iter_num = 500;
	orb_1d_1.orbit_1d(map_1d_1);

	orb_1d_2.x0 = 0.1;
	orb_1d_2.iter_num = 500;
	orb_1d_2.orbit_1d(map_1d_2);

	orb_1d_3.x0 = 0.1;
	orb_1d_3.iter_num = 500;
	orb_1d_3.orbit_1d(map_1d_3);

	orb_1d_4.x0 = 0.1;
	orb_1d_4.iter_num = 500;
	orb_1d_4.orbit_1d(map_1d_4);

	orb_1d_5.x0 = 0.1;
	orb_1d_5.iter_num = 500;
	orb_1d_5.orbit_1d(map_1d_5);


	/* 2D maps */
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
	hen.par[0] = 1.5;
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
	const int SCR_WIDTH = 1000;
	const int SCR_HEIGHT = 900;
	System sys(SCR_WIDTH, SCR_HEIGHT);

	// Set 2d map and Analysis features (x,y limits)
	sys.setMap(map_3);
	sys.setAnalysis(ps_3);
	
	// Set 1d map and Analysis features
	Analysis map_1d_anal;
	map_1d_anal.x_min = -0.01;
	map_1d_anal.x_max =  1.00;
	map_1d_anal.y_min = -0.01;
	map_1d_anal.y_max =  1.10;
	sys.setMap_1d(map_1d_4);
	// sys.setAnalysis(map_1d_anal);

	// Choose which application to run
	sys.running_map_2d = 1;
	sys.running_cobweb = 0;

	while (sys.running_sys)
	{
		// In future implementations, 
		// a menu will handle applications
		while (sys.running_map_2d)
		{
			sys.running_map_2d = sys.run_map_2d();
		}
		
		while (sys.running_cobweb)
		{
			sys.running_cobweb = sys.run_cobweb();
		}
		
		sys.running_sys = 0;
	} 

	return sys.sys_quit();
}
