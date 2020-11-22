#include "app1Dmaps.h"

app1DMap::app1DMap()
{
    /* Load all maps */
    Log_map *log = new Log_map();
    Moran_map *mor = new Moran_map();
    Triang_map *tri = new Triang_map();
    Lin_sin_map *lsin = new Lin_sin_map();
    Log_map_2ndO *log2 = new Log_map_2ndO();

    /* Create table with IDs */
    IDtable[SET_1DMAP_LOGISTIC]    = log->check_id();
    IDtable[SET_1DMAP_LOGISTIC2ND] = log2->check_id();
    IDtable[SET_1DMAP_LINEARSIN]   = lsin->check_id();
    IDtable[SET_1DMAP_MORAN]       = mor->check_id();
    IDtable[SET_1DMAP_TRIANGLE]    = tri->check_id();

    /* Maps parameters */
    log->par[0] = 3.0;
	mor->par[0] = 1.5;
	tri->par[0] = 1.0;
	lsin->par[0] = 1.0;
    log2->par[0] = 3.0;

    /* CobWeb space settings */
    Analysis cb_1, cb_2, cb_3, cb_4, cb_5;

    cb_1.x_min = -0.01;
	cb_1.x_max =  1.00;
	cb_1.y_min = -0.01;
	cb_1.y_max =  1.10;

    cb_2.x_min = -0.01;
	cb_2.x_max =  5.00;
	cb_2.y_min = -0.01;
	cb_2.y_max =  5.00;

    cb_3.x_min = -0.01;
	cb_3.x_max =  1.00;
	cb_3.y_min = -0.01;
	cb_3.y_max =  1.10;

    cb_4.x_min = -0.01;
	cb_4.x_max =  1.00;
	cb_4.y_min = -0.01;
	cb_4.y_max =  1.10;

    cb_5.x_min = -0.01;
	cb_5.x_max =  1.00;
	cb_5.y_min = -0.01;
	cb_5.y_max =  1.10;

    // Set Analysis and Maps to items
    map_items[log->check_id()] = log;
    map_items[mor->check_id()] = mor;
    map_items[tri->check_id()] = tri;
    map_items[lsin->check_id()] = lsin;
    map_items[log2->check_id()] = log2;
    analysis_items[log->check_id()] = cb_1;
    analysis_items[mor->check_id()] = cb_2;
    analysis_items[tri->check_id()] = cb_3;
    analysis_items[lsin->check_id()] = cb_4;
    analysis_items[log2->check_id()] = cb_5;
}

app1DMap::~app1DMap() {}

void app1DMap::SetApplication(olc::PixelGameEngine& pge, int32_t command_id)
{
    // Set canvas dimensions
    cSize.x = pge.ScreenWidth();
    cSize.y = pge.ScreenHeight();

    // Set map to run from ID
    std::string map_name = IDtable[command_id];
    map = map_items[map_name];
    anal = &analysis_items[map_name];

    // Set system coordinates to map
    sPosMin.x = anal->x_min;
    sPosMin.y = anal->y_min;
    sPosMax.x = anal->x_max;
    sPosMax.y = anal->y_max;
    sAxOrigin.x = 0.0;
    sAxOrigin.y = 0.0;
    sAxSize.x = sPosMax.x - sPosMin.x;
    sAxSize.y = sPosMax.y - sPosMin.y;
}

void app1DMap::CanvasToSystem()
{
    sPos.x = fabs(sPosMax.x - sPosMin.x) * ((double) cPos.x / (double) cSize.x) + sPosMin.x;
    sPos.y = fabs(sPosMax.y - sPosMin.y) * ((double) (cSize.y - cPos.y) / (double) cSize.y) + sPosMin.y;
}

void app1DMap::SystemToCanvas()
{
    cPos.x = (int) cSize.x * (sPos.x - sPosMin.x) / fabs(sPosMax.x - sPosMin.x);
    cPos.y = (int) cSize.y * (1.0 - ((sPos.y - sPosMin.y) / fabs(sPosMax.y - sPosMin.y)));
}

void app1DMap::DrawFunction(olc::PixelGameEngine& pge)
{
    for (int p = 0; p < FunctionPts; p++)
    {
        sPos.x = ((double) p / FunctionPts) * fabs(sPosMax.x - sPosMin.x);
        map->in = sPos.x;
        map->evolve();
        sPos.y = map->out;
        SystemToCanvas();
        pge.Draw(cPos, functionRGBA);
    }
}

void app1DMap::DrawIdentity(olc::PixelGameEngine& pge)
{
    sPos = sAxOrigin;
    SystemToCanvas();
    olc::vi2d cPosA = cPos;

    sPos = {sPosMax.x, sPosMax.x};
    SystemToCanvas();
    olc::vi2d cPosB = cPos;

    pge.DrawLine(cPosA, cPosB, identityRGBA);
}

void app1DMap::DrawAxes(olc::PixelGameEngine& pge)
{
    sPos = sAxOrigin;
    SystemToCanvas();
    olc::vi2d cPosOrigin = cPos;

    sPos = sAxSize;
    SystemToCanvas();
    olc::vi2d cSizeX = {cPos.x, cPosOrigin.y};
    olc::vi2d cSizeY = {cPosOrigin.x, cPos.y};
    pge.DrawLine(cPosOrigin, cSizeX, axesRGBA);
    pge.DrawLine(cPosOrigin, cSizeY, axesRGBA);
}

void app1DMap::DrawCobWeb(olc::PixelGameEngine& pge)
{
    cPos.x = pge.GetMouseX();
    cPos.y = pge.GetMouseY();
    olc::vi2d Node = cPos;

    // Draw first vertical line (starting at x axis)
    CanvasToSystem();
    map->in = sPos.x;
    map->evolve();
    sPos.y = map->out;
    SystemToCanvas();
    olc::vi2d LineB = cPos;
    sPos.y = sAxOrigin.y;
    SystemToCanvas();
    olc::vi2d LineA = cPos;
    sPos.y = map->out;
    pge.DrawLine(LineA, LineB, cobwebRGBA);

    cPos = Node;
    // CanvasToSystem();
    for (int i = 1; i < CobWebIter; i++)
    {
        // Vertical line (go to F(X))
        CanvasToSystem();
        map->in = sPos.x;
        map->evolve();
        sPos.y = map->out;
        SystemToCanvas();
        pge.DrawLine(Node, {Node.x, cPos.y}, cobwebRGBA);
        
        // Horizontal line (go to Identity line)
        Node.y = cPos.y;
        sPos.x = sPos.y;
        SystemToCanvas();
        pge.DrawLine(Node, cPos, cobwebRGBA);

        // Update node
        Node.x = cPos.x;
        Node.y = cPos.y;
    }
}

int app1DMap::run_frame(olc::PixelGameEngine& pge)
{
    pge.Clear(olc::WHITE);
    DrawIdentity(pge);
    DrawAxes(pge);
    DrawFunction(pge);

    /* Handle Key events */
    // Return to menu
    if (pge.GetKey(olc::Key::ESCAPE).bPressed) 
    {
        return RUN_MENU; 
    }

    // Decrease map parameter
    if (pge.GetKey(olc::Key::S).bPressed)
    {
        double Param = map->get_par();
        double Delta = fabs(map->get_par_max() - map->get_par_min()) / 50.0;
        map->set_par(Param - Delta);

        if (map->get_par() < map->get_par_min()) 
        {
            map->set_par(map->get_par_min());
        }
    }

    // Increase map parameter
    if (pge.GetKey(olc::Key::W).bPressed)
    {
        double Param = map->get_par();
        double Delta = fabs(map->get_par_max() - map->get_par_min()) / 50.0;
        map->set_par(Param + Delta);

        if (map->get_par() > map->get_par_max())
        {
            map->set_par(map->get_par_max());
        }
    }

    // If mouse is holding
    if (pge.GetMouse(0).bHeld) 
    {  
        DrawCobWeb(pge);
    }

    return RUN_1DMAP;
}