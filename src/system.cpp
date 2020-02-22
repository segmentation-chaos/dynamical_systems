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

    canvas.orb_pts.resize(2, vector<double>(0));

    cout << map->check_id() << endl;
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

            // Init. cond. line points density
            if (canvas.cEvent.type == SDL_MOUSEWHEEL)
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
        if (canvas.mouse_hold) 
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
        if (canvas.line_hold)
        {
            canvas.clear();
            canvas.drawOrbit();
            SDL_GetMouseState(&canvas.line_cX_b, &canvas.line_cY_b);
            canvas.drawLine();
        }
        else if (canvas.line_quit)
        {
            canvas.clear();
            canvas.drawOrbit();
            // canvas.line_run = true;
            canvas.line_quit = false;
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