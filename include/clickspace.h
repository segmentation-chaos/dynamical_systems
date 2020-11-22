// Load applications
#include "retromenu.h"
#include "app2Dmaps.h"
#include "app1Dmaps.h"

class olcClickSpace : public olc::PixelGameEngine
{
public:
    olcClickSpace() { sAppName = "Click Space"; }

    int32_t RunningID = RUN_MENU;

    // Retro Menu
    RetroMenu menu;
    // Iteractive 2D map
    app2DMap map2D;
    // Iteractive 1D map (cobweb)
    app1DMap map1D;

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};