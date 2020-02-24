#include "system.hpp"

System::System(unsigned int scrWidth, unsigned int scrHeight,
               Maps_2d *map_type, Analysis anal_type)
{
    map = map_type;
    analy = &anal_type;

    canvas.init(scrWidth, scrHeight);
    canvas.clear();
    canvas.setColor(255, 255, 255, 255);

    // Set system coordinates to map
    canvas.sX_min = analy->x_min;
    canvas.sX_max = analy->x_max;
    canvas.sY_min = analy->y_min;
    canvas.sY_max = analy->y_max;
    canvas.sX_scl_min = canvas.sX_min;
    canvas.sX_scl_max = canvas.sX_max;
    canvas.sY_scl_min = canvas.sY_min;
    canvas.sY_scl_max = canvas.sY_max;

    // Allocate vectors
    canvas.orb_pts.resize(2, vector<double>(0));
    canvas.line_orb.resize(2, vector<double>(0));
}

int System::run()
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
            canvas.drawLine();
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

void System::setMap(Maps_2d *map_in) 
{
    map = map_in;
}

// void System::setAnalysis(Analysis anal_in)
// {
    // analy = &anal_in;
// }