#include "app2Dmaps.h"

app2DMap::app2DMap()
{
    /* Load all maps */
	Hal_map *hal = new Hal_map();
    Hen_map *hen = new Hen_map();
    Std_map *std = new Std_map();
    Sfum_map *sfm = new Sfum_map();
    Std_ntwist_map *ntw = new Std_ntwist_map();

    /* Create table with IDs */
    IDtable[SET_2DMAP_FERMIULAM]  = sfm->check_id();
    IDtable[SET_2DMAP_HALLEY]     = hal->check_id();
    IDtable[SET_2DMAP_HENON]      = hen->check_id();
    IDtable[SET_2DMAP_STANDARD]   = std->check_id();
    IDtable[SET_2DMAP_STDNONTWST] = ntw->check_id();
	
    /* Map Default Parameters */
	std->par[0] = 1.5;
	hen->par[0] = 1.0;
	hen->par[1] = 0.5;
	ntw->par[0] = 0.615;
	ntw->par[1] = 0.4;
	ntw->par[2] = 2;
	hal->par[0] = 20.;
	sfm->par[0] = 0.001;

    /* Phase space settings*/
	Analysis ps_1, ps_2, ps_3, ps_4, ps_5;

	ps_1.x_min =  0.0;
	ps_1.x_max =  1.0;
	ps_1.y_min = -0.5;
	ps_1.y_max =  0.5;

	ps_2.x_min = -2.0;
	ps_2.x_max =  2.0;
	ps_2.y_min = -2.0;
	ps_2.y_max =  2.0;

	ps_3.x_min = -0.5;
	ps_3.x_max =  0.5;
	ps_3.y_min = -0.5;
	ps_3.y_max =  0.5;

	ps_4.x_min =  0.0;
	ps_4.x_max =  2.0 * M_PI;
	ps_4.y_min =  2.0;
	ps_4.y_max =  6.0;

	ps_5.x_min =  0.0;
	ps_5.x_max =  2.0 * M_PI;
	ps_5.y_min =  0.0;
	ps_5.y_max =  0.2;

    // Set Analysis and Maps to map
    map_items[std->check_id()] = std;
    map_items[hen->check_id()] = hen;
    map_items[ntw->check_id()] = ntw;
    map_items[hal->check_id()] = hal;
    map_items[sfm->check_id()] = sfm;
    analysis_items[std->check_id()] = ps_1;
    analysis_items[hen->check_id()] = ps_2;
    analysis_items[ntw->check_id()] = ps_3;
    analysis_items[hal->check_id()] = ps_4;
    analysis_items[sfm->check_id()] = ps_5;
}

app2DMap::~app2DMap() {}

void app2DMap::SetApplication(olc::PixelGameEngine& pge, int32_t command_id)
{
    // Set canvas dimensions
    cSize.x = pge.ScreenWidth();
    cSize.y = pge.ScreenHeight();

    // Set map to run from ID
    std::string map_name = IDtable[command_id];
    map  = map_items[map_name];
    anal = &analysis_items[map_name];

    // Set system coordinates to map
    sPosMin.x = anal->x_min;
    sPosMax.x = anal->x_max;
    sPosMin.y = anal->y_min;
    sPosMax.y = anal->y_max;
    sPosScaleMin = sPosMin;
    sPosScaleMax = sPosMax;

    // Allocate vectors
    orb_chunks.resize(1, 0);
    orb_ics.resize(2, std::vector<double>(0));
    orb_pts.resize(2, std::vector<double>(0));
    line_orb.resize(2, std::vector<double>(0));
}

void app2DMap::CanvasToSystem()
{
    sPos.x = fabs(sPosScaleMax.x - sPosScaleMin.x) * ((double) cPos.x / (double) cSize.x) + sPosScaleMin.x;
    sPos.y = fabs(sPosScaleMax.y - sPosScaleMin.y) * ((double) (cSize.y - cPos.y) / (double) cSize.y) + sPosScaleMin.y;
}

void app2DMap::SystemToCanvas()
{
    cPos.x = (int) cSize.x * (sPos.x - sPosScaleMin.x) / fabs(sPosScaleMax.x - sPosScaleMin.x);
    cPos.y = (int) cSize.y * (1.0 - ((sPos.y - sPosScaleMin.y) / fabs(sPosScaleMax.y - sPosScaleMin.y)));
}

void app2DMap::SetNewScale()
{
    olc::vd2d sPosScaleMinNew;
    olc::vd2d sPosScaleMaxNew;

    // Update scale to zoomed window
    if (cZoomFrameB.x > cZoomFrameA.x)
    {
        sPosScaleMinNew.x = fabs(sPosScaleMax.x - sPosScaleMin.x) * ((double) cZoomFrameA.x / (double) cSize.x) + sPosScaleMin.x;
        sPosScaleMaxNew.x = fabs(sPosScaleMax.x - sPosScaleMin.x) * ((double) cZoomFrameB.x / (double) cSize.x) + sPosScaleMin.x;
    }
    else
    {
        sPosScaleMaxNew.x = fabs(sPosScaleMax.x - sPosScaleMin.x) * ((double) cZoomFrameA.x / (double) cSize.x) + sPosScaleMin.x;
        sPosScaleMinNew.x = fabs(sPosScaleMax.x - sPosScaleMin.x) * ((double) cZoomFrameB.x / (double) cSize.x) + sPosScaleMin.x;
    }

    if (cZoomFrameA.y < cZoomFrameB.y)
    {
        sPosScaleMinNew.y = fabs(sPosScaleMax.y - sPosScaleMin.y) * ((double) (cSize.y - cZoomFrameB.y) / (double) cSize.y) + sPosScaleMin.y;
        sPosScaleMaxNew.y = fabs(sPosScaleMax.y - sPosScaleMin.y) * ((double) (cSize.y - cZoomFrameA.y) / (double) cSize.y) + sPosScaleMin.y; 
    }
    else
    {
        sPosScaleMaxNew.y = fabs(sPosScaleMax.y - sPosScaleMin.y) * ((double) (cSize.y - cZoomFrameB.y) / (double) cSize.y) + sPosScaleMin.y;
        sPosScaleMinNew.y = fabs(sPosScaleMax.y - sPosScaleMin.y) * ((double) (cSize.y - cZoomFrameA.y) / (double) cSize.y) + sPosScaleMin.y; 
    }

    sPosScaleMin = sPosScaleMinNew;
    sPosScaleMax = sPosScaleMaxNew;
}

void app2DMap::DrawPhasePoint(olc::PixelGameEngine& pge)
{
    pge.Draw(cPos, pointRGBA);
}

void app2DMap::DrawZoomRect(olc::PixelGameEngine& pge)
{
    olc::vi2d RectSize ={(cZoomFrameB.x - cZoomFrameA.x), 
                         (cZoomFrameB.y - cZoomFrameA.y)};
	pge.DrawRect(cZoomFrameA, RectSize, zoomRGBA);
}

void app2DMap::DrawPointLine(olc::PixelGameEngine& pge)
{
    // Draw Line
    pge.DrawLine(cLinePosA, cLinePosB, lineRGBA);

    // Draw position marks (squares)
    olc::vi2d markSize = {6, 6};
    olc::vi2d markPosA = {cLinePosA.x - 3, cLinePosA.y - 3};
    olc::vi2d markPosB = {cLinePosB.x - 3, cLinePosB.y - 3};
    pge.DrawRect(markPosA, markSize, lineRGBA);
    pge.DrawRect(markPosB, markSize, lineRGBA);

    if (line_pts != 0)
    {
        olc::vd2d versor = {(cLinePosB.x - cLinePosA.x) / (double) line_pts, 
                            (cLinePosB.y - cLinePosA.y) / (double) line_pts};

        for (int i = 1; i < line_pts; i++)
        {
            olc::vi2d markPos = {cLinePosA.x - 3 + (int) ((double) i * versor.x),
                                 cLinePosA.y - 3 + (int) ((double) i * versor.y)};
            pge.DrawRect(markPos, markSize, lineRGBA);
        }
    }
}

void app2DMap::DrawOrbits(olc::PixelGameEngine& pge)
{
    for (unsigned int i = 0; i < orb_pts[0].size(); i++)
    {
        sPos.x = orb_pts[0][i];
        sPos.y = orb_pts[1][i];

        SystemToCanvas();
        pge.Draw(cPos, pointRGBA);
    }
}

void app2DMap::IterateMapOnClick(olc::PixelGameEngine& pge)
{
    map->evolve();
    sPos.x = map->out[0];
    sPos.y = map->out[1];

    SystemToCanvas();
    DrawPhasePoint(pge);

    orb_pts[0].push_back(sPos.x);
    orb_pts[1].push_back(sPos.y);

    map->in[0] = map->out[0];
    map->in[1] = map->out[1];

    orb_size += 1;
    orb_chunks.back() = orb_size;
}

void app2DMap::IterateLinePts(olc::PixelGameEngine& pge)
{
    // Run dynamics for points in line
    if (line_iter < line_iter_max)
    {
        for (int p = 0; p < (int) line_orb[0].size(); p++)
        {
            map->in[0] = line_orb[0][p];
            map->in[1] = line_orb[1][p];
            map->evolve();
                
            sPos.x = map->out[0];
            sPos.y = map->out[1];  
            SystemToCanvas();
            DrawPhasePoint(pge);
            orb_pts[0].push_back(sPos.x);
            orb_pts[1].push_back(sPos.y);
            line_orb[0][p] = sPos.x;
            line_orb[1][p] = sPos.y;
        }
        orb_size += line_orb[0].size();
        orb_chunks.back() = orb_size;
        line_iter += 1;
    }
    else
    {
        line_orb[0].clear();
        line_orb[1].clear();
        pge.Clear(backgroundRGBA);
        DrawOrbits(pge);
            
        line_iter = 0;
        line_run  = false;
        line_quit = true;
        line_hold = false;
        orb_size = 0;
    }    
}

void app2DMap::SetLinePtsToRun(olc::PixelGameEngine& pge)
{
    orb_size = 0;
            
    if (line_pts != 0)
    {
        olc::vd2d versor = {(cLinePosB.x - cLinePosA.x) / (double) line_pts,
                            (cLinePosB.y - cLinePosA.y) / (double) line_pts};
 
        for (int i = 0; i <= line_pts; i++)
        {
            cPos.x = cLinePosA.x + (int) ((double) i * versor.x);
            cPos.y = cLinePosA.y + (int) ((double) i * versor.y);
            DrawPhasePoint(pge);
            CanvasToSystem();
            line_orb[0].push_back(sPos.x);
            line_orb[1].push_back(sPos.y);
                
            orb_size += 1;
        }
    }
    else if (line_pts == 0)
    {
        cPos.x = cLinePosA.x;
        cPos.y = cLinePosA.y;
        DrawPhasePoint(pge);
        CanvasToSystem();
        line_orb[0].push_back(sPos.x);
        line_orb[1].push_back(sPos.y);
 
        cPos.x = cLinePosB.x;
        cPos.y = cLinePosB.y;
        DrawPhasePoint(pge);
        CanvasToSystem();
        line_orb[0].push_back(sPos.x);
        line_orb[1].push_back(sPos.y);
 
        orb_size = 2;
    }

    orb_chunks.push_back(orb_size);
}

int app2DMap::run_frame(olc::PixelGameEngine& pge)
{
    pge.Clear(backgroundRGBA);
    DrawOrbits(pge);

    /* Handle Key events */
    // Return to menu
    if (pge.GetKey(olc::Key::ESCAPE).bPressed) 
    {
        orb_chunks.clear();
        orb_ics[0].clear();
        orb_ics[1].clear();
        orb_pts[0].clear();
        orb_pts[1].clear();
        line_orb[0].clear();
        line_orb[1].clear();

        return RUN_MENU; 
    }
   
    // Toggle Night mode style
    if (pge.GetKey(olc::Key::N).bPressed)
    {
        if (bNightMode)
        {
            backgroundRGBA = LightModeBackgroundRGBA;
            pointRGBA = LightModePointRGBA;
            bNightMode = false;
        }
        else
        {
            backgroundRGBA = NightModeBackgroundRGBA;
            pointRGBA = NightModePointRGBA;
            bNightMode = true;
        }
    }
    
    // Iterate for mouse click
    if (pge.GetMouse(0).bPressed && 
        !zoom_hold && !line_hold)
    {
        cPos.x = pge.GetMouseX();
        cPos.y = pge.GetMouseY();
        DrawPhasePoint(pge);

        CanvasToSystem();
        orb_ics[0].push_back(sPos.x);
        orb_ics[1].push_back(sPos.y);
        orb_pts[0].push_back(sPos.x);
        orb_pts[1].push_back(sPos.y);
        orb_chunks.push_back(1);
        orb_size = 1;

        map->in[0] = sPos.x;
        map->in[1] = sPos.y;

        run_dynamics = true;
    }
    else if (pge.GetMouse(0).bHeld)
    {
        run_dynamics = true;
    }
    else if (pge.GetMouse(0).bReleased)
    {
        run_dynamics = false;
    }
   
    // Save Orbit
    if (pge.GetKey(olc::Key::CTRL).bHeld && 
        pge.GetKey(olc::Key::S).bPressed) 
    {
        std::cout << "Orbit Saved from ";
        std::cout << map->check_id();
        std::cout << "  ==>  ./results/";
        std::cout << map->check_id();
        std::cout << "/canvas_orbit.dat" << std::endl;
        anal->save_orbit(map, orb_pts, orb_ics);
    }
    
    // Delete last iterated orbit
    if (pge.GetKey(olc::Key::X).bPressed)
    {
        if ((int) orb_pts[0].size() > orb_chunks.back())
        {
            orb_pts[0].resize(orb_pts[0].size() - orb_chunks.back());
            orb_pts[1].resize(orb_pts[1].size() - orb_chunks.back());
        }
        else if ((int) orb_pts[0].size() <= orb_chunks.back())
        {
            orb_pts[0].resize(0);
            orb_pts[1].resize(0);
        }

        pge.Clear(backgroundRGBA);
        DrawOrbits(pge);
    }

    // Delete all orbits
    if (pge.GetKey(olc::Key::CTRL).bHeld &&
        pge.GetKey(olc::Key::X).bPressed)
    {
        orb_chunks.clear();
        orb_ics[0].clear();
        orb_ics[1].clear();
        orb_pts[0].clear();
        orb_pts[1].clear();
        line_orb[0].clear();
        line_orb[1].clear();

        pge.Clear(backgroundRGBA);
        DrawOrbits(pge);
    }

    // Clear zoom (total zoom out)
    if (pge.GetKey(olc::Key::C).bPressed)
    {
        sPosScaleMin = sPosMin;
        sPosScaleMax = sPosMax;
        pge.Clear(backgroundRGBA);
        DrawOrbits(pge);
    }
 
    // Draw zoom rectangle
    if (pge.GetMouse(1).bPressed)
    {
        cZoomFrameA.x = pge.GetMouseX();
        cZoomFrameA.y = pge.GetMouseY();
        zoom_hold = true;
        zoom_set = false;
    }
    else if (zoom_hold && pge.GetMouse(0).bPressed)
    {
        zoom_hold = false;
        zoom_set = true;
    }

    // Draw Line of Init. Cond.
    if (pge.GetKey(olc::Key::R).bPressed)
    {
        cLinePosA.x = pge.GetMouseX();
        cLinePosA.y = pge.GetMouseY();
        line_hold = true;
    }
    else if (pge.GetKey(olc::Key::S).bPressed && line_hold)
    {
        line_hold = false;
        line_quit = true;
    }
    else if (pge.GetKey(olc::Key::SPACE).bPressed && line_hold)
    {
        cLinePosB.x = pge.GetMouseX();
        cLinePosB.y = pge.GetMouseY();
        SetLinePtsToRun(pge);
        line_run = true;
    }
 
    // Increase points in Line of Init. Cond.
    if (pge.GetMouseWheel() > 0 && line_hold)
    {
        line_pts += 1;
        if (line_pts > max_line_pts) { line_pts = max_line_pts; }
    }
    else if (pge.GetMouseWheel() < 0 && line_hold)
    {
        line_pts -= 1;
        if (line_pts < 0) { line_pts = 0; }
    }
 
    // Iterate dynamical map
    if (run_dynamics && !line_run)
    {
        IterateMapOnClick(pge);
    }

    // Draw zoom rectangle or set new scale
    if (zoom_hold)
    {
        pge.Clear(backgroundRGBA);
        DrawOrbits(pge);
        cZoomFrameB.x = pge.GetMouseX();
        cZoomFrameB.y = pge.GetMouseY();
        DrawZoomRect(pge);        
    }
    else if (zoom_set)
    {
        SetNewScale();
        pge.Clear(backgroundRGBA);
        DrawOrbits(pge);
        zoom_set = false;
    }

    // Draw Line of Init. Cond.
    if (line_hold && !line_run)
    {
        pge.Clear(backgroundRGBA);
        DrawOrbits(pge);
        
        cLinePosB.x = pge.GetMouseX();
        cLinePosB.y = pge.GetMouseY();
        DrawPointLine(pge);
    }
    else if (line_quit && !line_run)
    {
        pge.Clear(backgroundRGBA);
        DrawOrbits(pge);
        line_quit = false;
    }
    else if (line_run)
    {
        IterateLinePts(pge);
    }

    return RUN_2DMAP;
}
