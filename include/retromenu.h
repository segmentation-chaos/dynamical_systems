#include "command_ids.h"
#include "menumanager.h"
#include "olcPixelGameEngine.h"

class RetroMenu
{
public:
    RetroMenu();
    menuobject mo;
    menumanager mm;

    // Sprites
    olc::Sprite* sprGFX = nullptr;
    olc::Sprite* sprLOGO = nullptr;
    std::string sLastAction;

    int run_frame(olc::PixelGameEngine& pge);
};