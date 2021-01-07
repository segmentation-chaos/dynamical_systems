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
	hen->par[0] = 0.25;
	hen->par[1] = 1.0;
	ntw->par[0] = 0.615;
	ntw->par[1] = 0.4;
	ntw->par[2] = 2;
	hal->par[0] = 20.;
	sfm->par[0] = 0.001;

    /* Phase space settings*/
	Analysis ps_std, ps_hen, ps_ntw, ps_hal, ps_sfm;

	ps_std.x_min =  0.0;
	ps_std.x_max =  1.0;
	ps_std.y_min = -0.5;
	ps_std.y_max =  0.5;

	ps_hen.x_min = -1.5;
	ps_hen.x_max =  1.5;
	ps_hen.y_min = -1.5;
	ps_hen.y_max =  1.5;

	ps_ntw.x_min = -0.5;
	ps_ntw.x_max =  0.5;
	ps_ntw.y_min = -0.5;
	ps_ntw.y_max =  0.5;

	ps_hal.x_min =  0.0;
	ps_hal.x_max =  2.0 * M_PI;
	ps_hal.y_min =  2.0;
	ps_hal.y_max =  6.0;

	ps_sfm.x_min =  0.0;
	ps_sfm.x_max =  2.0 * M_PI;
	ps_sfm.y_min =  0.0;
	ps_sfm.y_max =  0.2;

    // Set Analysis and Maps to map
    map_items[std->check_id()] = std;
    map_items[hen->check_id()] = hen;
    map_items[ntw->check_id()] = ntw;
    map_items[hal->check_id()] = hal;
    map_items[sfm->check_id()] = sfm;
    analysis_items[std->check_id()] = ps_std;
    analysis_items[hen->check_id()] = ps_hen;
    analysis_items[ntw->check_id()] = ps_ntw;
    analysis_items[hal->check_id()] = ps_hal;
    analysis_items[sfm->check_id()] = ps_sfm;
}

app2DMap::~app2DMap() 
{
    ClearSystemData();
}

void app2DMap::SetApplication(olc::PixelGameEngine& pge, int32_t command_id)
{
    // Set Initial flags state
    run_color_canvas = false;
    run_color_orbit = false;
    clear_orbits = false;
    clear_frame = false;
    quit = false;

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
    sScale = {(sPosScaleMax.x - sPosScaleMin.x) / (double)pge.ScreenWidth(), 
              (sPosScaleMax.y - sPosScaleMin.y) / (double)pge.ScreenHeight()};

    // Allocate vectors
    orb_chunks.resize(1, 0);
    orb_ics.resize(2, std::vector<double>(0));
    orb_pts.resize(2, std::vector<double>(0));
    line_orb.resize(2, std::vector<double>(0));
    orb_colors.resize(0);

    // Create local menu
    local_menu.sprGFX = new olc::Sprite("./files/sprites/RetroMenuSprite.png");

    local_menu.mo["main"].SetID(LOCAL_2DMAP_MENU).SetTable(1, 1);
    
    local_menu.mo["main"]["Menu(M)"].SetTable(1, 7);
    auto& localMenuMain = local_menu.mo["main"]["Menu(M)"];
    
    localMenuMain["Set Color"].SetID(LOCAL_2DMAP_SETCOLOR).SetTable(1, 2);
    auto& localMenuSetColor = localMenuMain["Set Color"];
    localMenuSetColor["Set Color Orbit"].SetID(LOCAL_2DMAP_SETCOLOR_ORBIT);
    localMenuSetColor["Set Color Canvas"].SetID(LOCAL_2DMAP_SETCOLOR_CANVAS);
    localMenuSetColor["Set Nightmode"].SetID(LOCAL_2DMAP_SETCOLOR_NIGHTMODE);
    localMenuSetColor["Set Lightmode"].SetID(LOCAL_2DMAP_SETCOLOR_LIGHTMODE);
    
    localMenuMain["Set Map"].SetID(LOCAL_2DMAP_SETMAP).SetTable(1, 2);
    auto& localMenuSetMap = localMenuMain["Set Map"];
    localMenuSetMap["Custom Map"].SetID(LOCAL_2DMAP_SETCUSTOMMAP).Enable(false);
    localMenuSetMap["Library"].SetID(LOCAL_2DMAP_SETMAP_LIBRARY).SetTable(1, 4);
    auto& localMenuSetMapLibrary = localMenuSetMap["Library"];
    localMenuSetMapLibrary["Fermi-Ulam"].SetID(SET_2DMAP_FERMIULAM);
    localMenuSetMapLibrary["Halley"].SetID(SET_2DMAP_HALLEY);
    localMenuSetMapLibrary["Henon"].SetID(SET_2DMAP_HENON);
    localMenuSetMapLibrary["Standard"].SetID(SET_2DMAP_STANDARD);
    localMenuSetMapLibrary["Standard Non-Twist"].SetID(SET_2DMAP_STDNONTWST);

    localMenuMain["Set Parameter"].SetID(LOCAL_2DMAP_SETPARAMETER).Enable(false);
    localMenuMain["Set Point"].SetID(LOCAL_2DMAP_SETPOINT).Enable(false);
    localMenuMain["Set Axis"].SetID(LOCAL_2DMAP_SETAXIS).Enable(false);
    localMenuMain["Set Grid"].SetID(LOCAL_2DMAP_SETGRID).Enable(false);
    
    localMenuMain["Controls"].SetID(LOCAL_2DMAP_CONTROLS).Enable(false);
    localMenuMain["Center Frame"].SetID(LOCAL_2DMAP_CLEAR_FRAME);
    localMenuMain["Reset Orbits"].SetID(LOCAL_2DMAP_CLEAR_ORBITS);
    localMenuMain["Reset"].SetID(LOCAL_2DMAP_CLEAR);
    localMenuMain["Save Orbits"].SetID(LOCAL_2DMAP_SAVE_ORBITS);
    localMenuMain["Exit"].SetID(LOCAL_2DMAP_EXIT);

    local_menu.mo.Build();
    local_menu.mm.Open(&local_menu.mo["main"]);

    pge.SetPixelMode(olc::Pixel::NORMAL);
    pointRGBA = { 255, 255, 255, 255};
    backgroundRGBA = { 50,  50,  55, 255};
    color_wheel.SetColorWheel(150, olc::vi2d{5, 500});
}

void app2DMap::CanvasToSystem()
{
    sPos.x = fabs(sPosScaleMax.x - sPosScaleMin.x) * ((double) cPos.x / (double) cSize.x) + sPosScaleMin.x + sOffset.x;
    sPos.y = fabs(sPosScaleMax.y - sPosScaleMin.y) * ((double) (cSize.y - cPos.y) / (double) cSize.y) + sPosScaleMin.y - sOffset.y;
}

void app2DMap::SystemToCanvas()
{
    cPos.x = (int) cSize.x * (sPos.x - sPosScaleMin.x - sOffset.x) / fabs(sPosScaleMax.x - sPosScaleMin.x);
    cPos.y = (int) cSize.y * (1.0 - ((sPos.y - sPosScaleMin.y + sOffset.y) / fabs(sPosScaleMax.y - sPosScaleMin.y)));
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

    sScale = {(sPosScaleMax.x - sPosScaleMin.x) / (double) cSize.x, 
              (sPosScaleMax.y - sPosScaleMin.y) / (double) cSize.y};
}

void app2DMap::ZoomIn(olc::PixelGameEngine& pge)
{
    cPos = {pge.GetMouseX(), pge.GetMouseY()};
    CanvasToSystem();
    sMouseSystemLocBeforeZoom = sPos;

    cZoomFrameA.x = (int) (0.05 * pge.ScreenWidth());
    cZoomFrameA.y = (int) (0.05 * pge.ScreenHeight());
    cZoomFrameB.x = (int) (0.95 * pge.ScreenWidth());
    cZoomFrameB.y = (int) (0.95 * pge.ScreenHeight());
    SetNewScale();

    // Calculate Offset correction from Zooming
    cPos = {pge.GetMouseX(), pge.GetMouseY()};
    CanvasToSystem();
    sOffset.x += (sMouseSystemLocBeforeZoom.x - sPos.x);
    sOffset.y -= (sMouseSystemLocBeforeZoom.y - sPos.y);
}

void app2DMap::ZoomOut(olc::PixelGameEngine& pge)
{
    cPos = {pge.GetMouseX(), pge.GetMouseY()};
    CanvasToSystem();
    sMouseSystemLocBeforeZoom = sPos;

    cZoomFrameA.x = (int) (-0.05 * pge.ScreenWidth());
    cZoomFrameA.y = (int) (-0.05 * pge.ScreenHeight());
    cZoomFrameB.x = (int) (1.05 * pge.ScreenWidth());
    cZoomFrameB.y = (int) (1.05 * pge.ScreenHeight());
    SetNewScale();

    // Calculate Offset correction from Zooming
    cPos = {pge.GetMouseX(), pge.GetMouseY()};
    CanvasToSystem();
    sOffset.x += (sMouseSystemLocBeforeZoom.x - sPos.x);
    sOffset.y -= (sMouseSystemLocBeforeZoom.y - sPos.y);
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
        pge.Draw(cPos, orb_colors[i]);
    }
}

void app2DMap::DrawMenu(olc::PixelGameEngine& pge)
{
    // Draw Menu
    local_menu.mm.Draw(pge, local_menu.sprGFX, {40, 20});
}

void app2DMap::DrawGUI(olc::PixelGameEngine& pge, olc::vi2d cMouse)
{
    // Print map name
    int nTextScale = 3;
    pge.FillRect({0,  0}, {pge.ScreenWidth(), 8 * 3 * nTextScale}, olc::VERY_DARK_GREY);
    pge.DrawLine({0, 8 * 3 * nTextScale}, {pge.ScreenWidth(), 8 * 3 * nTextScale}, olc::WHITE);
    int nTextSize = map->check_id().size();
    pge.DrawString({(pge.ScreenWidth() - nTextSize * 8 * nTextScale) / 2, 8 * nTextScale}, map->check_id(), olc::WHITE, nTextScale);

    // Print mouse position (system coordinates)
    cPos = cMouse;
    CanvasToSystem();
    pge.FillRect({0, pge.ScreenHeight() - 16 * nTextScale}, {pge.ScreenWidth(), pge.ScreenHeight()}, olc::VERY_DARK_GREY);
    pge.DrawLine({0, pge.ScreenHeight() - 17 * nTextScale}, {pge.ScreenWidth(), pge.ScreenHeight() - 17 * nTextScale}, olc::WHITE);
    pge.DrawString({0, pge.ScreenHeight() - 16 * nTextScale}, "x = " + std::to_string(sPos.x), olc::WHITE, nTextScale);
    pge.DrawString({0, pge.ScreenHeight() -  8 * nTextScale}, "y = " + std::to_string(sPos.y), olc::WHITE, nTextScale);

    // Draw Menu
    DrawMenu(pge);
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
    orb_colors.push_back(pointRGBA);

    map->in[0] = map->out[0];
    map->in[1] = map->out[1];

    orb_size += 1;
    total_points += 1;
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

            total_points += 1;
            orb_colors.push_back(pointRGBA);
        }
        orb_size += line_orb[0].size();
        orb_chunks.back() = orb_size;
        line_iter += 1;
    }
    else
    {
        line_orb[0].clear();
        line_orb[1].clear();
            
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
            total_points += 1;
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
        total_points += 2;
    }

    orb_chunks.push_back(orb_size);
}

void app2DMap::ClearSystemData()
{
    orb_colors.clear();
    orb_chunks.clear();
    orb_ics[0].clear();
    orb_ics[1].clear();
    orb_pts[0].clear();
    orb_pts[1].clear();
    line_orb[0].clear();
    line_orb[1].clear();
    total_points = 0;
}

void app2DMap::DebugInfo(olc::PixelGameEngine& pge)
{
    olc::vi2d vFramePos = {2 * pge.ScreenWidth() / 3, 200};
    olc::vi2d vFrameSize = {pge.ScreenWidth() / 3, 600};
    // olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
    // pge.SetPixelMode(olc::Pixel::ALPHA);    
    pge.FillRect(vFramePos, vFrameSize, { 30,  30,  30, 150});
    pge.DrawRect(vFramePos, vFrameSize, {255, 255, 255, 200});
    // pge.SetPixelMode(currentPixelMode);

    float fElapsedTime = pge.GetElapsedTime();
    int nTextSize = 2;
    olc::Pixel pTextColor = olc::WHITE;
    pge.DrawString(vFramePos + olc::vi2d{10,  10}, "fElapsedTime = " + std::to_string(fElapsedTime), pTextColor, nTextSize);
    pge.DrawString(vFramePos + olc::vi2d{10,  30}, "Points = " + std::to_string(total_points),  pTextColor, nTextSize);
    pge.DrawString(vFramePos + olc::vi2d{10,  50}, "x_min = " + std::to_string(sPosScaleMin.x), pTextColor, nTextSize);
    pge.DrawString(vFramePos + olc::vi2d{10,  70}, "y_min = " + std::to_string(sPosScaleMin.y), pTextColor, nTextSize);
    pge.DrawString(vFramePos + olc::vi2d{10,  90}, "x_max = " + std::to_string(sPosScaleMax.x), pTextColor, nTextSize);
    pge.DrawString(vFramePos + olc::vi2d{10, 110}, "y_max = " + std::to_string(sPosScaleMax.y), pTextColor, nTextSize);
    std::string sMouseStatus = pge.GetMouse(0).bHeld ? "Hold" : 
                              (pge.GetMouse(0).bPressed ? "Pressed" : 
                              (pge.GetMouse(0).bReleased ? "Released" : "Free"));
    pge.DrawString(vFramePos + olc::vi2d{10, 130}, "Mouse(L): " + sMouseStatus, pTextColor, nTextSize);
}

void app2DMap::HandleLocalMenu(olc::PixelGameEngine& pge)
{
    // Open Menu
    if (local_menu.mm.GetPanelsSize() == 0)
    {
        local_menu.mm.Open(&local_menu.mo["main"]);
    }

    menuobject* command = nullptr;

    if (pge.GetKey(olc::Key::UP).bPressed)    { local_menu.mm.OnUp();    }
    if (pge.GetKey(olc::Key::DOWN).bPressed)  { local_menu.mm.OnDown();  }
    if (pge.GetKey(olc::Key::LEFT).bPressed)  { local_menu.mm.OnLeft();  }
    if (pge.GetKey(olc::Key::RIGHT).bPressed) { local_menu.mm.OnRight(); }
    if (pge.GetKey(olc::Key::Z).bPressed)     { local_menu.mm.OnBack();  }
    if (pge.GetKey(olc::Key::ENTER).bPressed || 
        pge.GetKey(olc::Key::M).bPressed)     { command = local_menu.mm.OnConfirm(); }

    // Handle command
    if (command != nullptr)
    {
        // sLastAction = "Selected: " + command->GetName() + " ID: " + std::to_string(command->GetID());
        local_menu.mm.Close();

        switch (command->GetID())
        {
            case LOCAL_2DMAP_EXIT:
                quit = true;
                break;
            case LOCAL_2DMAP_SETCOLOR_ORBIT:
                run_color_orbit = true;
                break;
            case LOCAL_2DMAP_SETCOLOR_CANVAS:
                run_color_canvas = true;
                break;
            case LOCAL_2DMAP_SETCOLOR_NIGHTMODE:
                set_night_mode = true;
                break;
            case LOCAL_2DMAP_SETCOLOR_LIGHTMODE:
                set_light_mode = true;
                break;
            case LOCAL_2DMAP_SAVE_ORBITS:
                save_orbits = true;
                break;
            case LOCAL_2DMAP_CLEAR_ORBITS:
                clear_orbits = true;
                break;
            case LOCAL_2DMAP_CLEAR_FRAME:
                clear_frame = true;
                break;
            case LOCAL_2DMAP_CLEAR:
                clear_frame = true;
                clear_orbits = true;
                break;
        }

        if (command->GetID() > SET_2DMAP && command->GetID() < LOCAL_2DMAP_MENU)
        {
            change_map = true;
            set_map_ID = command->GetID();
        }
    }
}

int app2DMap::run_frame(olc::PixelGameEngine& pge)
{
    pge.Clear(backgroundRGBA);
    DrawOrbits(pge);
    HandleLocalMenu(pge);

    // Start timing
    auto tp1 = std::chrono::high_resolution_clock::now();

    // Turn On|Off debug info
    if (pge.GetKey(olc::Key::D).bPressed) { debug = !debug; }

    // Return to main menu
    if (pge.GetKey(olc::Key::ESCAPE).bPressed || quit) 
    {
        ClearSystemData();

        return RUN_MENU; 
    }
   
    // Toggle night mode
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
    
    // Handle Panning
    olc::vi2d cMouse = {pge.GetMouseX(), pge.GetMouseY()};
    if (pge.GetKey(olc::Key::CTRL).bHeld && 
        pge.GetMouse(0).bPressed)
    {
        cStartPan = cMouse;
    }
    else if (pge.GetKey(olc::Key::CTRL).bHeld && 
             pge.GetMouse(0).bHeld)
    {
        sOffset.x -= (double)(cMouse.x - cStartPan.x) * sScale.x;
        sOffset.y -= (double)(cMouse.y - cStartPan.y) * sScale.y;
        cStartPan = cMouse;
        DrawOrbits(pge);
    }

    // Handle Zooming
    if (pge.GetMouseWheel() > 0 && !line_hold) { ZoomIn(pge); }  // Corrects Offset
    if (pge.GetMouseWheel() < 0 && !line_hold) { ZoomOut(pge); } // Corrects Offset

    // Iterate for mouse click
    if (pge.GetMouse(0).bPressed && !pge.GetKey(olc::Key::CTRL).bHeld &&
        !zoom_hold && !line_hold && !run_color_orbit && !run_color_canvas)
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
        orb_colors.push_back(pointRGBA);
        orb_size = 1;

        map->in[0] = sPos.x;
        map->in[1] = sPos.y;

        run_dynamics = true;
        total_points += 1;
    }
    else if (pge.GetMouse(0).bHeld && !pge.GetKey(olc::Key::CTRL).bHeld &&
             !line_hold && !run_color_orbit && !run_color_canvas)
    {
        run_dynamics = true;
    }
    else if (pge.GetMouse(0).bReleased)
    {
        run_dynamics = false;
    }
   
    // Save Orbit
    if ((pge.GetKey(olc::Key::CTRL).bHeld && 
         pge.GetKey(olc::Key::S).bPressed) || save_orbits) 
    {
        std::cout << "Orbit Saved from ";
        std::cout << map->check_id();
        std::cout << "  ==>  ./results/";
        std::cout << map->check_id();
        std::cout << "/canvas_orbit.dat" << std::endl;
        anal->save_orbit(map, orb_pts, orb_ics);
        save_orbits = false;
    }
    
    // Delete last iterated orbit
    if (pge.GetKey(olc::Key::X).bPressed)
    {
        if ((int) orb_pts[0].size() > orb_chunks.back())
        {
            orb_pts[0].resize(orb_pts[0].size() - orb_chunks.back());
            orb_pts[1].resize(orb_pts[1].size() - orb_chunks.back());
            orb_colors.resize(orb_colors.size() - orb_chunks.back());
            total_points -= orb_chunks.back();
        }
        else if ((int) orb_pts[0].size() <= orb_chunks.back())
        {
            orb_pts[0].resize(0);
            orb_pts[1].resize(0);
            orb_colors.resize(0);
            total_points = 0;
        }
    }

    // Delete all orbits
    if ((pge.GetKey(olc::Key::CTRL).bHeld &&
         pge.GetKey(olc::Key::X).bPressed) || clear_orbits)
    {
        ClearSystemData();
        clear_orbits = false;
    }

    // Clear zoom (total zoom out)
    if (pge.GetKey(olc::Key::C).bPressed || clear_frame)
    {
        sOffset = {0, 0};
        sPosScaleMin = sPosMin;
        sPosScaleMax = sPosMax;
        sScale = {(sPosScaleMax.x - sPosScaleMin.x) / (double) cSize.x, 
                  (sPosScaleMax.y - sPosScaleMin.y) / (double) cSize.y};
        clear_frame = false;
    }
 
    // Draw zoom rectangle
    if (pge.GetMouse(1).bPressed && !line_hold)
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
        line_hold = false;
    }
 
    // Increase points in Line of Init. Cond.
    if (line_hold && pge.GetMouseWheel() > 0)
    {
        line_pts += 1;
        if (line_pts > max_line_pts) { line_pts = max_line_pts; }
    }
    else if (line_hold && pge.GetMouseWheel() < 0)
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
        cZoomFrameB.x = pge.GetMouseX();
        cZoomFrameB.y = pge.GetMouseY();
        DrawZoomRect(pge);        
    }
    else if (zoom_set)
    {
        SetNewScale();
        zoom_set = false;
    }

    // Draw Line of Init. Cond.
    if (line_hold && !line_run)
    {
        cLinePosB.x = pge.GetMouseX();
        cLinePosB.y = pge.GetMouseY();
        DrawPointLine(pge);
    }
    else if (line_quit && !line_run)
    {
        line_quit = false;
    }
    else if (line_run)
    {
        IterateLinePts(pge);
    }

    // Draw Frame and Menu
    DrawGUI(pge, cMouse);

    if (run_color_canvas)
    {
        color_wheel.HandleColorWheel(pge, backgroundRGBA, run_color_canvas);
    }

    if (run_color_orbit)
    {
        color_wheel.HandleColorWheel(pge, pointRGBA, run_color_orbit);
    }

    if (set_night_mode)
    {
        backgroundRGBA = NightModeBackgroundRGBA;
        pointRGBA = NightModePointRGBA;
        bNightMode = true;
        set_night_mode = false;
    }
    if (set_light_mode)
    {
        backgroundRGBA = LightModeBackgroundRGBA;
        pointRGBA = LightModePointRGBA;
        bNightMode = false;
        set_light_mode = false;
    }
    
    if (change_map)
    {
        change_map = false;
        ClearSystemData();
        return set_map_ID;
    }

    // Stop timing
    auto tp2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = tp2 - tp1;

    if (debug) { DebugInfo(pge); }

    return RUN_2DMAP;
}
