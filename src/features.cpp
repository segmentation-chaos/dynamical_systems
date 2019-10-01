#include <iostream>
#include <math.h>
#include "features.hpp"

int Analysis::orbit_2d(Maps_2d* map)
{
    FILE *fout1, *fout2;
    if(map->check_id() == 1)
    {
        fout1 = fopen("results/standard_map/orbit.dat","w");
        fout2 = fopen("results/standard_map/orbit_ic.dat","w");
    }
    else if(map->check_id() == 2)
    {
        fout1 = fopen("results/henon_map/orbit.dat","w");
        fout2 = fopen("results/henon_map/orbit_ic.dat","w");
    }
    else
    {
        std::cout << "Invalid map type." << std::endl;
        exit(2);
    }
    fprintf(fout2, "%f %f\n", x0, y0);
   	map->in[0] = x0;
	map->in[1] = y0;
	for (int k = 0; k < iter_num; k++)
	{
		fprintf(fout1, "%f %f\n", map->out[0], map->out[1]);
		map->evolve();
		map->in[0] = map->out[0];
		map->in[1] = map->out[1];
	}
    fclose(fout1);
    fclose(fout2);
    return 0;
}

int Analysis::phase_space_2d(Maps_2d* map)
{
    FILE *fout1, *fout2;
    if(map->check_id() == 1)
    {
        fout1 = fopen("results/standard_map/phase_space.dat","w");
        fout2 = fopen("results/standard_map/phase_space_ic.dat","w");
    }
    else if(map->check_id() == 2)
    {
        fout1 = fopen("results/henon_map/phase_space.dat","w");
        fout2 = fopen("results/henon_map/phase_space_ic.dat","w");
    }
    else
    {
        std::cout << "Invalid map type." << std::endl;
        exit(2);
    }
    iter_num = 1000;
    double x, y;
    double delta_x = fabs(x_max - x_min) / ((double) num_x);
    double delta_y = fabs(y_max - y_min) / ((double) num_y);
    x = x_min;
    for (int i = 0; i <= num_x; i++)
    {
        y = y_min;
		for (int j = 0; j <= num_y; j++)
		{
            fprintf(fout2, "%f %f\n", x, y);
			map->in[0] = x;
			map->in[1] = y;
			for (int k = 0; k < iter_num; k++)
			{
				fprintf(fout1, "%f %f\n", map->out[0], map->out[1]);
				map->evolve();
				map->in[0] = map->out[0];
				map->in[1] = map->out[1];
			}
            y += delta_y;
		}
        x += delta_x;
	}
	fclose(fout1);
    fclose(fout2);
    return 0;
}