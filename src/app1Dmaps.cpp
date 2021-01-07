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
    Analysis cb_log, cb_mor, cb_tri, cb_lsin, cb_log2;

    cb_log.x_min = -0.01;
	cb_log.x_max =  1.00;
	cb_log.y_min = -0.01;
	cb_log.y_max =  1.10;

    cb_mor.x_min = -0.01;
	cb_mor.x_max =  5.00;
	cb_mor.y_min = -0.01;
	cb_mor.y_max =  5.00;

    cb_tri.x_min = -0.01;
	cb_tri.x_max =  1.00;
	cb_tri.y_min = -0.01;
	cb_tri.y_max =  1.10;

    cb_lsin.x_min = -0.01;
	cb_lsin.x_max =  1.00;
	cb_lsin.y_min = -0.01;
	cb_lsin.y_max =  1.10;

    cb_log2.x_min = -0.01;
	cb_log2.x_max =  1.00;
	cb_log2.y_min = -0.01;
	cb_log2.y_max =  1.10;

    // Set Analysis and Maps to items
    map_items[log->check_id()] = log;
    map_items[mor->check_id()] = mor;
    map_items[tri->check_id()] = tri;
    map_items[lsin->check_id()] = lsin;
    map_items[log2->check_id()] = log2;
    analysis_items[log->check_id()] = cb_log;
    analysis_items[mor->check_id()] = cb_mor;
    analysis_items[tri->check_id()] = cb_tri;
    analysis_items[lsin->check_id()] = cb_lsin;
    analysis_items[log2->check_id()] = cb_log2;
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

    fWorldPerScreenWidthPixel = (sPosMax.x - sPosMin.x) / pge.ScreenWidth();
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
    sPos = sPosMin;
    SystemToCanvas();
    olc::vi2d pb = cPos;

    for (double x = sPosMin.x; x < sPosMax.x; x += fWorldPerScreenWidthPixel)
    {
        sPos.x = x;
        map->in = x;
        map->evolve();
        sPos.y = map->out;
        SystemToCanvas();
        pge.DrawLine(pb, cPos, functionRGBA);
        pb = cPos;
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

void app1DMap::DrawGUI(olc::PixelGameEngine& pge)
{
    // Print map name
    int nTextScale = 3;
    pge.FillRect({0,  0}, {pge.ScreenWidth(), 8 * 3 * nTextScale}, olc::VERY_DARK_GREY);
    pge.DrawLine({0, 8 * 3 * nTextScale}, {pge.ScreenWidth(), 8 * 3 * nTextScale}, olc::WHITE);
    std::string sText = map->check_id() + " (" + std::to_string(map->get_iter_order()) + " order)";
    int nTextSize = sText.size();
    pge.DrawString({(pge.ScreenWidth() - nTextSize * 8 * nTextScale) / 2, 8 * nTextScale}, sText, olc::WHITE, nTextScale);
}

int app1DMap::run_frame(olc::PixelGameEngine& pge)
{
    pge.Clear(olc::WHITE);
    DrawIdentity(pge);
    DrawAxes(pge);
    DrawFunction(pge);
    DrawGUI(pge);
    // pge.DrawString(20, 20, map->check_id() + " (" + std::to_string(map->get_iter_order()) + " order)", olc::BLACK, 2);

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

    // Set iteration order
    if (pge.GetKey(olc::Key::K1).bPressed) { map->set_iter_order(1); }
    if (pge.GetKey(olc::Key::K2).bPressed) { map->set_iter_order(2); }
    if (pge.GetKey(olc::Key::K3).bPressed) { map->set_iter_order(3); }
    if (pge.GetKey(olc::Key::K4).bPressed) { map->set_iter_order(4); }
    if (pge.GetKey(olc::Key::K5).bPressed) { map->set_iter_order(5); }
    if (pge.GetKey(olc::Key::K6).bPressed) { map->set_iter_order(6); }
    if (pge.GetKey(olc::Key::K7).bPressed) { map->set_iter_order(7); }
    if (pge.GetKey(olc::Key::K8).bPressed) { map->set_iter_order(8); }
    if (pge.GetKey(olc::Key::K9).bPressed) { map->set_iter_order(9); }

    // If mouse is holding
    if (pge.GetMouse(0).bHeld) 
    {
        DrawCobWeb(pge);
    }

    return RUN_1DMAP;
}