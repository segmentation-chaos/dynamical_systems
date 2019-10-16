#include "features.hpp"

int Analysis::make_dir(string base_dir, string map_name)
{
    /* Create folder (if not existent) */
    if (-1 == mkdir((base_dir).c_str(), 
        S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
    {
        // Error call
    }
    else
    {
        cout << "Folder created " + base_dir << endl;
    }
    
    if (-1 ==  mkdir((base_dir + map_name).c_str(),
        S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) 
    {
        // Error call
    }
    else 
    {
        cout << "Folder created " + base_dir + map_name << endl;
    }
    
    return 0;
}

int Analysis::make_file(FILE **fout1, FILE **fout2, string dir, string anal_type)
{
    *fout1 = fopen((dir + "/" + anal_type + ".dat").c_str(), "w");
    *fout2 = fopen((dir + "/" + anal_type + "_ic.dat").c_str(), "w");
    return 0;
}

int Analysis::orbit_2d(Maps_2d* map)
{
    /**
    * Receives a map for orbit calculation.
    * Return: FILE: Orbit points (map_id/orbit.dat)
    *         FILE: Initial condition (map_id/orbit_ic.dat)
    */ 
    FILE *fout1, *fout2;
    string base_dir = "results/";
    string fl_type = "orbit";

    make_dir(base_dir, map->check_id());
    make_file(&fout1, &fout2, base_dir + map->check_id(), fl_type);

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
    /**
    * Receives a map for phase space calculation.
    * Return: FILE: Phase space points (map_id/phase_space.dat)
    *         FILE: Set of initial conditions (map_id/phase_space_ic.dat)
    */
    FILE *fout1, *fout2;
    string base_dir = "results/";
    string fl_type = "phase_space";

    make_dir(base_dir, map->check_id());
    make_file(&fout1, &fout2, base_dir + map->check_id(), fl_type);
    
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