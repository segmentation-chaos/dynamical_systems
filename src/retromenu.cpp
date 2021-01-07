#include "retromenu.h"

RetroMenu::RetroMenu() {}

int RetroMenu::run_frame(olc::PixelGameEngine& pge)
{
    // pge.Clear(olc::Pixel(204, 229, 255));
    pge.Clear(olc::Pixel( 50,  50,  55));
    menuobject* command = nullptr;

    // Handle Key events   
    if (pge.GetKey(olc::Key::M).bPressed)     { mm.Open(&mo["main"]);    }
    if (pge.GetKey(olc::Key::UP).bPressed)    { mm.OnUp();    }
    if (pge.GetKey(olc::Key::DOWN).bPressed)  { mm.OnDown();  }
    if (pge.GetKey(olc::Key::LEFT).bPressed)  { mm.OnLeft();  }
    if (pge.GetKey(olc::Key::RIGHT).bPressed) { mm.OnRight(); }
    if (pge.GetKey(olc::Key::Z).bPressed ||
        pge.GetKey(olc::Key::ESCAPE).bPressed){ mm.OnBack();  }
    if (pge.GetKey(olc::Key::SPACE).bPressed ||
        pge.GetKey(olc::Key::ENTER).bPressed) { command = mm.OnConfirm(); }

    // Handle command
    if (command != nullptr)
    {
        sLastAction = "Selected: " + command->GetName() + " ID: " + std::to_string(command->GetID());
        mm.Close();

        return command->GetID();
    }

    // Draw Logo
    olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
    pge.SetPixelMode(olc::Pixel::ALPHA);
    int32_t LogoScale = 3 * nPixelScale;
    olc::vi2d vLogoPosition = {(pge.ScreenWidth() - LogoScale * sprLOGO->width) / 2, 10};
    pge.DrawSprite(vLogoPosition, sprLOGO, LogoScale);
    pge.SetPixelMode(currentPixelMode);
        
    // Print Current Version
    std::string sVersion = "Version 0.0.1";
    int sVersionSize = sVersion.size();
    pge.DrawString(pge.ScreenWidth() - sVersionSize * 8 * 2, pge.ScreenHeight() - 8 * 2, sVersion, olc::DARK_GREY, 2);

    // Print Engine Credit
    std::string sEngine = "made with olcPixelGameEngine";
    pge.DrawString(0, pge.ScreenHeight() - 8 * 2, sEngine, olc::DARK_GREY, 2);

    // Draw Menu
    olc::vi2d PanelSize = nPatch * mo["main"].GetSizeInPatches();
    mm.Draw(pge, sprGFX, {(pge.ScreenWidth() - PanelSize.x) / 2, 
                          (pge.ScreenHeight() - PanelSize.y) / 2 + 20});

    // Print last command
    pge.DrawString(10, 0.97 * pge.ScreenHeight(), sLastAction, olc::BLACK, 2);

    return RUN_MENU;
}