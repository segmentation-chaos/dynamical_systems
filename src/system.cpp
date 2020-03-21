#include "system.hpp"

System::System(unsigned int scrWidth, unsigned int scrHeight)
{
    // Load SDL and set canvas
    canvas.init(scrWidth, scrHeight);
    canvas.clear();
    canvas.setColor(255, 255, 255, 255);

    running_sys = 1;
}

int System::run_map_2d()
{
    // Game loop
    while (!canvas.quit)
    {
        // Event handling
        while (SDL_PollEvent(&(canvas.cEvent)) != 0)
        {
            // Close window request
            if (canvas.cEvent.type == SDL_QUIT) { canvas.quit = true; }

            // Run for one click
            if (canvas.cEvent.button.button == SDL_BUTTON_LEFT && !canvas.mouse_hold)
            {
                SDL_GetMouseState(&canvas.cX, &canvas.cY);
                canvas.drawPhasePoint();

                // Convert Canvas -> System
                canvas.CanvasToSystem();

                canvas.orb_pts[0].push_back(canvas.sX);
                canvas.orb_pts[1].push_back(canvas.sY);

                map->in[0] = canvas.sX;
                map->in[1] = canvas.sY;

                canvas.mouse_hold = true;
            }
            else if (canvas.cEvent.type == SDL_MOUSEBUTTONUP)
            {
                canvas.mouse_hold = false;
            }

            // Draw zoom rectangle
            canvas.zoom_quit = false;
            if (canvas.cEvent.button.button == SDL_BUTTON_RIGHT && !canvas.zoom)
            {
                SDL_GetMouseState(&canvas.zoom_cX_a, &canvas.zoom_cY_a);
                canvas.zoom = true;
            }
            else if (canvas.cEvent.type == SDL_MOUSEBUTTONUP && canvas.zoom)
            {
                canvas.zoom = false;
                canvas.zoom_quit = true;
            }

            // Total zoom out
            if (canvas.cEvent.key.keysym.sym == SDLK_c)
            {
                canvas.zoom_clear = true;
            }

            // Draw init. cond. line
            if (canvas.cEvent.key.keysym.sym == SDLK_r && !canvas.line_hold)
            {
                SDL_GetMouseState(&canvas.line_cX_a, &canvas.line_cY_a);
                canvas.line_hold = true;
            }
            else if (canvas.cEvent.key.keysym.sym == SDLK_s && canvas.line_hold)
            {
                canvas.line_hold = false;
                canvas.line_quit = true;
            }
            else if (canvas.cEvent.key.keysym.sym == SDLK_e && canvas.line_hold)
            {
                SDL_GetMouseState(&canvas.line_cX_b, &canvas.line_cY_b);
                double versor[2];

                // Allocates line points in a vector
                if (canvas.line_pts != 0)
                {
                    versor[0] = (canvas.line_cX_b - canvas.line_cX_a) / (double) canvas.line_pts;
                    versor[1] = (canvas.line_cY_b - canvas.line_cY_a) / (double) canvas.line_pts;

                    for (int i = 0; i <= canvas.line_pts; i++)
                    {
                        canvas.cX = canvas.line_cX_a + (int) ((double) i * versor[0]);
                        canvas.cY = canvas.line_cY_a + (int) ((double) i * versor[1]);
                        canvas.drawPhasePoint();
                        canvas.CanvasToSystem();
                        canvas.line_orb[0].push_back(canvas.sX);
                        canvas.line_orb[1].push_back(canvas.sY);
                    }
                }
                else if (canvas.line_pts == 0)
                {
                    canvas.cX = canvas.line_cX_a;
                    canvas.cY = canvas.line_cY_a;
                    canvas.drawPhasePoint();
                    canvas.CanvasToSystem();
                    canvas.line_orb[0].push_back(canvas.sX);
                    canvas.line_orb[1].push_back(canvas.sY);

                    canvas.cX = canvas.line_cX_b;
                    canvas.cY = canvas.line_cY_b;
                    canvas.drawPhasePoint();
                    canvas.CanvasToSystem();
                    canvas.line_orb[0].push_back(canvas.sX);
                    canvas.line_orb[1].push_back(canvas.sY);
                }
                
                canvas.line_run = true;
            }

            // Init. cond. line points density
            if (canvas.cEvent.type == SDL_MOUSEWHEEL && canvas.line_hold)
            {
                if (canvas.cEvent.wheel.y > 0) 
                {
                    canvas.line_pts += 1;
                    if (canvas.line_pts > canvas.max_line_pts) 
                    { 
                        canvas.line_pts = canvas.max_line_pts; 
                    }
                }
                else if (canvas.cEvent.wheel.y < 0)
                {
                    canvas.line_pts -= 1;
                    if (canvas.line_pts < 0) 
                    { 
                        canvas.line_pts = 0;
                    }
                }
            }
        }

        
        // Run dynamical step
        if (canvas.mouse_hold && !canvas.line_run) 
        {
            map->evolve();

            canvas.sX = map->out[0];
            canvas.sY = map->out[1];

            canvas.SystemToCanvas();
            canvas.drawPhasePoint();

            canvas.orb_pts[0].push_back(canvas.sX);
            canvas.orb_pts[1].push_back(canvas.sY);

            map->in[0] = map->out[0];
            map->in[1] = map->out[1];
        }

        // Draw zoom rectangle
        if (canvas.zoom) 
        {
            canvas.clear();
            canvas.drawOrbit();

            SDL_GetMouseState(&canvas.zoom_cX_b, &canvas.zoom_cY_b);
            canvas.drawZoomRect();
        }
        else if (!canvas.zoom && canvas.zoom_quit)
        {
            // Update scale
            canvas.SetNewScale();
            canvas.clear();
            canvas.drawOrbit();
            canvas.zoom_quit = false;
        }

        // Total zoom out
        if (canvas.zoom_clear)
        {
            canvas.sX_scl_min = canvas.sX_min;
            canvas.sX_scl_max = canvas.sX_max;
            canvas.sY_scl_min = canvas.sY_min;
            canvas.sY_scl_max = canvas.sY_max;
            canvas.zoom_clear = false;

            canvas.clear();
            canvas.drawOrbit();
        }

        // Draw Init. Cond. Line
        if (canvas.line_hold && !canvas.line_run)
        {
            canvas.clear();
            canvas.drawOrbit();
            SDL_GetMouseState(&canvas.line_cX_b, &canvas.line_cY_b);
            canvas.drawZoomLine();
        }
        else if (canvas.line_quit && !canvas.line_run)
        {
            canvas.clear();
            canvas.drawOrbit();
            // canvas.line_run = true;
            canvas.line_quit = false;
        }
        else if (canvas.line_run)
        {
            // Run dynamics for points in line
            if (canvas.line_iter < canvas.line_iter_max)
            {
                for (unsigned int p = 0; p < canvas.line_orb[0].size(); p++)
                {
                    map->in[0] = canvas.line_orb[0][p];
                    map->in[1] = canvas.line_orb[1][p];

                    map->evolve();
                    
                    canvas.sX = map->out[0];
                    canvas.sY = map->out[1];  

                    canvas.SystemToCanvas();
                    canvas.drawPhasePoint();

                    canvas.orb_pts[0].push_back(canvas.sX);                  
                    canvas.orb_pts[1].push_back(canvas.sY);

                    canvas.line_orb[0][p] = canvas.sX;
                    canvas.line_orb[1][p] = canvas.sY;                    
                }
                canvas.line_iter += 1;
            }
            else
            {

                canvas.line_orb[0].clear();
                canvas.line_orb[1].clear();

                canvas.clear();
                canvas.drawOrbit();
                canvas.line_iter = 0;
                canvas.line_run = false;
                canvas.line_quit = true;
                canvas.line_hold = false;
            }
        }

        canvas.show();
    }

    if (canvas.quit == true)
    {
        return 0;
    }
    else
    {
        return 1;
    }

    return 1;
}

int System::run_cobweb()
{
    // Game loop
    while (!canvas.quit)
    {
        // Event handling
        while (SDL_PollEvent(&(canvas.cEvent)) != 0)
        {
            // Close window request
            if (canvas.cEvent.type == SDL_QUIT) { canvas.quit = true; }

            // Run for one click
            if (canvas.cEvent.button.button == SDL_BUTTON_LEFT && !canvas.mouse_hold)
            {
                canvas.mouse_hold = true;
            }
            else if (canvas.cEvent.type == SDL_MOUSEBUTTONUP)
            {
                canvas.mouse_hold = false;
            }

            // Increase|Decrease system's main parameter
            // Currently this feature do not consider systems 
            // with more than 1 par. It always changes par[0]
            // Current problem with setting par[0]: map_1d->par[0] += x --> (seg_fault)
            //  Must use get/set functions
            
            // Increase par[0]
            if (canvas.cEvent.key.keysym.sym == SDLK_s)
            {
                double cur_par = map_1d->get_par();
                double incr = abs(map_1d->get_par_max() - map_1d->get_par_min()) / 120.0;
                map_1d->set_par(cur_par - incr);
                
                if (map_1d->get_par() < map_1d->get_par_min()) 
                {
                    map_1d->set_par(map_1d->get_par_min());
                }
            }
            // Decrease par[0]
            if (canvas.cEvent.key.keysym.sym == SDLK_w)
            {
                double cur_par = map_1d->get_par();
                double incr = abs(map_1d->get_par_max() - map_1d->get_par_min()) / 120.0;
                map_1d->set_par(cur_par + incr);

                if (map_1d->get_par() > map_1d->get_par_max())
                {
                    map_1d->set_par(map_1d->get_par_max());
                }
            }
        }

        // Clear canvas
        canvas.clear();
        
        /*** Draw indentity line (X = F(X)) ***/
        // Identity line color
        Uint8 r_id = 50;
        Uint8 g_id = 50;
        Uint8 b_id = 50; 
        // Set origin point
        canvas.sX = 0.0;
        canvas.sY = 0.0;
        canvas.SystemToCanvas();
        int cX0 = canvas.cX;
        int cY0 = canvas.cY;
        // Set final point
        canvas.sX = canvas.sX_max;
        canvas.sY = canvas.sX_max;
        canvas.SystemToCanvas();
        int cXf = canvas.cX;
        int cYf = canvas.cY;
        canvas.drawLine(cX0, cY0, cXf, cYf, r_id, g_id, b_id, 255);

        /*** Draw axes ***/
        // Axes color
        Uint8 r_ax = 0;
        Uint8 g_ax = 130;
        Uint8 b_ax = 0;
        canvas.drawLine(cX0, cY0, cXf, cY0, r_ax, g_ax, b_ax, 255);
        canvas.drawLine(cX0, cY0, cX0, cYf, r_ax, g_ax, b_ax, 255);

        // Function graphs color
        Uint8 r_fc = 0;
        Uint8 g_fc = 50;
        Uint8 b_fc = 200;
        unsigned int pts_fc = 10000;
        for (unsigned int p = 0; p < pts_fc; p++)
        {
            // Draw map function F(X)
            canvas.sX = ((double) p / pts_fc) * abs(canvas.sX_max - canvas.sX_min);
            map_1d->in = canvas.sX;
            map_1d->evolve();
            canvas.sY = map_1d->out;
            canvas.SystemToCanvas();
            canvas.drawPoint(canvas.cX, canvas.cY, r_fc, g_fc, b_fc, 255);
        }
        
        // Iter cobweb
        if (canvas.mouse_hold)
        {
            Uint8 r_cbw = 255;
            Uint8 g_cbw = 0;
            Uint8 b_cbw = 0;
            Uint8 a_cbw = 200;
            unsigned int CobWeb_iter = 5000;
            SDL_GetMouseState(&canvas.cX, &canvas.cY);
            int cX0 = canvas.cX;
            int cY0 = canvas.cY;
    
            for (unsigned int i = 0; i < CobWeb_iter; i++)
            {
                // Vertical line (go to F(X))
                canvas.CanvasToSystem();
                map_1d->in = canvas.sX;
                map_1d->evolve();
                canvas.sY = map_1d->out;
                canvas.SystemToCanvas();
                canvas.drawLine(cX0, cY0, cX0, canvas.cY, r_cbw, g_cbw, b_cbw, a_cbw);

                // Horizontal line (go to Identity line)
                cY0 = canvas.cY;
                canvas.sX = canvas.sY;
                canvas.SystemToCanvas();
                canvas.drawLine(cX0, cY0, canvas.cX, canvas.cY, r_cbw, g_cbw, b_cbw, a_cbw);
                
                // Update to next iter
                cX0 = canvas.cX;
                cY0 = canvas.cY;
            }
        }

        canvas.show();
    }

    if (canvas.quit == true)
    {
        return 0;
    }
    else
    {
        return 1;
    }

    return 1;
}

void System::setMap_1d(Maps_1d *map_in)
{
    map_1d = map_in;
}

void System::setMap(Maps_2d *map_in) 
{
    map = map_in;

    // Allocate vectors
    canvas.orb_pts.resize(2, vector<double>(0));
    canvas.line_orb.resize(2, vector<double>(0));
}

void System::setAnalysis(Analysis anal_in)
{    
    analy = &anal_in;

    // Set system coordinates to map
    canvas.sX_min = analy->x_min;
    canvas.sX_max = analy->x_max;
    canvas.sY_min = analy->y_min;
    canvas.sY_max = analy->y_max;
    canvas.sX_scl_min = canvas.sX_min;
    canvas.sX_scl_max = canvas.sX_max;
    canvas.sY_scl_min = canvas.sY_min;
    canvas.sY_scl_max = canvas.sY_max;
}

int System::sys_quit()
{
    return canvas.canvas_quit();
}