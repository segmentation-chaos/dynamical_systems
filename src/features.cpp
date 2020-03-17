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

int Analysis::winding_number(Maps_2d *map)
{
    /**
    * Receives a map for winding number calculation.
    * 
    * Return: FILE: Winding number points for each action value (map_id/winding.dat)
    *         FILE: Set of initial conditions (tt0, J) (map_id/winding_ic.dat)
    */
    FILE *fout1, *fout2;
    string base_dir = "results/";
    string fl_type = "winding";

    make_dir(base_dir, map->check_id());
    make_file(&fout1, &fout2, base_dir + map->check_id(), fl_type);

    /* Creates initial pts along a straight line */
    /* (x0, y0) -> (xf, yf) */
    double lin_size = sqrt(pow((wind_xf - wind_x0), 2) + pow((wind_yf - wind_y0), 2));
    double lin_ang = acos((wind_xf - wind_x0) / lin_size);
    double delta_lin = (double) lin_size / pts_num;

    double w;
    map->mod = false;

    /* Loop over init. pts */
    iter_num = 10000;
    for (int i = 0; i <= pts_num; i++)
    {
        map->in[0] = wind_x0 + (double) i * delta_lin * cos(lin_ang);
        map->in[1] = wind_y0 + (double) i * delta_lin * sin(lin_ang);

        fprintf(fout2, "%f %f\n", map->in[0], map->in[1]); 
        fprintf(fout1, "%f %f ", map->in[0], map->in[1]);

        for (int n = 0; n < iter_num; n++)
        {
            map->evolve();
			map->in[0] = map->out[0];
			map->in[1] = map->out[1];
        }
        
        w = (map->in[0] - (wind_x0 + (double) i * delta_lin * cos(lin_ang))) / ((double) iter_num);

        fprintf(fout1, "%f\n", w);
    }

    map->mod = true;
    fclose(fout1);
    fclose(fout2);
    
    return 0;
}